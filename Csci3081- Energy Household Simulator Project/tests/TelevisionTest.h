#ifndef TOASTERTEST_H_
#define TOASTERRTEST_H_
#include <cxxtest/TestSuite.h>


#include "../libs/Television.h"

class TelevisionTest: public CxxTest::TestSuite {
public:


	///	When Television is off, it consume no power
	void test_Television_configuration_constructor() {

		map<string, string> configuration;

		configuration ["offPower"] = "10";
		configuration ["onPower"] = "100";
		configuration ["size"] = "27";

		Television t = Television(configuration);
		TS_ASSERT_EQUALS(t.getPower(), 10);

		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 10);
		t.activate();

		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 100);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 100);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 100);
		t.activate();
		TS_ASSERT_EQUALS(t.getPower(), 10);
	}

	void test_Television_bad_configuration_constructor() {

		map<string, string> configuration;

		configuration ["offPower"] = "-10";
		configuration ["onPower"] = "-100";
		configuration ["waterDraw"] = "-111";
		configuration ["firstCycleLength"] = "-1";
		configuration ["state"] = "WRONG";
		configuration ["size"] = "-27";

		Television t = Television(configuration);
		TS_ASSERT_EQUALS(t.getPower(), 25);

		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 25);
		t.activate();

		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 200);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 200);
		t.tick();
		TS_ASSERT_EQUALS(t.getPower(), 200);
		t.activate();
		TS_ASSERT_EQUALS(t.getPower(), 25);
	}
};

#endif
