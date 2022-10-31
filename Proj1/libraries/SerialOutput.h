/**
 * This just prints out the details of each character in input. 
 * It iterates through each character of input and prints out the position followed by the value.
 */

void showInputDetails(char *inputArr, int maxLength) {
	Serial.println(F("Input Details: "));
	for (int i = 0; i <= maxLength; i++){
		Serial.print(i);
		Serial.print(" - ");
		if (inputArr[i] == '\0'){
			Serial.println(F("null"));
		} else {
			Serial.println(inputArr[i]);
		}
	}
}



/**
 * Prints to serial the status of the referenced led. 
 * It will display the following details: led name, blinking or solid, turned on or off, how fast it blinks.
 */
void showLedStatus(LedSettings *led){
	Serial.print(F("\r\nStatus for: "));
	switch (led->ledName) {
		case t_D13:
			Serial.print(F("D13"));
			break;
		case t_LED:
			Serial.print(F("LED"));
			break;
		default:
			Serial.print(F("an led"));
	}
	Serial.print(F(". \r\n"));

	if (led->ledSetting == t_BLINK){
		Serial.print(F("Blinks every "));
		Serial.print(led->blinkRate);
		Serial.print(F(" ms.\r\n"));
	} else if (led->ledSetting == t_ALT){
		Serial.print(F("Alternates colors every "));
		Serial.print(led->blinkRate);
		Serial.print(F(" ms.\r\n"));
	} else {
		Serial.print(F("It is turned "));
		if (led->ledSetting == t_ON) { 
			Serial.print(F("on.\r\n")); }
		else {
			Serial.print(F("off.\r\n")); }
	}
	Serial.print(F("Color is "));
	switch (led->ledPin){
		case t_GREEN:
			Serial.print(F("green"));
			break;
		case t_RED:
			Serial.print(F("red"));
			break;
		default:
			Serial.print(F("white"));
	}
	Serial.print(F(".\r\n"));

//	Serial.println("blink rate: ");
//	Serial.println(led->blinkRate);
}

	


/**
 * This prints a help menu of available commands. In trying to save memory, it only shows the name of each command. Not what it does.
 * Note: this method has at least 700-800 bytes of memory used. Shrinking / reducing it may be necessary.
 */
void showHelp(){
//	Serial.println("\r\nHelp:\r\nHere's a list of the available commands:\r\nd13 on\r\nd13 off\r\nd13 blink\r\n\r\n"
//							"led green\r\nled red\r\nled off\r\nled blink\r\nled alt\r\n\r\n"
//							"set blink <number>  (ex. set blink 1000)\r\n"
//							"status leds\r\nversion\r\nhelp\r\n\r\n");
//							//"debug on,  debug off");
    Serial.println(F(
			 "\r\nHelp for Arduino Project 1\r\n\r\n"
			 "Here are the commands that can be entered:\r\n"
			 "\td13 on - onboard led on\r\n" 
			 "\td13 off - onboard led off\r\n"
			 "\td13 blink - blinks onboard led\r\n"
			 "\tled green - green led on\r\n"
			 "\tled red - red led on\r\n"
			 "\tled off - green & red led off\r\n"
			 "\tled blink - blinks led\r\n"
			 "\tled alt - alternates colors\r\n"
			 "\tset blink <number> - ie. set blink 500 (in ms)\r\n"
			 "\tdebug on - debug mode on - shows more debug info\r\n"
			 "\tdebug off - debug mode off\r\n"
			 "\thelp - shows this help file\r\n"
			 "\tversion - shows app version\r\n"
			 "\tstatus leds - shows the status of each led\r\n\r\n"
			 "\tNote: commands are not case sensitive.\r\n"));
}

/**
 * Prints the given app version to serial.
 * */
void showVersion(float version ){
	Serial.println();
	Serial.print("Version ");
	Serial.println(version);
}
