#ifndef UTILITIESTEST_H_
#define UTILITIESTEST_H_

#include <cxxtest/TestSuite.h>

#include "../libs/utilities.h"

#include <iostream>
#include <stdio.h>
#include <fstream>

using namespace std;
using log4cxx::Logger;
using log4cxx::LoggerPtr;
using log4cxx::PropertyConfigurator;
using log4cxx::File;

LoggerPtr utilitiesTestLog(Logger::getLogger("utilitiesTest"));

class utilitiesTest: public CxxTest::TestSuite {

public:


	void test_vectorize0() {
		string line = "hello     world    y'all    !";
		string token = " ";
		vector<string> result = vectorize(line, token);

		TS_ASSERT(result[0] == "hello");
		TS_ASSERT(result[1] == "world");
		TS_ASSERT(result[2] == "y'all");
		TS_ASSERT(result[3] == "!");
	}

	void test_vectorize() {
		string line = "the quick brown fox jumped over the lazy dog";
		string token = " ";
		vector<string> result = vectorize(line, token);

		TS_ASSERT(result[0] == "the");
		TS_ASSERT(result[3] == "fox");
		TS_ASSERT(result[8] == "dog");
	}

	void test_vectorize2() {
		string line = "the quick brown fox jumped over the lazy dog";
		string token = ":";
		vector<string> result = vectorize(line, token);

		TS_ASSERT(result[0] == line);

	}

	void test_vectorize3() {
		cout << "Got here!" << endl;
		string line = "the quick brown fox jumped over the lazy dog";
		string token = "fox";
		vector<string> result = vectorize(line, token);

		TS_ASSERT(result[0] == "the quick brown ");
		TS_ASSERT(result[1] == " jumped over the lazy dog");
	}


	void test_extract_label() {

		string line1 = "sleeping RandomTime waking-up 6:30 7:00";
		string line2 = "waking-up Delay maybe-shower 5";
		string line3 = "maybe-shower Choice start-shower dressing 0.5";

		TS_ASSERT("sleeping" == extractLabel(line1));
		TS_ASSERT("waking-up" == extractLabel(line2));
		TS_ASSERT("maybe-shower" == extractLabel(line3));

	}

	void test_time_to_ticks(){
		string time1 = "00:00";
		string time2 = "00:01";
		string time3 = "00:51";
		string time4 = "01:50";
		string time5 = "10:10";
		string time6 = "99:99";

		TS_ASSERT_EQUALS (0 * 60 + 0, convertTimeToTicks(time1));
		TS_ASSERT_EQUALS (0 * 60 + 1, convertTimeToTicks(time2));
		TS_ASSERT_EQUALS (0 * 60 + 51, convertTimeToTicks(time3));
		TS_ASSERT_EQUALS (1 * 60 + 50, convertTimeToTicks(time4));
		TS_ASSERT_EQUALS (10 * 60 + 10, convertTimeToTicks(time5));
		TS_ASSERT_EQUALS (99 * 60 + 99, convertTimeToTicks(time6));
	}

	void test_copy_file(){
		string line;

		ofstream originalFile ("originalFile", ios::out);
		originalFile << "sleeping RandomTime waking-up 00:04 00:04\n";
		originalFile << "waking-up RandomTime waking-up 00:15 00:25";
		originalFile.close();

		copyFile("originalFile", "newFile");

		ifstream newFile ("newFile");

		getline (newFile,line);
		cout << line << endl;
		TS_ASSERT(line == "sleeping RandomTime waking-up 00:04 00:04");

		getline (newFile, line);
		cout << line << endl;
		TS_ASSERT(line == "waking-up RandomTime waking-up 00:15 00:25");

		remove("originalFile");
		remove("newFile");

	}

};

#endif
