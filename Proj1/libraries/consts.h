// Define section: 
#define t_OFF 0
#define t_ON 1
#define t_LED 2
#define t_LEDS 3
#define t_ALT 4

#define t_GREEN 5
#define t_RED 6

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

/**
 * Table that commands are checked against. Each 'entry' is 4 characters long. 
 * Characters 1 & 2 of each entry are the first two letters of the command word. Character 3 is the length of the word, 
 * and character 4 is what the tokenized version of that word is.
*/
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

/**
 * Current app version. 
 */
const float appVersion = 1.01; 
/**
 * Max length of a command that can be entered at the serial prompt.
*/
const char maxLen = 15;
