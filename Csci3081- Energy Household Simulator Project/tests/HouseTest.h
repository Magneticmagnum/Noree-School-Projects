#ifndef HOUSETEST_H_
#define HOUSETEST_H_

#define FILE_NAME "testScript"

#include <cxxtest/TestSuite.h>

#include "../libs/House.h"
#include "../libs/Refrigerator.h"
#include "../libs/Oven.h"
#include "../libs/WaterHeater.h"
#include "../libs/Toaster.h"
#include "../libs/Microwave.h"
#include "../libs/Dishwasher.h"
#include "../libs/Television.h"
#include "../libs/Model.h"
#include "../libs/Scheduler.h"
#include "../libs/Stove.h"
#include "../libs/Person.h"

class HouseTest: public CxxTest::TestSuite {
public:

	void test_simple_parse(){

		House house;
		ofstream testingFile (FILE_NAME, ios::out);
		testingFile << "type Toaster\n";
		testingFile.close();

		House house1 (FILE_NAME);

		TS_ASSERT_EQUALS(house1.models_.size(), 1);
		TS_ASSERT_EQUALS(house1.getNumberDevices(), 1);

		remove(FILE_NAME);

	}


	void test_comment_parse(){

		House house;
		ofstream testingFile (FILE_NAME, ios::out);
		testingFile << "#type Computer\n";
		testingFile << "type Toaster\n";
		testingFile.close();

		House house1 (FILE_NAME);

		TS_ASSERT_EQUALS(house1.models_.size(), 1);
		TS_ASSERT_EQUALS(house1.getNumberDevices(), 1);

		remove(FILE_NAME);

	}

	void test_invalid_line(){

		House house;
		ofstream testingFile (FILE_NAME, ios::out);
		testingFile << "type Computer";
		testingFile << "type Toaster\n";
		testingFile.close();

		House house1 (FILE_NAME);

		TS_ASSERT_EQUALS(house1.models_.size(), 0);
		TS_ASSERT_EQUALS(house1.getNumberDevices(), 0);

		remove(FILE_NAME);

	}

	void test_invalid_model_activate(){

		House house;
		ofstream testingFile (FILE_NAME, ios::out);
		testingFile << "#type Computer";
		testingFile.close();

		House house1 (FILE_NAME);

		house1.activateModel("Computer");

		TS_ASSERT(true);
		remove(FILE_NAME);

	}

	void test_tick(){

		House house;
		ofstream testingFile (FILE_NAME, ios::out);
		testingFile << "type Toaster ";
		testingFile << "label BraveLittle ";
		testingFile << "onPower 100 ";
		testingFile << "offPower  10\n";
		testingFile.close();

		House house1 (FILE_NAME);
		house1.tick();

		TS_ASSERT_EQUALS(house1.getNumberDevices(), 1);

		house1.activateModel("BraveLittle");
		house1.models_["BraveLittle"]->tick();
		TS_ASSERT_EQUALS(house1.getTotalPower(), 100);

		remove(FILE_NAME);

	}

	void test_all_models(){

		ofstream testingFile (FILE_NAME, ios::out);
		testingFile << "type Computer\n";
		testingFile << "type Toaster\n";
		testingFile << "type Dishwasher\n";
		testingFile << "type Lights\n";
		testingFile << "type Oven\n";
		testingFile << "type Refrigerator ";
		testingFile << "state ON ";
		testingFile << "timeCounter 1\n";
		testingFile << "type Shower\n";
		testingFile << "type Stove\n";
		testingFile << "type Television\n";
		testingFile << "type WaterHeater\n";
		testingFile << "type Person\n";
		testingFile.close();

		House house1 (FILE_NAME);

		TS_ASSERT_EQUALS(house1.models_.size(), 11);
		TS_ASSERT_EQUALS(house1.getNumberDevices(), 11);

		TS_ASSERT_EQUALS(house1.getTotalEnergy(), 0);
		house1.tick();
		TS_ASSERT_EQUALS(house1.getTotalEnergy(), 429);
		TS_ASSERT_EQUALS(house1.getTotalPower(), 429);
		remove(FILE_NAME);

	}


	void test_advanced_interaction(){

		House house;
		ofstream testingFile (FILE_NAME, ios::out);
		testingFile << "type Dishwasher ";
		testingFile << "label Dishy ";
		testingFile << "waterHeaterName Walter ";
		testingFile << "firstCycleLength 1 ";
		testingFile << "waterDraw 200\n";
		testingFile << "type Waterheater ";
		testingFile << "label Walter\n";
		testingFile.close();

		House house1 (FILE_NAME);

		TS_ASSERT_EQUALS(house1.getNumberDevices(), 2);
		WaterHeater* walter = (WaterHeater*) house1.models_["Walter"];
		TS_ASSERT_EQUALS(walter->getTemperature(), 50);
		house1.models_["Dishy"]->activate();
		house1.models_["Dishy"]->tick();
		TS_ASSERT_EQUALS(walter->getTemperature(), 7);
		remove(FILE_NAME);

	}

	void test_advanced_bad_interaction(){

		House house;
		ofstream testingFile (FILE_NAME, ios::out);
		testingFile << "type Dishwasher ";
		testingFile << "label Dishy ";
		testingFile << "waterHeaterName Walter ";
		testingFile << "firstCycleLength 1 ";
		testingFile << "waterDraw 200\n";
		testingFile << "type Waterheater ";
		testingFile << "label Walter\n";
		testingFile.close();

		House house1 (FILE_NAME);

		TS_ASSERT_EQUALS(house1.getNumberDevices(), 2);
		WaterHeater* walter = (WaterHeater*) house1.models_["Walter"];
		TS_ASSERT_EQUALS(walter->getTemperature(), 50);
		house1.models_["Dishy"]->activate();
		house1.models_["Dishy"]->tick();
		TS_ASSERT_EQUALS(walter->getTemperature(), 7);
		remove(FILE_NAME);

	}

};

#endif /* SCHEDULERTEST_H_ */
