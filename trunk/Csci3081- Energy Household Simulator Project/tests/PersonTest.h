#ifndef PERSONTEST_H_
#define PERSONTEST_H_

#include <cxxtest/TestSuite.h>

#include "../libs/Person.h"
#include <fstream>

#include <iostream>

using namespace std;
using log4cxx::Logger;
using log4cxx::LoggerPtr;
using log4cxx::PropertyConfigurator;
using log4cxx::File;

LoggerPtr personTestLog(Logger::getLogger("PersonTest"));

#define TEST_ITERATIONS 100
#define FILE_NAME "testScript"

class PersonTest: public CxxTest::TestSuite {
	friend class Person;

public:

	void test_simple_random_event() {

		map<string, string> configuration;

		configuration ["filename"] = FILE_NAME;
		configuration ["startState"] = "sleeping";

		ofstream testingFile (FILE_NAME, ios::out);
		testingFile << "sleeping RandomTime waking-up 00:04 00:04\n";
		testingFile << "waking-up RandomTime waking-up 00:15 00:25";
		testingFile.close();

		House house;
		Person* p = new Person(house, configuration);

		house.registerModel( (Model *)p);

		TS_ASSERT (p->getState() == "sleeping");

		house.run(5);

		TS_ASSERT (p->getState() == "waking-up");

		remove(FILE_NAME);
	}

	void test_simple_delay_event() {
		map<string, string> configuration;

		configuration ["filename"] = FILE_NAME;
		configuration ["startState"] = "sleeping";

		ofstream testingFile (FILE_NAME, ios::out);
		testingFile << "sleeping Delay waking-up 5\n";
		testingFile << "waking-up RandomTime waking-up 00:15 00:25";
		testingFile.close();

		House house;
		Person* p = new Person(house, configuration);

		house.registerModel( (Model *)p);

		TS_ASSERT (p->getState() == "sleeping");

		house.run(6);

		TS_ASSERT (p->getState() == "waking-up");

		remove(FILE_NAME);
	}

	void test_simple_choice_event1() {

		map<string, string> configuration;

		configuration ["filename"] = FILE_NAME;
		configuration ["startState"] = "maybe-shower";

		ofstream testingFile (FILE_NAME, ios::out);

		testingFile << "maybe-shower Choice start-shower dressing 0\n";
		testingFile << "start-shower Delay dressing 5";
		testingFile << "dressing Delay breakfast 5";
		testingFile.close();

		House house;
		Person* p = new Person(house, configuration);

		house.registerModel( (Model *)p);

		TS_ASSERT (p->getState() == "maybe-shower");

		house.run(1);

		TS_ASSERT (p->getState() == "dressing");

		remove(FILE_NAME);
	}

	void test_simple_choice_event2() {

		map<string, string> configuration;

		configuration ["filename"] = FILE_NAME;
		configuration ["startState"] = "maybe-shower";


		ofstream testingFile (FILE_NAME, ios::out);

		testingFile << "maybe-shower Choice start-shower dressing 1\n";
		testingFile << "start-shower Delay dressing 5";
		testingFile << "dressing Delay breakfast 5";
		testingFile.close();

		House house;
		Person* p = new Person(house, configuration);

		house.registerModel( (Model *)p);

		TS_ASSERT (p->getState() == "maybe-shower");

		house.run(1);

		TS_ASSERT (p->getState() == "start-shower");

		remove(FILE_NAME);
	}

	void test_simple_choice_event3() {

		map<string, string> configuration;

		configuration ["filename"] = FILE_NAME;
		configuration ["startState"] = "maybe-shower";


		int choiceOneCount = 0;
		int choiceTwoCount = 0;

		for(int i = 0; i < TEST_ITERATIONS; i++){

			ofstream testingFile (FILE_NAME, ios::out);

			testingFile << "maybe-shower Choice start-shower dressing .5\n";
			testingFile << "start-shower Delay dressing 5";
			testingFile << "dressing Delay breakfast 5";
			testingFile.close();

			House house;
			Person* p = new Person(house, configuration);

			house.registerModel( (Model *)p);

			TS_ASSERT (p->getState() == "maybe-shower");

			house.run(1);

			if(p->getState() == "start-shower")
				choiceOneCount++;

			if(p->getState() == "dressing")
				choiceTwoCount++;

			remove(FILE_NAME);

		}

		TS_ASSERT(choiceOneCount && choiceTwoCount);
	}

