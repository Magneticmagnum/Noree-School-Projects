#ifndef OVENTEST_H_
#define OVENTEST_H_

#include <cxxtest/TestSuite.h>

#include "../libs/Oven.h"
#include <map>

class OvenTest: public CxxTest::TestSuite {
public:

	void test_Oven_getPower() {
		map <string, string> configuration;
		Oven d(configuration);
		int i = 0;

		TS_ASSERT_EQUALS(d.getEnergy(), 0);

		TS_ASSERT_EQUALS(d.getPower(), 0);
		d.activate(40);
		TS_ASSERT_EQUALS(d.getPower(), 7500);

		for (i = 0; i < 10; i++) {
			TS_ASSERT_EQUALS(d.getPower(), 7500);
			d.tick();
		}
		for (i = 0; i < 40; i++) {
			TS_ASSERT_EQUALS(d.getPower(), 2500);
			d.tick();
		}
		TS_ASSERT_EQUALS(d.getPower(), 0);
		d.tick();
		TS_ASSERT_EQUALS(d.getPower(), 0);

	}

	void test_Oven_configuration_constructor() {

		map<string, string> configuration;

		configuration ["preHeatPower"] = "3000";
		configuration ["cookPower"] = "500";
		configuration ["preHeatTime"] = "5";

		Oven d(configuration);
		int i = 0;

		TS_ASSERT_EQUALS(d.getEnergy(), 0);

		TS_ASSERT_EQUALS(d.getPower(), 0);
		d.activate(40);
		TS_ASSERT_EQUALS(d.getPower(), 3000);

		for (i = 0; i < 5; i++) {
			d.tick();
		}
		for (i = 0; i < 40; i++) {
			d.tick();
		}
		TS_ASSERT_EQUALS(d.getPower(),0);
		d.tick();
		TS_ASSERT_EQUALS(d.getPower(), 0);
		d.tick();
		TS_ASSERT_EQUALS(d.getPower(), 0);
		d.tick();
		TS_ASSERT_EQUALS(d.getPower(), 0);

	}

	void test_Oven_configuration_constructor2() {

		map<string, string> configuration;

		configuration ["preHeatPower"] = "3000";
		configuration ["cookPower"] = "500";
		configuration ["preHeatTime"] = "5";
		configuration ["state"] = "OFF";

		Oven d(configuration);
		int i = 0;

		TS_ASSERT_EQUALS(d.getEnergy(), 0);

		TS_ASSERT_EQUALS(d.getPower(), 0);
		d.activate(40);
		TS_ASSERT_EQUALS(d.getPower(), 3000);

		for (i = 0; i < 5; i++) {
			TS_ASSERT_EQUALS(d.getPower(), 3000);
			d.tick();
		}
		for (i = 0; i < 40; i++) {
			TS_ASSERT_EQUALS(d.getPower(),500);
			d.tick();
		}
		TS_ASSERT_EQUALS(d.getPower(), 0);
		d.tick();
		TS_ASSERT_EQUALS(d.getPower(), 0);

	}

	void test_Oven_configuration_bad_constructor() {

		map<string, string> configuration;

		configuration ["preHeatPower"] = "-10";
		configuration ["cookPower"] = "-10";
		configuration ["preHeatTime"] = "-10";
		configuration ["offPower"] = "-10";

		Oven d(configuration);
		int i = 0;

		TS_ASSERT_EQUALS(d.getEnergy(), 0);

		TS_ASSERT_EQUALS(d.getPower(), 0);
		d.activate(40);

		for (i = 0; i < 10; i++) {
			TS_ASSERT_EQUALS(d.getPower(), 7500);
			d.tick();
		}
		for (i = 0; i < 40; i++) {
			TS_ASSERT_EQUALS(d.getPower(),2500);
			d.tick();
		}
		TS_ASSERT_EQUALS(d.getPower(), 0);
		d.tick();
		TS_ASSERT_EQUALS(d.getPower(), 0);

	}
};

#endif
