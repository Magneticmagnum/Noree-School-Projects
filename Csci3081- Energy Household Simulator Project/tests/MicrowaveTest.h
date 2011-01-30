#ifndef MICROWAVETEST_H_
#define MICROWAVETEST_H_

#include <cxxtest/TestSuite.h>

#include "../libs/Microwave.h"
#include "../libs/default_configuration.h"


class MicrowaveTest: public CxxTest::TestSuite {
public:


	void test_Microwave_getPower_simple() {
		map <string, string> configuration;
		Microwave m = Microwave(configuration);

		int i = 0;
		int totalPower = 1500;

		TS_ASSERT_EQUALS(m.getPower(), 0);
		m.activate(1);
		TS_ASSERT(m.getPower() == totalPower);

		m.tick();
		m.tick();

		TS_ASSERT_EQUALS(m.getPower(), 0);

		m.tick();
		TS_ASSERT_EQUALS(m.getPower(), 0);

		TS_ASSERT_EQUALS(m.getEnergy(), totalPower);
	}

	void test_Microwave_getPower_complex1() {
		map <string, string> configuration;
		Microwave m = Microwave(configuration);

		int i = 0;
		int totalPower = 1500;
		double length = 0.75;

		TS_ASSERT_EQUALS(m.getPower(), 0);
		m.activate(length);
		TS_ASSERT(m.getPower() == totalPower*length);

		m.tick();
		m.tick();

		TS_ASSERT_EQUALS(m.getPower(), 0);

		m.tick();
		TS_ASSERT_EQUALS(m.getPower(), 0);

		TS_ASSERT_EQUALS(m.getEnergy(), totalPower*length);
	}

	void test_Microwave_getPower_complex2() {
		map <string, string> configuration;
		Microwave m = Microwave(configuration);

		int i = 0;
		int totalPower = 1500;
		double length = 1.75;

		TS_ASSERT_EQUALS(m.getPower(), 0);
		m.activate(length);
		TS_ASSERT(m.getPower() == (int)(totalPower*length)/2);

		m.tick();
		TS_ASSERT(m.getPower() == (int)(totalPower*length)/2);

		m.tick();

		TS_ASSERT_EQUALS(m.getPower(), 0);

		m.tick();
		TS_ASSERT_EQUALS(m.getPower(), 0);

		TS_ASSERT_DELTA(m.getEnergy(), totalPower*length, 1);
	}

	void test_Microwave_configuration_complex2() {
		map<string, string> configuration;

		configuration ["offPower"] = "3";
		configuration ["onPower"] = "9001";
		configuration ["notARealOption"] = "NONSENSE";
		configuration ["state"] = "OFF";

		Microwave m = Microwave(configuration);
		int i = 0;
		int totalPower = 9001;
		double length = 1.75;

		TS_ASSERT_EQUALS(m.getPower(), 3);
		m.activate(length);
		TS_ASSERT(m.getPower() == (int)(totalPower*length)/2);

		m.tick();
		TS_ASSERT(m.getPower() == (int)(totalPower*length)/2);

		m.tick();

		TS_ASSERT_EQUALS(m.getPower(), 0);

		m.tick();
		TS_ASSERT_EQUALS(m.getPower(), 0);

		for(i = 0; i < 100; i++){
			m.tick();
			m.activate(1);
		}
		TS_ASSERT_EQUALS(m.getPower(),9001);

	}

	void test_Microwave_bad_configuration_complex() {
		map<string, string> configuration;

		configuration ["offPower"] = "-300";
		configuration ["onPower"] = "-100";
		configuration ["notARealOption"] = "Baddddd";
		configuration ["label"] = "bad microwave";

		Microwave m = Microwave(configuration);
		int i = 0;
		int totalPower = 9001;
		double length = 105;

		TS_ASSERT_EQUALS(m.getPower(), 0);
		m.activate(1);
		TS_ASSERT(m.getPower() == 1500);

		m.tick();
		m.tick();

		TS_ASSERT_EQUALS(m.getPower(), 0);

		m.tick();
		TS_ASSERT_EQUALS(m.getPower(), 0);

		TS_ASSERT_EQUALS(m.getEnergy(), 1500);

	}

	void test_Microwave_bad_configuration_complex2() {
		map<string, string> configuration;

		configuration ["offPower"] = "-3";
		configuration ["onPower"] = "-1";
		configuration ["notARealOption"] = "Badddd";
		configuration ["label"] = "bad microwave";
		configuration ["state"] = "ACTIVATED";

		Microwave m = Microwave(configuration);
		int totalPower = -1000;
		double length = -105;

		TS_ASSERT_EQUALS(m.getPower(), 1500);
		m.activate(1);
		TS_ASSERT(m.getPower() == 1500);

		m.tick();
		m.tick();

		TS_ASSERT_EQUALS(m.getPower(), 0);

		m.tick();
		TS_ASSERT_EQUALS(m.getPower(), 0);

		TS_ASSERT_EQUALS(m.getEnergy(), 1500);

	}

	void test_Microwave_within_bounds_complex3() {
		map<string, string> configuration;

		Microwave m = Microwave(configuration);

		TS_ASSERT_EQUALS(m.getPower(), 0);
		m.activate();
		m.tick();
		m.tick();
		m.tick();
		m.tick();
		TS_ASSERT_EQUALS(m.getPower(), 0);		

	}

};

#endif
