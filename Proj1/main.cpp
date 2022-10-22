#include <Arduino.h>

#define t_LED 2;
#define EOL t_EOL 255;

// function headers
void clearInput();
void getInput();
void printInputDetails();

enum {
	maxLen = 50
};


char nums[11] = "0123456789"; 
char input[maxLen + 1]; // used to parse input from serial

void setup() {
	Serial.begin(9600);
	Serial.setTimeout(10000);
	while (!Serial){ /*wait*/}
	int cont = 1;

}

void loop(){
	getInput();
	
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
		//printInputDetails();
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
