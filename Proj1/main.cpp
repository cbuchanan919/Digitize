#include <Arduino.h>
#include <Taskscheduler.h>

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
#define t_EOL 255
// ---------------


// function headers
void clearInput();
int getInput();
void printInputDetails();
void parseInput();
void showHelp();
void showVersion();
// ----------------



// led settings
char ledColor; 		// t_green, t_red
char ledStatus;		// t_on, t_off, t_blink
int blinkRate;		// how fast to blink

const char lookupTable[] = {'o', 'f', 3, t_OFF, 
			    'o', 'n', 2, t_ON, 
			    'l', 'e', 3, t_LED,
			    'g', 'r', 5, t_GREEN,
			    'r', 'e', 3, t_RED,
			    's', 'e', 3, t_SET,
			    'b', 'l', 5, t_BLINK,
			    'd', '1', 3, t_D13,
			    's', 't', 6, t_STATUS,
			    'v', 'e', 7, t_VERSION,
			    'h', 'e', 4, t_HELP
			   };

const float appVersion = 0.01;
const int maxLen = 25;
const char nums[] = "0123456789"; 
char input[maxLen + 1]; // used to parse input from serial
char commandTokens[maxLen + 1];


void setup() {
	Serial.begin(9600);
	Serial.setTimeout(10000);
	while (!Serial){ /*wait*/}

}

void loop(){
	commandTokens[0] = t_GREEN;
	Serial.println(commandTokens[0]);
	if (commandTokens[0] == t_GREEN) {Serial.println("yes");}
	showHelp();
	showVersion();
	if (getInput() > 0){
		parseInput();
	}
}


void clearInput() {
	for (int i = 0; i < maxLen + 1; i++){
		input[i] = '\0';
	}
}


/*
 * This gets input from the console. It reads till the return key is pressed, or the max characters reached. 
 * It will write to the input variable (array).
 */
int getInput(){
	clearInput();
	int index = 0;
	int cont = 1;
	int addChar = 1;
	if (Serial){
		Serial.println("Please enter command: ");
		while (cont == 1) {
			if (Serial.available() > 0){
				addChar = 1;
				char check = Serial.read();
				if (check == 32 && input[index - 1] == 32) { /*No double spaces*/ addChar = 0; }
				if (index >= maxLen) { /* max input reached */ cont = 0; }
				if (check == 13){ /* Return key entered */ cont = 0; }
				if (check == 8 || check == 127) {
					// backspace & delete
					addChar = 0;

					index--;
					input[index] = '\0';

					// print new line
					Serial.println();
					Serial.print(input);
				}

				if (cont == 1 && addChar == 1) { 
					input[index] = check; 
					Serial.print(check);
					index++;
				}
			}
		}
		input[index] = '\0';
		Serial.println("\r\nYou Wrote: ");
		Serial.println(input);
		Serial.println();
		//printInputDetails();
	}
	return index;
}

void parseInput(){

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
 * This prints a help menu of available commands.
 *
 * */
void showHelp(){
	Serial.print("Showing help for Arduino Project 1, Version ");
	Serial.print(appVersion);
	Serial.print(". ");
	
	Serial.println();

}

void showVersion(){
	Serial.print("\r\nThe current version of this app is: ");
	Serial.print(appVersion);
	Serial.print("\r\n");
}