	void test_simple_random_delay_event() {

		map<string, string> configuration;

		configuration ["filename"] = FILE_NAME;
		configuration ["startState"] = "makeToast";

		ofstream testingFile (FILE_NAME, ios::out);
		testingFile << "makeToast RandomDelay work 10 15\n";
		testingFile << "work RandomTime dinner 01:15 02:25";
		testingFile.close();

		House house;
		Person* p = new Person(house, configuration);

		house.registerModel( (Model *)p);

		TS_ASSERT (p->getState() == "makeToast");

		house.run(9);

		TS_ASSERT(p->getState() == "makeToast");

		house.run(18);

		TS_ASSERT (p->getState() == "work");

		remove(FILE_NAME);
	}


	void test_simple_activate_event() {
		map<string, string> emptyConfiguration;
		map<string, string> configuration;

		configuration ["filename"] = FILE_NAME;
		configuration ["startState"] = "makeToast";


		ofstream testingFile (FILE_NAME, ios::out);
		testingFile << "makeToast Activate work Toaster 0\n";
		testingFile << "work RandomTime dinner 01:15 02:25";
		testingFile.close();

		House house;
		Person* p = new Person(house, configuration);
		Toaster* toaster1 = new Toaster(emptyConfiguration);

		house.registerModel( (Model *)p);
		house.registerModel( (Model *)toaster1);

		TS_ASSERT (p->getState() == "makeToast");

		house.run(2);

		TS_ASSERT (p->getState() == "work");
		TS_ASSERT_EQUALS(toaster1->getEnergy(), 450);

		remove(FILE_NAME);
	}

	void test_simple_activate_event_stove() {
		map<string, string> emptyConfiguration;
		map<string, string> configuration;

		configuration ["filename"] = FILE_NAME;
		configuration ["startState"] = "onStove1";


		ofstream testingFile (FILE_NAME, ios::out);
		testingFile << "onStove1 Activate onStove2 Stove 2 LOW 2\n";
		testingFile << "onStove2 Activate onStove3 Stove 2 MEDIUM 2\n";
		testingFile << "onStove3 Activate onLights1 Stove 20 HIGH 2\n";
		testingFile << "onLights1 Activate offLights2 Lights 2 ON 5 2\n";
		testingFile << "offLights2 Activate onLights3 Lights 2 OFF 2\n";
		testingFile.close();

		House house;
		Person* p = new Person(house, configuration);
		Stove* stove1 = new Stove(emptyConfiguration);
		Lights* lights = new Lights(emptyConfiguration);

		house.registerModel( (Model *)p);
		house.registerModel( (Model *)stove1);
		house.registerModel( (Model *)lights);

		TS_ASSERT (p->getState() == "onStove1");

		house.run(45);

		TS_ASSERT (p->getState() == "onLights3");

		remove(FILE_NAME);
	}

	void test_simple_script_event() {
		map <string, string> emptyConfiguration;

		map <string, string> configuration;

		configuration ["filename"] = FILE_NAME;
		configuration ["startState"] = "sleeping";

		ofstream testingFile (FILE_NAME, ios::out);
		testingFile << "sleeping RandomTime wake-up 00:10 00:15\n";
		testingFile << "wake-up Delay make-coffee 5\n";
		testingFile << "make-coffee Activate make-toast Stove 2 HIGH 5\n";
		testingFile << "make-toast Activate eat Toaster 0\n";
		testingFile << "eat RandomDelay work 5 10\n";
		testingFile << "work RandomTime go-home 08:00 08:30";
		testingFile.close();


		House house;
		Person* p = new Person(house, configuration);
		Stove* stove1 = new Stove(emptyConfiguration);
		Toaster* toaster1 = new Toaster(emptyConfiguration);

		house.registerModel( (Model *)p);
		house.registerModel( (Model *)stove1);
		house.registerModel( (Model *)toaster1);

		TS_ASSERT (p->getState() == "sleeping");

		house.run(10);

		while(p->getState() != "wake-up")
			house.run(1);

		TS_ASSERT (p->getState() == "wake-up");

		house.run(6);

		TS_ASSERT(p->getState() == "make-coffee");

		house.run(1);

		TS_ASSERT (p->getState() == "make-toast");

		house.run(1);

		TS_ASSERT (p->getState() == "eat");

		house.run(15);

		TS_ASSERT (p->getState() == "work");

		remove(FILE_NAME);
	}

