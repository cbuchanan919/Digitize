// function headers
void clearInput();
int getInput();
//void showInputDetails();
void showInputDetails(char *inputArr, int maxLength);
void parseInput();
unsigned char getCommandFromWord(int start, int end);
unsigned int getNumber(int start, int end);
void applyCommands(unsigned char newCmd[6]);
void showLedStatus(LedSettings *led);
void ledControl(LedSettings *led, char overrides);
void showHelp();
void showVersion(float version);
// ----------------