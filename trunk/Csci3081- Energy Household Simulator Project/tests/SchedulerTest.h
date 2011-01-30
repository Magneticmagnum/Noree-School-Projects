#ifndef SCHEDULERTEST_H_
#define SCHEDULERTEST_H_

#define FILE_NAME "testScript"

#include <cxxtest/TestSuite.h>

#include "../libs/Refrigerator.h"
#include "../libs/Oven.h"
#include "../libs/WaterHeater.h"
#include "../libs/Toaster.h"
#include "../libs/Microwave.h"
#include "../libs/Dishwasher.h"
#include "../libs/Television.h"
#include "../libs/Model.h"
#include "../libs/Scheduler.h"
#include "../libs/Stove.h"
#include "../libs/Person.h"

class SchedulerTest: public CxxTest::TestSuite {
public:

	///	Test the getTime() method of Scheduler
	/// Should return the time that have passed since started
	void test_Scheduler_getTime() {
		Scheduler s = Scheduler();
		s.setTime(100);
		TS_ASSERT_EQUALS(s.getTime(), 100);
	}

	void test_full_phase_2_runthrough(){
		Scheduler schedule;
		int days = 10;
		schedule.registerHouse("tests/phase2_model_list_testing");

		schedule.run(1440*days);

		TS_ASSERT(true);

	}

};

#endif /* SCHEDULERTEST_H_ */