	void test_commented_script_event() {
		map <string, string> emptyConfiguration;

		map <string, string> configuration;

		configuration ["filename"] = FILE_NAME;
		configuration ["startState"] = "sleeping";

		ofstream testingFile (FILE_NAME, ios::out);
		testingFile << "sleeping RandomTime wake-up 00:10 00:15\n";
		testingFile << "#a comment\n";
		testingFile << "wake-up Delay make-coffee 5\n";
		testingFile << "#another comment\n";
		testingFile << "make-coffee Activate make-toast Stove 2 HIGH 5\n";
		testingFile << "make-toast Activate eat Toaster 0\n";
		testingFile << "eat RandomDelay work 5 10\n";
		testingFile << "#final comment\n";
		testingFile << "work RandomTime go-home 08:00 08:30";
		testingFile.close();


		House house;
		Person* p = new Person(house, configuration);
		Stove* stove1 = new Stove(emptyConfiguration);
		Toaster* toaster1 = new Toaster(emptyConfiguration);

		house.registerModel( (Model *)p);
		house.registerModel( (Model *)stove1);
		house.registerModel( (Model *)toaster1);

		TS_ASSERT (p->getState() == "sleeping");

		house.run(10);

		while(p->getState() != "wake-up")
			house.run(1);

		TS_ASSERT (p->getState() == "wake-up");

		house.run(6);

		TS_ASSERT(p->getState() == "make-coffee");

		house.run(1);

		TS_ASSERT (p->getState() == "make-toast");

		house.run(1);

		TS_ASSERT (p->getState() == "eat");

		house.run(15);

		TS_ASSERT (p->getState() == "work");

		remove(FILE_NAME);
	}

	void test_repeating_event() {

		map<string, string> configuration;

		configuration ["filename"] = FILE_NAME;
		configuration ["startState"] = "delay1";

		ofstream testingFile (FILE_NAME, ios::out);
		testingFile << "delay1 Delay delay2 6\n";
		testingFile << "delay2 Delay delay1 6";
		testingFile.close();

		House house;
		Person* p = new Person(house, configuration);

		house.registerModel( (Model *)p);


		TS_ASSERT (p->getState() == "delay1");

		house.run(7);

		TS_ASSERT (p->getState() == "delay2");

		house.run(7);

		TS_ASSERT (p->getState() == "delay1");

		house.run(7);

		TS_ASSERT (p->getState() == "delay2");

		remove(FILE_NAME);
	}


	void test_invalid_event() {
		map<string, string> configuration;

		configuration ["filename"] = FILE_NAME;
		configuration ["startState"] = "delay";

		ofstream testingFile (FILE_NAME, ios::out);
		testingFile << "delay Delay waking-up 1";
		testingFile.close();

		House house;
		Person* p = new Person(house, configuration);

		house.registerModel( (Model *)p);

		house.run(50);
	}

	void test_invalid_start_state() {
		map<string, string> configuration;

		configuration ["filename"] = FILE_NAME;
		configuration ["startState"] = "notARealStartState";

		ofstream testingFile (FILE_NAME, ios::out);
		testingFile << "delay Delay waking-up 1";
		testingFile.close();

		House house;
		Person* p = new Person(house, configuration);

		house.registerModel( (Model *)p);

		house.run(50);
	}


