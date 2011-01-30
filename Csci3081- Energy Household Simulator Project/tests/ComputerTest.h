#ifndef TOASTERTEST_H_
#define TOASTERRTEST_H_
#include <cxxtest/TestSuite.h>


#include "../libs/Computer.h"

class ComputerTest: public CxxTest::TestSuite {
public:

	///	Test the tick() and activate() method of Computer
	///	When Computer is on, it consumes 500 watts per tick.
	///	When Computer is off, it consumes 10 watts
	void test_Computer_simple_constructor_tick() {

		map<string, string> configuration;
		Computer t = Computer(configuration);
		TS_ASSERT_EQUALS(t.getPower(), 100);

		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 100);
		t.activate();
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 500);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 500);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 500);
		t.activate();
		TS_ASSERT_EQUALS(t.getPower(), 100);
	}

	void test_Computer_map_constructor_tick() {

		map<string, string> configuration;

		configuration ["offPower"] = "3";

		Computer t = Computer(configuration);
		TS_ASSERT_EQUALS(t.getPower(), 3);

		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 3);
		t.activate();
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 500);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 500);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 500);
		t.activate();
		TS_ASSERT_EQUALS(t.getPower(), 3);
	}

	void test_Computer_map_constructor_complex() {

		map<string, string> configuration;

		configuration ["offPower"] = "3";
		configuration ["onPower"] = "9001";
		configuration ["notARealOption"] = "NONSENSE";

		Computer t = Computer(configuration);
		TS_ASSERT_EQUALS(t.getPower(), 3);

		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 3);
		t.activate();
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 9001);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 9001);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 9001);
		t.activate();
		TS_ASSERT_EQUALS(t.getPower(), 3);
	}

	void test_Computer_bad_configuration_complex() {

		map<string, string> configuration;

		configuration ["offPower"] = "-90";
		configuration ["onPower"] = "-9001";
		configuration ["notARealOption"] = "NONSENSE";
		configuration ["label"] = "HAL";
		configuration ["state"] = "OFF";

		Computer t = Computer(configuration);
		TS_ASSERT_EQUALS(t.getPower(), 100);

		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 100);
		t.activate();
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 500);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 500);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 500);
		t.activate();
		TS_ASSERT_EQUALS(t.getPower(), 100);
	}

	void test_Computer_bad_configuration_complex2() {

		map<string, string> configuration;

		configuration ["offPower"] = "-90";
		configuration ["onPower"] = "-9001";
		configuration ["notARealOption"] = "NONSENSE";
		configuration ["label"] = "HAL";
		configuration ["state"] = "ACTIVATED";

		Computer t = Computer(configuration);
		TS_ASSERT_EQUALS(t.getPower(), 500);

		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 500);
		t.activate();
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 100);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 100);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 100);
		t.activate();
		TS_ASSERT_EQUALS(t.getPower(), 500);
	}

};

#endif
