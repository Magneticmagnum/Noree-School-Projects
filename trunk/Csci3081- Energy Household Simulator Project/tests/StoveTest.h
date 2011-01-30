#ifndef TOASTERTEST_H_
#define TOASTERRTEST_H_
#include <cxxtest/TestSuite.h>

#include "../libs/Stove.h"
#define RUN_TIME 3

class StoveTest: public CxxTest::TestSuite {
public:

	///	Test the tick() and activate() method of Television
	///	When Television is on, it consumes 180 power per tick.
	///	When Television is off, it consume no power
	void test_Stove_low() {
		map<string, string> configuration;
		Stove t = Stove(configuration);
		TS_ASSERT_EQUALS(t.getPower(), 0);
		TS_ASSERT_EQUALS(t.getEnergy(), 0);

		t.tick();

		TS_ASSERT_EQUALS(t.getPower(), 0);
		TS_ASSERT_EQUALS(t.getEnergy(), 0);

		t.activate(LOW, RUN_TIME);

		for (int i = 0; i < RUN_TIME; i++) {
			t.tick();
			TS_ASSERT_EQUALS(t.getPower(), COST_LOW);
		}

		TS_ASSERT_EQUALS(t.getEnergy(), COST_LOW * RUN_TIME);
	}

	void test_Stove_medium() {
		map<string, string> configuration;
		Stove t = Stove(configuration);
		TS_ASSERT_EQUALS(t.getPower(), 0);

		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 0);

		t.activate(MEDIUM, RUN_TIME);

		for (int i = 0; i < RUN_TIME; i++) {
			t.tick();
			TS_ASSERT_EQUALS(t.getPower(), COST_MEDIUM);
		}

		TS_ASSERT_EQUALS(t.getEnergy(), COST_MEDIUM * RUN_TIME);
	}

	void test_Stove_high() {
		map<string, string> configuration;
		Stove t = Stove(configuration);
		TS_ASSERT_EQUALS(t.getPower(), 0);

		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 0);

		t.activate(HIGH, RUN_TIME);

		for (int i = 0; i < RUN_TIME; i++) {
			t.tick();
			TS_ASSERT_EQUALS(t.getPower(), COST_HIGH);
		}

		TS_ASSERT_EQUALS(t.getEnergy(), COST_HIGH * RUN_TIME);
	}

	void test_Stove_multiple_configuration() {

		map<string, string> configuration;

		configuration ["offPower"] = "10";
		configuration ["lowPower"] = "100";
		configuration ["mediumPower"] = "200";
		configuration ["highPower"] = "300";

		Stove t = Stove(configuration);
		TS_ASSERT_EQUALS(t.getPower(), 40);

		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 40);

		t.activate(HIGH, RUN_TIME);
		t.activate(LOW, RUN_TIME);
		t.activate(MEDIUM, RUN_TIME);
		t.activate(HIGH, RUN_TIME);
		int theirCost = 300 + 100 + 200 + 300;

		for (int i = 0; i < RUN_TIME; i++) {
			t.tick();
			TS_ASSERT_EQUALS(t.getPower(), theirCost);
		}

		t.tick();

		TS_ASSERT_EQUALS(t.getEnergy(), theirCost * RUN_TIME + 80);
		TS_ASSERT_EQUALS(t.getPower(), 40);

		t.activate(HIGH, RUN_TIME);
		t.activate(HIGH, RUN_TIME);
		t.activate(HIGH, RUN_TIME);
		t.activate(HIGH, RUN_TIME);
		t.activate(HIGH, RUN_TIME);

		t.tick();

		TS_ASSERT_EQUALS(t.getPower(), 300 * NUMBER_OF_BURNERS);

	}

	void test_Stove_bad_configuration() {

		map<string, string> configuration;

		configuration ["offPower"] = "-10";
		configuration ["lowPower"] = "-100";
		configuration ["mediumPower"] = "-200";
		configuration ["highPower"] = "-300";
		configuration ["numberOfBurners"] = "-4";

		Stove t = Stove(configuration);
		TS_ASSERT_EQUALS(t.getPower(), 0);

		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 0);

		t.activate(HIGH, RUN_TIME);
		t.activate(LOW, RUN_TIME);
		t.activate(MEDIUM, RUN_TIME);
		t.activate(HIGH, RUN_TIME);
		int theirCost = 1400 + 350 + 700 + 1400;

		for (int i = 0; i < RUN_TIME; i++) {
			t.tick();
			TS_ASSERT_EQUALS(t.getPower(), theirCost);
		}

		t.tick();

		TS_ASSERT_EQUALS(t.getEnergy(), theirCost * RUN_TIME);
		TS_ASSERT_EQUALS(t.getPower(), 0);

		t.activate(HIGH, RUN_TIME);
		t.activate(HIGH, RUN_TIME);
		t.activate(HIGH, RUN_TIME);
		t.activate(HIGH, RUN_TIME);
		t.activate(HIGH, RUN_TIME);

		t.tick();

		TS_ASSERT_EQUALS(t.getPower(), 1400 * NUMBER_OF_BURNERS);

	}

};

#endif
