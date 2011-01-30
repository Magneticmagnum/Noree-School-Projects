#ifndef SHOWERTEST_H_
#define SHOWERTEST_H_

#include <cxxtest/TestSuite.h>

#include "../libs/Shower.h"

class ShowerTest: public CxxTest::TestSuite {
public:

	void test_Shower_config() {
		map<string, string> emptyConfiguration;
		map<string, string> configuration;
		configuration ["waterDraw"] = "7";
		configuration ["tempDraw"] = "40";
		configuration ["waterHeaterName"] = "Waterheater";
		configuration ["label"] = "Shower";

		House s = House();

		WaterHeater* w = new WaterHeater(emptyConfiguration);
		s.registerModel(w);

		Shower* d = new Shower(configuration, s);
		s.registerModel(d);

		int i = 0;
		double beforeTemp = w->getTemperature();
		d->activate();
		d->tick();
		double afterTemp = w->getTemperature();

		TS_ASSERT(beforeTemp > afterTemp);

	}
	void test_Shower_bad_config() {
		map<string, string> emptyConfiguration;
		map<string, string> configuration;
		configuration ["waterDraw"] = "-7";
		configuration ["tempDraw"] = "-40";
		configuration ["label"] = "Shower";
		configuration ["state"] = "OFF";

		House s = House();

		WaterHeater* w = new WaterHeater(emptyConfiguration);
		s.registerModel(w);

		Shower* d = new Shower(configuration, s);
		s.registerModel(d);

		int i = 0;
		double beforeTemp = w->getTemperature();
		d->activate();
	}

};

#endif /*SHOWERTEST->H*/
