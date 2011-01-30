#ifndef WATERHEATERTEST_H_
#define WATERHEATERTEST_H_

#include <cxxtest/TestSuite.h>


#include "../libs/WaterHeater.h"

#define ACCURACY 0.0001

class WaterHeaterTest: public CxxTest::TestSuite {
public:

	///	Ensures that the water loses temperature at the proper rate.
	void test_WaterHeater_temperature() {
		map <string, string> configuration;
		WaterHeater t  = WaterHeater(configuration);

		int i;
		double projectedTemperature = 50;

 		TS_ASSERT_EQUALS(t.getTemperature(), projectedTemperature);
		t.tick();
		projectedTemperature -= 0.0029;
		TS_ASSERT_EQUALS(t.getTemperature(), projectedTemperature);

		for(i = 0; i < 300; i++){
			t.tick();
			projectedTemperature -= 0.0029;
		}

		TS_ASSERT_EQUALS(t.getTemperature(), projectedTemperature);

		for(i = 0; i < 44; i++){
			t.tick();
			projectedTemperature -= 0.0029;
		}

		TS_ASSERT_EQUALS(t.getTemperature(), projectedTemperature);
	}

	///	Ensures that the water consumes power at the proper times.
	void test_WaterHeater_power() {
		map <string, string> configuration;
		WaterHeater t  = WaterHeater(configuration);

		configuration ["offPower"] = "0";
		configuration ["onPower"] = "3500";
		configuration ["maxLiters"] = "150";

		WaterHeater tMake  = WaterHeater(configuration);

		int i;

 		TS_ASSERT_EQUALS(t.getTemperature(), 50);
 		TS_ASSERT_EQUALS(t.getPower(), 0);

		for(i = 0; i < 346; i++){
			t.tick();
			tMake.tick();
		}

		TS_ASSERT_DELTA(t.getTemperature(), 49.3216, ACCURACY);
		TS_ASSERT_EQUALS(t.getPower(), 4500);

		TS_ASSERT_DELTA(tMake.getTemperature(), 49.3299, ACCURACY);

		for(i = 0; i < 5; i++)
			t.tick();

		TS_ASSERT_EQUALS(t.getPower(), 0);
		TS_ASSERT_DELTA(t.getTemperature(), 50.2821, ACCURACY);

	}

	///	tests the activate function of the water heater
	void test_WaterHeater_getWater() {
		map <string, string> configuration;
		WaterHeater t  = WaterHeater(configuration);

 		TS_ASSERT_EQUALS(t.getTemperature(), 50);

 		t.activate(8, 40);

 		TS_ASSERT_DELTA(t.getTemperature(), 48.6676, ACCURACY);
 		TS_ASSERT_DELTA(t.getLiters(), 198.1395, ACCURACY);

	}

	void test_WaterHeater_bad_config() {
		map <string, string> configuration;
		WaterHeater t  = WaterHeater(configuration);

		configuration ["offPower"] = "-10";
		configuration ["onPower"] = "-3500";
		configuration ["maxLiters"] = "-150";
		configuration ["minTemperature"] = "45";
		configuration ["maxTemperature"] = "-80";

		WaterHeater tMake  = WaterHeater(configuration);

		int i;

 		TS_ASSERT_EQUALS(t.getTemperature(), 50);
 		TS_ASSERT_EQUALS(t.getPower(), 0);

		for(i = 0; i < 346; i++){
			t.tick();
			tMake.tick();
		}

		TS_ASSERT_DELTA(t.getTemperature(), 49.3216, ACCURACY);
		TS_ASSERT_EQUALS(t.getPower(), 4500);

		TS_ASSERT_DELTA(tMake.getTemperature(), 48.9966, ACCURACY);

		for(i = 0; i < 5; i++)
			t.tick();

		TS_ASSERT_EQUALS(t.getPower(), 0);
		TS_ASSERT_DELTA(t.getTemperature(), 50.2821, ACCURACY);

	}


};

#endif
