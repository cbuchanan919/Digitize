/**
 * The available settings for one led.
*/
struct ledSettings {
	/**
	 * The tokenized name of the led. Currently either t_LED or t_D13
	*/
	unsigned char ledName;
	/**
	 * The pin that the led is associated with. Current options: t_GREEN, t_RED, or t_D13
	*/
	unsigned char ledPin;
	/**
	 * The pin that was last turned on. Same as for ledPin: t_GREEN, t_RED, t_D13
	*/
	unsigned char ledLastPin = t_EOL;
	/**
	 * What overall state the led is set to: t_ON, t_OFF, t_BLINK, or t_ALT
	*/
	unsigned char ledSetting;
	/**
	 * The current state of the led: HIGH, or LOW
	*/
	unsigned char ledState;
	/**
	 * How fast to blink in milliseconds
	*/
	unsigned int  blinkRate = 500;
	/**
	 * Used to store the last blink time.
	*/
	unsigned long previousMillis = 0;
}; 
typedef struct ledSettings LedSettings;
