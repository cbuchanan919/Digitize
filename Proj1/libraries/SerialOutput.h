
/*
 * This just prints out the details of each character in input. 
 * It iterates through each character of input and prints out the position followed by the value.
 */
void showInputDetails(char *inputArr, int maxLength) {
	Serial.println("Printing Input Details: ");
	for (int i = 0; i <= maxLength; i++){
		Serial.print(i);
		Serial.print(" - ");
		if (inputArr[i] == '\0'){
			Serial.println("null");
		} else {
			Serial.println(inputArr[i]);
		}
	}
}



/*
 * Prints to serial the status of the referenced led.
 *
 * */
void showLedStatus(LedSettings *led){
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
	} else if (led->ledSetting == t_ALT){
		Serial.print("It is set to alternate colors every ");
		Serial.print(led->blinkRate);
		Serial.print(" ms.\r\n");
	} else {
		Serial.print("It is turned ");
		if (led->ledSetting == t_ON) { 
			Serial.print("on.\r\n"); }
		else {
			Serial.print("off.\r\n"); }
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
 * This prints a help menu of available commands. Note: this method has at least 700-800 bytes of memory used. Shrinking / reducing it may be necessary.
 *
 * */
void showHelp(){
	Serial.print("\r\nShowing help for Arduino Project 1. \r\n");

	Serial.println("\r\nHere are some of the commands that can be entered: \r\n");
	
	Serial.println("\td13 on - turns on the onboard led.");
	Serial.println("\td13 off - turns off the onboard led.");
	Serial.println("\td13 blink - blinks the onboard led every <x> interval.\r\n");

	Serial.println("\tled green - turns on the green led.");
	Serial.println("\tled red - turns on the red led.");
	Serial.println("\tled off - turns off both the green & red led.");
	Serial.println("\tled blink - blinks which ever color of led that was last turned on.");
	Serial.println("\tled alt - alternates colors every <x> interval.\r\n");

	Serial.println("\tset blink <number> - ie. set blink 500 - This will set the interval for both the onboard led and the red/green led's.\r\n");

	Serial.println("\tdebug on - Turns on debug mode. This shows more debug info");
	Serial.println("\tdebug off - Turns off debug mode.\r\n");

	Serial.println("\thelp - displays this help info.\r\n");
	Serial.println("\tversion - displays app version.\r\n");

	Serial.println("\tstatus leds - displays current status for each of the led's.\r\n");

	Serial.println("\r\n\r\n\tNote: commands are not case sensitive.\r\n");


}

/*
 * Prints the version info to serial.
 * */
void showVersion(float version ){
	Serial.print("\r\n\r\nThe current version of this app is: ");
	Serial.print(version);
	Serial.println("\r\nIt was written by Chris Buchanan\r\n\r\n");
}
