// Define section: 
#define t_OFF 0
#define t_ON 1
#define t_LED 2
#define t_LEDS 3
#define t_ALT 4

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


const char lookupTable[] = {
	'o', 'f', '3', t_OFF, 
	'o', 'n', '2', t_ON, 
	'l', 'e', '3', t_LED,
	'l', 'e', '4', t_LEDS,
	'a', 'l', '3', t_ALT,
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


const float appVersion = 1.01; 
const int maxLen = 25;