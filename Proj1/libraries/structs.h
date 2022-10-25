// led settings
struct ledSettings {
	char ledName;			// t_led, t_d13
	char ledPin; 			// t_green, t_red, t_D13
	char ledLastPin = t_EOL;	// t_green, t_red, t_D13
	char ledSetting;		// t_on, t_off, t_blink, t_alt
	char ledState;			// used for blink: HIGH, LOW
	unsigned int  blinkRate = 500;		// how fast to blink in ms
	unsigned long previousMillis = 0; // used to store last blink time
}; 
typedef struct ledSettings LedSettings;