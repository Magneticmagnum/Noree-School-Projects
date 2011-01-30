#ifndef DISHWASHERTEST_H_
#define DISHWASHERTEST_H_

#include <cxxtest/TestSuite.h>

#include "../libs/Dishwasher.h"

class DishWasherTest: public CxxTest::TestSuite {
public:


	void test_Dishwasher_getPower() {
		map<string, string> configuration;
		House s = House();
		WaterHeater* w = new WaterHeater(configuration);

		map<string, string> dishwasherConfiguration;
		dishwasherConfiguration["waterHeaterName"] = "Waterheater";

		s.registerModel((Model *)w);
		Dishwasher* d = new Dishwasher(dishwasherConfiguration, s);
		s.registerModel((Model *)d);

		int i = 0;

		TS_ASSERT_EQUALS(d->getPower(), 4);
		d->activate();
		TS_ASSERT_EQUALS(d->getPower(), 2000);

		for(i = 0; i < 45; i++){
			TS_ASSERT_EQUALS(d->getPower(), 2000);
			TS_ASSERT_EQUALS(d->getEnergy(), 2000*i);
			d->tick();
		}
		TS_ASSERT_EQUALS(d->getPower(), 4);
		d->tick();
		TS_ASSERT_EQUALS(d->getPower(), 4);
	}

	void test_Dishwasher_config_constructor() {
		map<string, string> emptyConfiguration;
		House s = House();
		WaterHeater* w = new WaterHeater(emptyConfiguration);

		map<string, string> configuration;

		configuration ["offPower"] = "20";
		configuration ["onPower"] = "3000";
		configuration ["state"] = "OFF";

		s.registerModel((Model*)w);
		Dishwasher* d = new Dishwasher(configuration, s);
		s.registerModel((Model*)d);

		int i = 0;

		TS_ASSERT_EQUALS(d->getPower(), 20);
		d->activate();
		TS_ASSERT_EQUALS(d->getPower(), 3000);

		for(i = 0; i < 45; i++){
			TS_ASSERT_EQUALS(d->getPower(), 3000);
			TS_ASSERT_EQUALS(d->getEnergy(), 3000*i);
			d->tick();
		}
		TS_ASSERT_EQUALS(d->getPower(), 20);
		d->tick();
		TS_ASSERT_EQUALS(d->getPower(), 20);
	}

	void test_Dishwasher_config_bad_constructor() {
		map<string, string> emptyConfiguration;
		House s = House();
		WaterHeater* w = new WaterHeater(emptyConfiguration);

		map<string, string> configuration;

		configuration ["offPower"] = "-20";
		configuration ["onPower"] = "-3000";
		configuration ["waterHeaterName"] = "waterHeater";
		configuration ["waterDraw"] = "-50";
		configuration ["firstCycleLength"] = "-30";
		configuration ["onPeriod"] = "-15";
		configuration ["state"] = "ON";

		s.registerModel((Model*)w);
		Dishwasher* d = new Dishwasher(configuration, s);
		s.registerModel((Model*)d);

		int i = 0;

		TS_ASSERT_EQUALS(d->getPower(), 2000);
		d->activate();
		TS_ASSERT_EQUALS(d->getPower(), 2000);

		for(i = 0; i < 45; i++){
			TS_ASSERT_EQUALS(d->getPower(), 2000);
			TS_ASSERT_EQUALS(d->getEnergy(), 2000*i);
			d->tick();
		}
		TS_ASSERT_EQUALS(d->getPower(), 4);
		d->tick();
		TS_ASSERT_EQUALS(d->getPower(), 4);
	}

};

#endif
