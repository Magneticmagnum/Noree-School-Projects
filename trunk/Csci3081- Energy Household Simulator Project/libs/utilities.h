#include <stdlib.h>
#include <string>
#include <map>
#include <vector>

using namespace std;

/**
 * Given a string and a token, this function will tokenize the line into a
 * vector of strings.
 */
vector<string> vectorize(string line, string token);

/**
 * Given a line, the label is returned.
 */
string extractLabel(string line);

/**
 * Given a time in a 24-hour format (e.g. 14:30) will convert it to ticks.
 */
int convertTimeToTicks(string time);


/**
 * Given the path to the old file and a new file, will copy the old file and
 * rename it to the name specified in new file.
 */
void copyFile(char* old, char* newfile);
