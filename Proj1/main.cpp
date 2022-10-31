#include <Arduino.h>
#include <consts.h>
#include <structs.h>
#include <SerialOutput.h>
#include <funcProto.h>




char input[maxLen + 1]; 	///< An input buffer used to parse input from serial
int inputIndex = 0; 		///< used to store current index of input.

int commandEntered = 1;		///< used to record if a command needs to be parsed. If user presses return, this is set to 1, and input is parsed.

char inDebugMode = t_EOL;	///< if = t_DEBUG, shows more info in console. Currently disabled for space saving purposes.

LedSettings d13;		///< onboard (pin 13) led settings
LedSettings led;		///< 2 color led settings

/**
 * The first / initial function, it's where all initialization is done.
 */
void setup() {
	Serial.begin(9600);
	while (!Serial){ /*wait*/}
	Serial.println(F("For commands, type 'help'"));

	// d13
	d13.ledName = t_D13;
	d13.ledPin = t_D13;
	d13.ledSetting = t_BLINK;

	// led
	led.ledName = t_LED;
	led.ledPin = t_RED;
	led.ledSetting = t_BLINK;


	// initialize pins (leds) as outputs.
	pinMode(t_D13, OUTPUT);		// built in LED
	pinMode(t_RED, OUTPUT);		// green led
	pinMode(t_GREEN, OUTPUT);	// red led
}

/**
 * The second / looping function. 
 * This is the main control method after setup runs. It loops over and over throughout the lifetime of the app.
 */
void loop(){

	if (getInput() > 0){
		parseInput();
	}
	ledControl(&d13, t_EOL);
	ledControl(&led, t_EOL);
}


/**
 * Sets all characters in input to null. Also sets input index to 0.
 */
void clearInput() {
	inputIndex = 0;
	for (int i = 0; i < maxLen + 1; i++){
		input[i] = '\0';
	}
}


/**
 * This gets an input charcter from the console. It returns 0 till the return key is pressed, or the max characters reached. 
 * Then it returns 1. It will write current character to the input variable (array).
 */
int getInput(){
	int cont = 1;
	int addChar = 1;
	if (inputIndex < 0) {inputIndex = 0;}
	if (Serial){
		if (commandEntered == 1){
			clearInput();
			commandEntered = 0;
			Serial.println(F("\r\nEnter command:"));
		}
		if (Serial.available() > 0){
			addChar = 1;
			char check = Serial.read();
			if (check == 32 && input[inputIndex - 1] == 32) { /*No double spaces*/ addChar = 0; }
			if (inputIndex >= maxLen) { /* max input reached */ cont = 0; }
			if (check == 13){ 
				/* Return key entered */ 
				cont = 0; 
				commandEntered = 1;
			}
			if (check == 8 || check == 127) {
				// backspace & delete
				addChar = 0;
									
				// print new line
				if (input[0] != '\0'){
					Serial.print("<<");
					Serial.println();
				}
				
				if (inputIndex > 0) { inputIndex--; }
				input[inputIndex] = '\0';

				if (input[0] != '\0'){
					Serial.print(input);
				}
				
				
			}

			if (cont == 1 && addChar == 1) { 
				input[inputIndex] = check; 
				Serial.print(check);
				inputIndex++;
			}
		}
		if (commandEntered == 1){
			// command entered. show what was entered.
			input[inputIndex] = '\0';
		
			if (inDebugMode == t_DEBUG){
				Serial.println(F("\r\nYou Wrote: "));
				Serial.println(input);
				Serial.println("\r\n");
				Serial.print(F("InputIndex: "));
				Serial.print(inputIndex);
				Serial.println();
				showInputDetails(&input[0], maxLen);
			}
		
		}
	}
	return commandEntered;
}

/**
 * It parses all the characters of the input buffer looking for commands. For each word detected, 
 * it checks to see if it's a recognised command. If it is, it adds it to a command buffer. 
 * It then passes the command buffer to the apply commands method.
 */
void parseInput(){
	int start = 0;
	int end = 0;
	unsigned char commands[6];
	int cmdIndex = 0;
	unsigned char noCmd = t_NO_COMMAND_FOUND;
	for (int i = 0; i < maxLen; i++) {
		if (input[i] == '\0') { 
			i = maxLen; 
		} else {
			if (input[i + 1] == 32 || input[i + 1] == '\0') {
				// next char is space or end of input. end word
				end = i+1;
				unsigned char cmd = getCommandFromWord(start, end);
				if (cmd == noCmd) {
					// no command match found. check for int
					if (cmdIndex > 1 && commands[cmdIndex - 2] == t_SET && commands[cmdIndex - 1] == t_BLINK){
						// set blink. check for number
						unsigned int num = getNumber(start, end);
						/*
						if (inDebugMode == t_DEBUG){
							Serial.print(F("\r\nNumber: "));
							Serial.print(num);
						}
						*/
						if (num > 0){
							// number found
							if (num < 256){
								commands[cmdIndex] = (unsigned char)num;
								cmdIndex++;
							} else {
								// convert int to 2 bytes...
								commands[cmdIndex] = t_WORD;
								cmdIndex++;
								commands[cmdIndex] = (num >> 8) & 0xFF;
								cmdIndex++;
								commands[cmdIndex] = (num >> 0) & 0xFF;
								cmdIndex++;
							}
						}
							
					}

				} else {
					// command match found
					commands[cmdIndex] = cmd;
					cmdIndex++;
				}
			}
			if (input[i - 1] == 32) {
				start = i;
				end = i;
			}

		}
	} 
	if (cmdIndex == 0) {
		// no command recognised
		Serial.print(F(" (unrecognised command)\r\n"));
	}

	if (inDebugMode == t_DEBUG){
		Serial.print(F("\r\nI found "));
		Serial.print(cmdIndex);
		Serial.print(F(" commands.\r\n"));
	}

	if (cmdIndex > 0) {
		commands[cmdIndex] = t_EOL;
		applyCommands(commands);
	}
}


