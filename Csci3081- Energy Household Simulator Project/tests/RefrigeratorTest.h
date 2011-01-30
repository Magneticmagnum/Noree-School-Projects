#ifndef REFRIGERATORTEST_H_
#define REFRIGERATORTEST_H_

#include <cxxtest/TestSuite.h>

#include "../libs/Refrigerator.h"

class RefrigeratorTest: public CxxTest::TestSuite {
public:

	void test_Refrigerator_Config_Constructor_simple(){
		int power = 1000;

		map<string, string> configuration;
		configuration ["onPeriod"] = "3";
		configuration ["dutyCycle"] = "0.3";
		configuration ["onPower"] = "1000";
		configuration ["totalCost"] = "0";
		configuration ["timeCounter"] = "0";
		configuration ["state"] = "ON";
		configuration ["timeCounter"] = "0";
		configuration ["label"] = "Refrigerator";

		Refrigerator f(configuration);

		TS_ASSERT_EQUALS(f.getPower(), power);
		f.tick();
		f.tick();
		f.tick();
		f.tick();
		TS_ASSERT_EQUALS(f.getPower(), 0);
		for (int i = 0; i < 7; i++)
			f.tick();
		TS_ASSERT_EQUALS(f.getPower(), power);

		f.activate();

		TS_ASSERT_EQUALS(f.getPower(), power);

	}

	void test_Refrigerator_activate() {
		map<string, string> configuration;
		configuration ["state"] = "ON";
		configuration ["timeCounter"] = "0";
		configuration ["onPower"] = "200";

		Refrigerator f = Refrigerator(configuration);
		Refrigerator r = Refrigerator(configuration);

		f.activate();
		f.activate();
		f.activate();
		TS_ASSERT_EQUALS(f.getPower(), 200);
		f.tick();
		TS_ASSERT_EQUALS(f.getPower(), 200);
		f.tick();
		TS_ASSERT_EQUALS(f.getPower(), 200);
		f.tick();
		TS_ASSERT_EQUALS(f.getPower(), 200);
		f.tick();
		TS_ASSERT_EQUALS(f.getPower(), 0);
		f.tick();
		TS_ASSERT_EQUALS(f.getPower(), 0);


		while(r.isRunning()){
			r.tick();
		}
		r.activate();
		r.activate();
		r.activate();
		TS_ASSERT_EQUALS(r.getPower(), 0);
		r.tick();
		TS_ASSERT_EQUALS(r.getPower(), 0);
		r.tick();
		TS_ASSERT_EQUALS(r.getPower(), 0);
		r.tick();
		TS_ASSERT_EQUALS(r.getPower(), 0);
		r.tick();
		TS_ASSERT_EQUALS(r.getPower(), 0);
	}

	void test_Refrigerator_specify() {
		int power = 300;

		map<string, string> configuration;

		configuration ["onPeriod"] = "7";
		configuration ["onPower"] = "300";
		configuration ["offPower"] = "0";
		configuration ["totalCost"] = "0";
		configuration ["state"] = "OFF";
		configuration ["timeCounter"] = "1";
		configuration ["label"] = "Refrigerator";

		Refrigerator f = Refrigerator(configuration);
		TS_ASSERT(!f.isRunning());

		f.tick();
		f.tick();
		f.tick();

		//TS_ASSERT_EQUALS(f.getPower(), 0);
		for (int i = 0; i < 7; i++)
			f.tick();
		//TS_ASSERT_EQUALS(f.getPower(), 300);

		f.activate();

		//TS_ASSERT_EQUALS(f.getPower(), 300);
	}

	void test_Refrigerator_Config_Constructor_bad(){

		map<string, string> configuration;
		configuration ["onPeriod"] = "-3";
		configuration ["dutyCycle"] = "-0.3";
		configuration ["inactivePeriod"] = "-10";
		configuration ["inactiveOnPeriod"] = "-3";
		configuration ["activeOnPeriod"] = "-3";
		configuration ["activeDutyCycle"] = "-1";
		configuration ["inactiveDutyCycle"] = "-1";
		configuration ["onPower"] = "-1000";
		configuration ["offPower"] = "-1";
		configuration ["totalCost"] = "-50";
		configuration ["timeCounter"] = "-10";
		configuration ["state"] = "ON";
		configuration ["timeCounter"] = "-10";
		configuration ["label"] = "Refrigerator";

		Refrigerator f(configuration);

		TS_ASSERT_EQUALS(f.getPower(), 300);
		f.tick();
		f.tick();
		f.tick();
		f.tick();
		TS_ASSERT_EQUALS(f.getPower(), 300);
		for (int i = 0; i < 7; i++)
			f.tick();
		TS_ASSERT_EQUALS(f.getPower(), 300);

		f.activate();

		TS_ASSERT_EQUALS(f.getPower(), 300);
		f.tick();
		TS_ASSERT_EQUALS(f.getPower(), 300);
	}


	void test_Refrigerator_no_state(){

		map<string, string> configuration;

		Refrigerator f(configuration);

		if(f.isRunning()){
			TS_ASSERT_EQUALS(f.getPower(), 300);
		}

		if(!f.isRunning()){
			TS_ASSERT_EQUALS(f.getPower(), 0);
		}
	}

};

#endif