	void test_invalid_file() {
		map<string, string> configuration;

		configuration ["filename"] = "doesNotExist.txt";
		configuration ["startState"] = "delay";

		House house;
		Person* p = new Person(house, configuration);

		house.registerModel( (Model *)p);

		house.run(5);
	}

	void test_invalid_person() {
		map<string, string> configuration;

		House house;
		Person* p = new Person(house, configuration);

		house.registerModel( (Model *)p);

		house.run(5);
	}

	void test_invalid_activation() {
		map<string, string> configuration;
		map<string, string> emptyConfiguration;

		configuration ["filename"] = FILE_NAME;
		configuration ["startState"] = "makeToast";

		ofstream testingFile (FILE_NAME, ios::out);
		testingFile << "makeToast Activate work Toaster 1 3\n";
		testingFile << "work RandomTime dinner 01:15 02:25";
		testingFile.close();

		House house;
		Person* p = new Person(house, configuration);
		Toaster* toaster1 = new Toaster(emptyConfiguration);

		house.registerModel( (Model *)p);
		house.registerModel( (Model *)toaster1);


		TS_ASSERT (p->getState() == "makeToast");

		house.run(2);

		TS_ASSERT (p->getState() == "work");
		TS_ASSERT_EQUALS(toaster1->getEnergy(), 0);

		remove(FILE_NAME);
	}

	void test_deconstructor(){
		map<string, string> configuration;
		House h;
		Person* p = new Person(h, configuration);

		delete(p);

		// Make sure we can reach this point without problems.
		TS_ASSERT(true);

	}

	void test_do_dishes() {
		map<string, string> emptyConfiguration;
		map<string, string> configuration;

		configuration ["filename"] = FILE_NAME;
		configuration ["startState"] = "waitADay";

		map<string, string> dishwasherConfiguration;

		configuration ["waterHeaterName"] = "Waterheater";

		ofstream testingFile (FILE_NAME, ios::out);
		testingFile << "waitADay Delay doDishes 1440\n";
		testingFile << "doDishes Dishes waitADay";
		testingFile.close();


		House house;
		Person* p = new Person(house, configuration);
		Dishwasher* dishwasher1 = new Dishwasher(dishwasherConfiguration, house);
		WaterHeater* waterheater1 = new WaterHeater(emptyConfiguration);

		house.registerModel( (Model *)p);
		house.registerModel( (Model *)dishwasher1);
		house.registerModel( (Model *)waterheater1);

		house.run(1440);

		TS_ASSERT_EQUALS(house.getDaysSinceDishwasherRan(), 1);
		TS_ASSERT_EQUALS(dishwasher1->getPower(), 4)


		house.run(1440);

		TS_ASSERT_EQUALS(house.getDaysSinceDishwasherRan(), 2);
		TS_ASSERT_EQUALS(dishwasher1->getPower(), 4)


		house.run(1440);

		TS_ASSERT_EQUALS(house.getDaysSinceDishwasherRan(), 3);
		TS_ASSERT_EQUALS(dishwasher1->getPower(), 4)

		house.run(10);

		TS_ASSERT_EQUALS(house.getDaysSinceDishwasherRan(), 0);
		TS_ASSERT_EQUALS(dishwasher1->getPower(),2000)

		remove(FILE_NAME);
	}

	void test_get_time(){

		map<string, string> configuration;

		configuration ["filename"] = FILE_NAME;
		configuration ["label"] = "Ada Lovelace";
		configuration ["startState"] = "sleeping";

		ofstream testingFile (FILE_NAME, ios::out);
		testingFile << "sleeping RandomTime waking-up 00:04 00:04\n";
		testingFile << "waking-up RandomTime waking-up 00:15 00:25";
		testingFile.close();

		House house;
		Person* p = new Person(house, configuration);

		house.registerModel( (Model *)p);

		TS_ASSERT_EQUALS (p->getTime(), 0);

		house.run(1);

		TS_ASSERT_EQUALS (p->getTime(), 1);

		remove(FILE_NAME);
	}


};

#endif
