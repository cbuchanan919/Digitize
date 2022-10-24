#include <Arduino.h>

// Define section: 
#define t_OFF 0
#define t_ON 1
#define t_LED 2
#define t_LEDS 3

#define t_GREEN 7
#define t_RED 8

#define t_SET 9
#define t_BLINK 10
#define t_D13 13
#define t_WORD 14
#define t_STATUS 15
#define t_VERSION 16
#define t_HELP 17
#define t_DEBUG 50
#define t_NO_COMMAND_FOUND 254
#define t_EOL 255
// ---------------


// led settings
struct ledSettings {
	char ledName;			// t_led, t_d13
	char ledPin; 			// t_green, t_red, t_D13
	char ledLastPin = t_EOL;	// t_green, t_red, t_D13
	char ledSetting;		// t_on, t_off, t_blink
	char ledState;			// used for blink: HIGH, LOW
	unsigned int  blinkRate = 500;		// how fast to blink in ms
	unsigned long previousMillis = 0; // used to store last blink time
}; 
typedef struct ledSettings LedSettings;


// function headers
void clearInput();
int getInput();
void printInputDetails();
void parseInput();
char getCommandFromWord(int start, int end);
void applyCommands(char newCmd[6]);
void ledStatus(LedSettings *led);
void ledControl(LedSettings *led);
void showHelp();
void showVersion();
// ----------------



const char lookupTable[] = {
	'o', 'f', '3', t_OFF, 
	'o', 'n', '2', t_ON, 
	'l', 'e', '3', t_LED,
	'l', 'e', '4', t_LEDS,
	'g', 'r', '5', t_GREEN,
	'r', 'e', '3', t_RED,
	's', 'e', '3', t_SET,
	'b', 'l', '5', t_BLINK,
	'd', '1', '3', t_D13,
	's', 't', '6', t_STATUS,
	'v', 'e', '7', t_VERSION,
	'h', 'e', '4', t_HELP,
	'd', 'e', '5', t_DEBUG
};


const float appVersion = 0.01;
const char nums[] = "0123456789"; 
const int maxLen = 25;

char input[maxLen + 1]; 	// used to parse input from serial
int inputIndex = 0; 		// used to store current index of input.

char commandTokens[maxLen + 1];
int commandEntered = 1;

char inDebugMode = t_EOL;	// if = t_DEBUG, shows more info in console.
LedSettings d13;
LedSettings led;


void setup() {
	Serial.begin(9600);
	while (!Serial){ /*wait*/}
	Serial.setTimeout(500);
	Serial.println("Info: Type 'help' for command options.");


	// d13
	d13.ledName = t_D13;
	d13.ledPin = t_D13;
	d13.ledSetting = t_BLINK;

	// led
	led.ledName = t_LED;
	led.ledPin = t_RED;


	// initialize pins (leds) as outputs.
	pinMode(t_D13, OUTPUT);		// built in LED
	pinMode(t_RED, OUTPUT);		// green led
	pinMode(t_GREEN, OUTPUT);	// red led
	led.ledSetting = t_BLINK;
}


void loop(){

	if (getInput() > 0){
		parseInput();
	}
	ledControl(&d13);
	ledControl(&led);
}


/*
 * Sets all characters in input to null. Also sets input index to 0.
 * */
void clearInput() {
	inputIndex = 0;
	for (int i = 0; i < maxLen + 1; i++){
		input[i] = '\0';
	}
}


/*
 * This gets an input charcter from the console. It returns 0 till the return key is pressed, or the max characters reached. Then it returns 1.
 * It will write current character to the input variable (array).
 */
int getInput(){
	int cont = 1;
	int addChar = 1;
	if (inputIndex < 0) {inputIndex = 0;}
	if (Serial){
		if (commandEntered == 1){
			clearInput();
			commandEntered = 0;
			Serial.println("\r\nPlease enter command:");
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

				inputIndex--;
				input[inputIndex] = '\0';

				// print new line
				Serial.println();
				Serial.print(input);
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
				Serial.println("\r\nYou Wrote: ");
				Serial.println(input);
				Serial.println();

				printInputDetails();
			}
		}
	}
	return commandEntered;
}


