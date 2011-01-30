/*
 * @class Scheduler
 * @brief The simulation's time keeper.
 * Keeps track of a house and notifies it when a tick has gone by.
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "Model.h"
#include "../libs/WaterHeater.h"
#include "../libs/House.h"
#include <vector>
#include <map>
#include <string>

using namespace std;

class Scheduler {

	friend class SchedulerTest; /// Needed for testing.

public:

	/// Default constructor
	Scheduler();

	/// Deconstructor
	virtual ~Scheduler();

	////Register a model to a list that will be notified of ticks.
	bool registerHouse(char* modelSpecFile);

	/// Returns the number of ticks passed since the Scheduler was started.
	static int getTime();

	///	Setup the number of ticks that have passed
	static void setTime(int time);

	/**
	 * Start scheduler in a loop for Input time, during the loop,
	 *	the scheduler will call the tick of every registered model every minute.
	 */
	void run(int);


protected:

	///	The number of ticks that have passed
	static int totalTime_;

	///	The house to be notified of time's passage.
	House* house_;

};

#endif /* SCHEDULER_H_ */
