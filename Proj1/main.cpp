#include <Arduino.h>
//#include <Scheduler.h>

// Define section: 
#define t_OFF 0
#define t_ON 1
#define t_LED 2

#define t_GREEN 4
#define t_RED 5

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
void applyCommands(char newCmd[4]);
void ledStatus(LedSettings *led);
void ledControl(LedSettings *led);
void showHelp();
void showVersion();
// ----------------


/*
   const char lookupTable[] = {
   'o', 'f', 3, t_OFF, 
   'o', 'n', 2, t_ON, 
   'l', 'e', 3, t_LED,
   'g', 'r', 5, t_GREEN,
   'r', 'e', 3, t_RED,
   's', 'e', 3, t_SET,
   'b', 'l', 5, t_BLINK,
   'd', '1', 3, t_D13,
   's', 't', 6, t_STATUS,
   'v', 'e', 7, t_VERSION,
   'h', 'e', 4, t_HELP,
   'd', 'e', 5, t_DEBUG
   };
   */

const char lookupTable[] = {
	'o', 'f', '3', t_OFF, 
	'o', 'n', '2', t_ON, 
	'l', 'e', '3', t_LED,
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

char inDebugMode = t_DEBUG;	// if = t_DEBUG, shows more info in console.
LedSettings d13;
LedSettings led;


void setup() {
	Serial.begin(9600);
	while (!Serial){ /*wait*/}
	Serial.setTimeout(500);


	// d13
	d13.ledName = t_D13;
	d13.ledPin = t_D13;
	d13.ledSetting = t_BLINK;

	// led
	led.ledName = t_LED;
	led.ledPin = t_GREEN;


	// initialize pins (leds) as outputs.
	pinMode(t_D13, OUTPUT);		// built in LED
}


void loop(){
	//	showHelp();
	//	showVersion();
	if (getInput() > 0){
		parseInput();
		//		ledStatus(&d13);
		//		ledStatus(&led);
		//		getCommandFromWord(0,2);


	}
	ledControl(&d13);
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
	if (Serial){
		if (commandEntered == 1){
			clearInput();
			commandEntered = 0;
			Serial.println("Please enter command: ");
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
			Serial.println("\r\nYou Wrote: ");
			Serial.println(input);
			Serial.println();
			printInputDetails();
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

	Serial.print("\r\nI found ");
	Serial.print(cmdIndex);
	Serial.print(" commands.\r\n");
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
			if (lookupTable[i] == input[start]){
				if (lookupTable[i + 1] == input[start + 1]) {
					int lookupLen = lookupTable[i + 2] - '0';
					Serial.println("here2");
					Serial.print(lookupLen);
					Serial.print(" - ");
					Serial.println(wordLen);
					if (lookupLen == wordLen){
						return lookupTable[i + 3]; 
					}
				}

				}
				//Serial.println(lookupTable[i]);
			}	
		}
		Serial.println("no command found");
		return t_NO_COMMAND_FOUND;
}


	void applyCommands(char newCmd[4]) {
		int badDefault = 0;
		switch (newCmd[4]) {
			case t_DEBUG:
				if (newCmd[1] == t_ON) {inDebugMode = t_DEBUG; }
				else { inDebugMode = t_EOL; }
				break;
			default:
				// do nothing
				badDefault = 0;



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


	void ledControl(LedSettings *led){
		if (led->ledSetting == t_BLINK){
			int currentMillis = millis();
			if (currentMillis - led->previousMillis >= led->blinkRate){
				led->previousMillis = currentMillis;
				if (led->ledState == HIGH) {
					led->ledState = LOW;
				} else {
					led->ledState = HIGH;
				}
			}


		}
		digitalWrite(led->ledPin, led->ledState);

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
		Serial.print("Showing help for Arduino Project 1, Version ");
		Serial.print(appVersion);
		Serial.print(". \r\n");
		Serial.println("Here are some of the commands that can be entered: ");

		Serial.println("debug on - Turns on debug mode. This shows more debug info");
		Serial.println("debug off - Turns off debug mode.\r\n");




	}


	void showVersion(){
		Serial.print("\r\nThe current version of this app is: ");
		Serial.print(appVersion);
		Serial.print("\r\n");
	}