void parseInput(){
	int start = 0;
	int end = 0;
	char commands[6];
	int cmdIndex = 0;
	char noCmd = t_NO_COMMAND_FOUND;
	for (int i = 0; i < maxLen; i++) {
		if (input[i] == '\0') { 
			i = maxLen; 
		} else {
			if (input[i + 1] == 32 || input[i + 1] == '\0') {
				// next char is space or end of input. end word
				end = i+1;
				char cmd = getCommandFromWord(start, end);
				if (cmd != noCmd) {
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
		Serial.print(" (Command not recognised)\r\n");
	}
	if (inDebugMode == t_DEBUG){
		Serial.print("\r\nI found ");
		Serial.print(cmdIndex);
		Serial.print(" commands.\r\n");
	}
	if (cmdIndex > 0) {
		commands[cmdIndex] = t_EOL;
		applyCommands(commands);
	}
}


/*
 * tries to get a command from a word. It goes through the lookup table and if a match is found, returns the token associated with the command.
 * */
char getCommandFromWord(int start, int end){
	int wordLen = end - start;
	int lookupLen = sizeof(lookupTable)/sizeof(lookupTable[0]);
	if (wordLen >= 2) { 
		for (int i = 0; i < lookupLen; i+= 4){
			if (lookupTable[i] == tolower(input[start])){
				if (lookupTable[i + 1] == tolower(input[start + 1])) {
					int lookupLen = lookupTable[i + 2] - '0';
					if (lookupLen == wordLen){
						return lookupTable[i + 3]; 
					}
				}

			}
			//Serial.println(lookupTable[i]);
		}	
	}
	if (inDebugMode == t_DEBUG){ Serial.println("no command found"); }
	return t_NO_COMMAND_FOUND;
}

void applyCommands(char newCmd[6]){
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
			led.ledLastPin = led.ledPin;
			switch (newCmd[1]){
				case t_GREEN:
					led.ledSetting = t_ON;
					led.ledPin = t_GREEN;
					break;
				case t_RED:
					led.ledSetting = t_ON;
					led.ledPin = t_RED;
					break;
				case t_OFF:
					led.ledSetting = t_OFF;
					break;
				case t_BLINK:
					led.ledSetting = t_BLINK;
					break;
				default:
					break;
			}
//			ledStatus(&led);
			break;
		case t_SET:
			switch (newCmd[1]){
				case t_BLINK:
					Serial.println("setting blink not implemented yet :(");
					break;
				default:
					break;
			}
			break;

		case t_DEBUG:
			if (newCmd[1] == t_ON) {
				inDebugMode = t_DEBUG;
				Serial.println("\r\ndebug mode on");
			} else if (newCmd[1] == t_OFF) {
				inDebugMode = t_EOL;
				Serial.println("\r\ndebug mode off");
			}
			break;
		case t_HELP:
			showHelp();
			break;
		case t_VERSION:
			showVersion();
			break;
		case t_STATUS:
			if (newCmd[1] == t_LEDS){
				ledStatus(&led);
				ledStatus(&d13);
			}
			break;
		default:
			// do nothing
			break;
	}
}

/*
 * This just prints out the details of each character in input. 
 * It iterates through each character of input and prints out the position followed by the value.
 */
void printInputDetails() {
	Serial.println("Printing Input Details: ");
	for (int i = 0; i < maxLen + 1; i++){
		Serial.print(i);
		Serial.print(" - ");
		if (input[i] == '\0'){
			Serial.println("null");
		} else {
			Serial.println(input[i]);
		}
	}
}


int getNumber(char *input, int size){
	int result = 0;
	for (int i = 0; i < size; i++){

	}
	return -1;	
}


/*
 * controls the blinking / status of the led.
 * */
void ledControl(LedSettings *led){
	int currentMillis = millis();
	if (currentMillis - led->previousMillis >= led->blinkRate){
		led->previousMillis = currentMillis;
		switch (led->ledSetting) {

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


/*
 * Prints to serial the status of the referenced led.
 *
 * */
void ledStatus(LedSettings *led){
	Serial.print("\r\nPrinting led status for: ");
	switch (led->ledName) {
		case t_D13:
			Serial.print("D13");
			break;
		case t_LED:
			Serial.print("LED");
			break;
		default:
			Serial.print("an led");
	}
	Serial.print(". \r\n");

	if (led->ledSetting == t_BLINK){
		Serial.print("It is set to blink every ");
		Serial.print(led->blinkRate);
		Serial.print(" ms.\r\n");
	} else {
		Serial.print("It is turned ");
		if (led->ledSetting == t_ON) { 
			Serial.print(" on.\r\n"); }
		else {
			Serial.print(" off.\r\n"); }
	}
	Serial.print("The color is ");
	switch (led->ledPin){
		case t_GREEN:
			Serial.print("green");
			break;
		case t_RED:
			Serial.print("red");
			break;
		default:
			Serial.print("white");
	}
	Serial.print(".\r\n");

}


/*
 * This prints a help menu of available commands.
 *
 * */
void showHelp(){
	Serial.print("\r\nShowing help for Arduino Project 1, Version ");
	Serial.print(appVersion);
	Serial.print(". \r\n");

	Serial.println("\r\nHere are some of the commands that can be entered: \r\n");
	
	Serial.println("\td13 on - turns on the onboard led.");
	Serial.println("\td13 off - turns off the onboard led.");
	Serial.println("\td13 blink - blinks the onboard led every <x> interval.\r\n");

	Serial.println("\tled green - turns on the green led.");
	Serial.println("\tled red - turns on the red led.");
	Serial.println("\tled off - turns off both the green & red led.");
	Serial.println("\tled blink - blinks which ever color of led that was last turned on.\r\n");

	Serial.println("\tset blink <number> - ie. set blink 500 - This will set the interval for both the onboard led and the red/green led's.\r\n");

	Serial.println("\tdebug on - Turns on debug mode. This shows more debug info");
	Serial.println("\tdebug off - Turns off debug mode.\r\n");

	Serial.println("\thelp - displays this help info.\r\n");
	Serial.println("\tversion - displays app version.\r\n");

	Serial.println("\tstatus leds - displays current status for each of the led's.\r\n");

	Serial.println("\r\n\r\n\tNote: commands are not case sensitive.\r\n");


}


void showVersion(){
	Serial.print("\r\nThe current version of this app is: ");
	Serial.print(appVersion);
	Serial.print("\r\n");
}



