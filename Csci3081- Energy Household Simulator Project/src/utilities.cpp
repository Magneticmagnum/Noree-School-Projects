#include "../libs/utilities.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

vector<string> vectorize (string line, string token) {

	int position = 0;
	vector < string > solution;

	// Run through the line.
	while  (position != -1) {

		string piece;

		// Break off the piece and add it to the vector.
		position = line.find (token);
		piece = line.substr (0, position);
		line = line.substr (position + token.length (), line.length ());

		if  (! (piece == ""))
			solution.push_back (piece);
	}

	return solution;
}

string extractLabel (string line) {

	int position = line.find (" ");

	return line.substr (0, position);
}


int convertTimeToTicks (string time) {

	int hours, minutes;

	vector < string > parts = vectorize (time, ":");

	hours = atoi (parts[0].c_str ());
	minutes = atoi (parts[1].c_str ());

	return  (hours * 60 + minutes);
}


void copyFile (char* original, char* copied){

	remove (copied); // Remove old version.

	string line;
	ifstream oldFile (original);
	ofstream newFile (copied);

	// Open file and read it line by line
	if  (oldFile.is_open ())	{

		while (oldFile.good ()){

			// Put the line from the old file into the new file.
			getline  (oldFile,line);
			newFile << line << endl;
		}
	}

	oldFile.close ();
	newFile.close ();
}
