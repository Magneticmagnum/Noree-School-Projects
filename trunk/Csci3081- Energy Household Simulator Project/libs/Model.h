#ifndef MODEL_H_
#define MODEL_H_

#include <string>

using namespace std;

const int OFF = 0;
const int ON = 1;
const int ACTIVATED = 2;
const int ACTIVATED_OFF = 3;
const int LOW = 1;
const int MEDIUM = 2;
const int HIGH = 3;

/// @class Model
/// @brief A base class for all the devices' modeling
///
/// Model is a class that describes the basic characteristics of devices
/// A class must inherit from Model to be a proper device

class Model {

public:

	/**
	 * Default constructor
	 */
	Model();

	/**
	 * Destructor
	 */
	virtual ~Model();

	/**
	 * Notifies the model that one tick of time has passed since the last call to tick().
	 * Subclasses must implement this function to create their behaviors.
	 */
	virtual void tick();

	/**
	 * Notifies the model that it has been activated this tick.
	 * Subclasses must implement this function to create their behaviors.
	 */
	virtual void activate();

	/**
	 * The virtual function used by the WaterHeater to get water
	 */
	virtual void activate(double, double);

	/**
	 * The virtual functin used by the Stove to heat burners.
	 */
	virtual void activate(int, int);

	/**
	 * The virtual function used by Microwave for testing.
	 */
	virtual void activate(double);

	/**
	 * Returns the instantaneous power usage of this Model.
	 */
	virtual int getPower();

	/**
	 * Returns the total energy usage of this Model since it was created.
	 */
	virtual int getEnergy();

	/**
	 * Returns the name of the type of the Model
	 */
	virtual string getLabel();

protected:

	/// The power this device is drawing at this moment
	int power_;

	/// The total power spend since this device is created in the scenario
	int totalCost_;

	///The time period that this device is on in one cycle
	int onPeriod_;

	///	The time period that this device is off in one cycle
	int offPeriod_;

	/// The amount of power used when the model is on.
	int onPower_;

	/// The amount of power used when the mode l is off.
	int offPower_;

	/// Measures the amount of time that the cycle has been run for so far.
	int timeCounter_;

	/// A string that indicates the name of the model.
	string label_;

	/// A string that indicates the type of the model (e.g. toaster, lights, etc.)
	string type_;

	/// An indicator of the current state the model is  in (ON, OFF, etc.)
	int state_;

};

#endif /* MODEL_H_ */
