#include "Arduino.h"

#define t_LED 2;
#define EOL t_EOL 255;

enum {
	maxLen = 50
};

char nums[11] = "0123456789"; 
char input[maxLen + 1]; // used to parse input from serial



void clearInput();
void getInput();


void setup() {                
	// initialize the digital pin as an output.
	Serial.begin(9600);
	while (!Serial){ /* wait */ }
//	Serial.setTimeout(100);
}

void loop() {
	clearInput();
	getInput();	

	//	Serial.println(input);
}  

void clearInput() {
	for (int i = 0; i < maxLen + 1; i++){
		input[i] = '\0';
	}
}


/*
 * This gets input from the console. It reads till the return key is pressed, or the max characters reached.
 */
void getInput(){
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
	}
}
int getNumber(char *input, int size){
	int result = 0;
	for (int i = 0; i < size; i++){

	}
	return -1;	
}
