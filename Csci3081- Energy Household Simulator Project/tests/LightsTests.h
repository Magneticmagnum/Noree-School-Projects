#ifndef LIGHTSTEST_H_
#define LIGHTSTEST_H_
#include <cxxtest/TestSuite.h>
#include "../libs/Lights.h"
#include "../libs/default_configuration.h"

class LightsTest: public CxxTest::TestSuite {
public:

	///	Test the tick() method of Lights
	///	When Lights is on, it consumes power*number_of_tick
	///	When Lights is off, it consume no power
	void test_Lights_tick() {
		map <string, string> configuration;
		Lights t = Lights(configuration);

		TS_ASSERT_EQUALS(t.getPower(), 0);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 0);

		LIGHTS_ON_POWER;

		t.activate(50, ON);
		TS_ASSERT_EQUALS(t.getPower(), 45 * LIGHTS_ON_POWER);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 45 * LIGHTS_ON_POWER);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 45 * LIGHTS_ON_POWER);
		t.activate(30, OFF);
		TS_ASSERT_EQUALS(t.getPower(), 15 * LIGHTS_ON_POWER);
		t.activate(20, OFF);
		TS_ASSERT_EQUALS(t.getPower(), 0);

		t.activate(101, ON);
		TS_ASSERT_EQUALS(t.getPower(), 45 * LIGHTS_ON_POWER);
		t.activate(101, OFF);
		TS_ASSERT_EQUALS(t.getPower(), 0);

	}

	void test_Lights_configuration() {

		map<string, string> configuration;

		configuration ["offPower"] = "0";
		configuration ["onPower"] = "30";
		configuration ["label"] = "outdoorLights";
		configuration ["activatedLights"] = "0";

		Lights t = Lights(configuration);

		TS_ASSERT_EQUALS(t.getPower(), 0);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 0);

		t.activate(45, ON);
		TS_ASSERT_EQUALS(t.getPower(), 30 * 45);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 30 * 45);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 30 * 45);
		t.activate(30, OFF);
		TS_ASSERT_EQUALS(t.getPower(), 30 * 15);
		t.activate(20, OFF);
		TS_ASSERT_EQUALS(t.getPower(), 0);

	}

	void test_Lights_bad_configuration() {

		map<string, string> configuration;

		configuration ["offPower"] = "0";
		configuration ["onPower"] = "-30";
		configuration ["label"] = "Bad Lights";
		configuration ["activatedLights"] = "-1";

		Lights t = Lights(configuration);

		TS_ASSERT_EQUALS(t.getPower(), 0);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 0);

		t.activate(45, ON);
		TS_ASSERT_EQUALS(t.getPower(), 1 * 45);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 1 * 45);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 1 * 45);
		t.activate(30, OFF);
		TS_ASSERT_EQUALS(t.getPower(), 1 * 15);
		t.activate(20, OFF);
		TS_ASSERT_EQUALS(t.getPower(), 0);

	}
};

#endif /*LIGHTSTEST.H*/