/**
 * Tries to get a command from a word. It goes through the lookup table and if a match is found, returns the token associated with the command. 
 * int start: starting index of the word.
 * int end: last character of the word.
 * */
unsigned char getCommandFromWord(int start, int end){
	int wordLen = end - start;
	int lookupLen = sizeof(lookupTable)/sizeof(lookupTable[0]);
	if (wordLen >= 2) { 
		char s = tolower(input[start]);
		char s1 = tolower(input[start + 1]);
		for (int i = 0; i < lookupLen; i+= 4){
			if (lookupTable[i] == s){
				if (lookupTable[i + 1] == s1) {
					int lookupLen = lookupTable[i + 2] - '0';
					if (lookupLen == wordLen){
						return lookupTable[i + 3]; 
					}
				}

			}
		}	
	}

	if (inDebugMode == t_DEBUG){ Serial.println(F("no command found")); }
	
	return t_NO_COMMAND_FOUND;
}

/**
 * Returns an integer from the given location in the input string if found. Else returns 0.
 */
unsigned int getNumber(int start, int end){
	unsigned int result = 0;
	char check;
	for (int i = start; i < end; i++){
		check = input[i];
		if (check > '9' || check < '0') {
			return 0; // some error occured.
		}
		result = (result*10) + check - '0';
	}
	return result;	
}


/**
 * Performs given command based on the logic in the switch tree. 
 */
void applyCommands(unsigned char newCmd[6]){
	switch (newCmd[0]) {
		case t_D13:
			switch (newCmd[1]){
				case t_ON:
					d13.ledSetting = t_ON;
					break;
				case t_OFF:
					d13.ledSetting = t_OFF;
					break;
				case t_BLINK:
					d13.ledSetting = t_BLINK;
					break;
				default:
					break;
			}
			break;
		case t_LED:
			switch (newCmd[1]){
				case t_GREEN:
					led.ledSetting = t_ON;
					led.ledLastPin = led.ledPin;
					led.ledPin = t_GREEN;
					break;
				case t_RED:
					led.ledSetting = t_ON;
					led.ledLastPin = led.ledPin;
					led.ledPin = t_RED;
					break;
				case t_OFF:
					led.ledSetting = t_OFF;
					break;
				case t_BLINK:
					led.ledSetting = t_BLINK;
					break;
				case t_ALT:
					led.ledSetting = t_ALT;
					led.ledLastPin = t_RED;
					led.ledPin = t_GREEN;
					led.ledState = HIGH;
					break;
				default:
					break;
			}
			break;
		case t_SET:
			switch (newCmd[1]){
				case t_BLINK:
					{
					unsigned int blinkRate = 0;
					switch (newCmd[2]){
						case t_WORD:
								blinkRate = (newCmd[3] << 8) + newCmd[4];
							break;
						case t_EOL:
							// badly implemented.
							break;
						default:
							unsigned char c = newCmd[2];
							blinkRate = (unsigned int)c;
						break;
					}
					
					if (inDebugMode == t_DEBUG){
						Serial.print(F("The converted int: "));
						Serial.print(blinkRate);
					}
					
					if (blinkRate > 0){
						led.blinkRate = blinkRate;
						d13.blinkRate = blinkRate;	
					}
					}
					break;
				default:
					break;
			}
			break;

		case t_DEBUG:
			
			if (newCmd[1] == t_ON) {
				inDebugMode = t_DEBUG;
				Serial.println(F("\r\ndebug mode on"));
			} else if (newCmd[1] == t_OFF) {
				inDebugMode = t_EOL;
				Serial.println(F("\r\ndebug mode off"));
			}
			
			break;
		case t_HELP:
			showHelp();
			break;
		case t_VERSION:
			showVersion(appVersion);
			break;
		case t_STATUS:
			if (newCmd[1] == t_LEDS){
				showLedStatus(&led);
				showLedStatus(&d13);
			}
			break;
		default:
			// do nothing
			break;
	}
	// force check of validity of the led's
	ledControl(&led, t_ON);
	ledControl(&d13, t_ON);
}



/**
 * controls the blinking / status of the given led. Setting override to t_ON will force the check.
 * */
void ledControl(LedSettings *led, char override){
	int currentMillis = millis();
	if (currentMillis - led->previousMillis >= led->blinkRate || override == t_ON){
		led->previousMillis = currentMillis;
		switch (led->ledSetting) {
			case t_ALT:
				{
				char tmpPrev = led->ledLastPin;
				led->ledLastPin = led->ledPin;
				led->ledPin = tmpPrev;
				}
				break;
			case t_BLINK:
				if (led->ledState == HIGH) {
					led->ledState = LOW;
				} else {
					led->ledState = HIGH;
				}

				break;
			case t_ON:
				led->ledState = HIGH;
				break;
			case t_OFF:
				led->ledState = LOW;
				break;
			default:
				break;
		}


		digitalWrite(led->ledLastPin, LOW);
		digitalWrite(led->ledPin, led->ledState);

	}
}


