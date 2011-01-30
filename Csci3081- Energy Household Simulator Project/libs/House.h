#ifndef HOUSE_H_
#define HOUSE_H_

#include "Model.h"
#include "../libs/House.h"
#include "../libs/Refrigerator.h"
#include "../libs/Oven.h"
#include "../libs/WaterHeater.h"
#include "../libs/Toaster.h"
#include "../libs/Microwave.h"
#include "../libs/Dishwasher.h"
#include "../libs/Lights.h"
#include "../libs/Television.h"
#include "../libs/Computer.h"
#include "../libs/Scheduler.h"
#include "../libs/Stove.h"
#include "../libs/Shower.h"
#include "../libs/Person.h"

#include <vector>
#include <map>
#include <string>

using namespace std;

///	@class House
///	@brief A manager of all the devices working according to the time.
///	Devices need to register to be noticed by the tick()
///	The scheduler will go through the list of device and call their tick in sequence.


class House {

	friend class HouseTest;

public:

	House();

	/**
	 * Default constructor
	 */
	House(char* filePath);

	/**
	 * Destructor
	 */
	virtual ~House();

	/**
	 * tick tick
	 */

	void tick();


	/**
	 * Returns the number of ticks
	 * that have passed since the Scheduler was started.
	 */
	static int getTime();

	///	Setup the number of ticks that have passed
	static void setTime(int time);

	/**
	 * Returns the number of devices created in simulator
	 */
	int getNumberDevices();

	/**
	 * Gets amount of water at desired temperature from WaterHeater
	 */
	void getWater(double amount, double temperature);

	/**
	 * Gets the total amount of power being drawn by all models at the
	 * moment this function is called.
	 */
	int getTotalPower();

	/**
	 * Gets the total amount of energy used up through this point in time
	 */
	int getTotalEnergy();

	/// Returns the number of days that passed since dishwasher last ran.
	int getDaysSinceDishwasherRan();

	/**
	 * Will run the house (and all of its contained models) for the specified
	 * number of ticks.  Used for testing only.
	 */
	void run(int times);

	/// Indicates the dishwasher ran.  Resets daysSinceDishwasherRan to zero.
	void dishwasherRan();

	/// Runs 'Delete' on all of the models contained within house.
	void cleanup();

	/**
	 * Activates the model with the given name using the args specified in the
	 * vector.
	 */

	/// Activates a two argument model with two doubles.
	void activateModel(string modelName, double, double);

	/// Activates a one argument model.
	void activateModel(string modelName, double);

	/// Activates a zero-argument moded
	void activateModel(string modelName);

	/**
	 * Register a model to a list that will be notified of ticks.
	 */
	bool registerModel(Model*);


protected:

	///	The number of ticks that have passed
	static int totalTime_;

	///	The number of models in the list
	int numDevices_;

	///	The list of registered models
	map<string, Model*> models_;

	/// The number of days that have passed since the dishwasher has run.
	int daysSinceDishwasherRan_;

	/**
	 * Given a line of the correct format, will generate a map between the
	 * pairs of variables and values.
	 */
	map<string, string> generateConfig(vector<string> vectored);

	/**
	 * Given a model configuration, will create the model and add it to the list
	 * of models of that house.
	 */
	void createModel(map<string, string> configuration);

};

#endif /* HOUSE_H_ */
