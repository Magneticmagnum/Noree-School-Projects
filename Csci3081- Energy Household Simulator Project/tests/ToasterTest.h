#ifndef TOASTERTEST_H_
#define TOASTERRTEST_H_

#include <cxxtest/TestSuite.h>

#include "../libs/Toaster.h"

class ToasterTest: public CxxTest::TestSuite {
public:

	///	Test the tick() method of Toaster
	///	When Toaster is on, it consumes power*number_of_tick
	///	When Toaster is off, it consume no power
	void test_Toaster_configuration() {

		map<string, string> configuration;

		configuration ["offPower"] = "10";
		configuration ["onPower"] = "100";
		configuration ["onPeriod"] = "3";

		Toaster t = Toaster(configuration);

		TS_ASSERT_EQUALS(t.getPower(), 10);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 10);

		t.activate();
		TS_ASSERT_EQUALS(t.getPower(), 100);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 100);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 100);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 10);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 10);

	}

	void test_Toaster_bad_configuration() {

		map<string, string> configuration;

		configuration ["offPower"] = "-10";
		configuration ["onPower"] = "-100";
		configuration ["onPeriod"] = "-3";

		Toaster t = Toaster(configuration);

		TS_ASSERT_EQUALS(t.getPower(), 0);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 0);

		t.activate();
		TS_ASSERT_EQUALS(t.getPower(), 450);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 450);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 0);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 0);

	}
};

#endif
