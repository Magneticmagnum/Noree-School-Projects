/**
 * @class Microwave
 * @brief A model of a microwave
 *
 * This class derives from the Model class.  It describes a microwave that
 * consumes 1.5kW-hours when throughout a time period selected from a uniform
 * distribution across 40-120 seconds. While not running, this model consumes
 * no power
 *
 * <p>Microwave can understand the following variables in its model-spec file: </p>
 * <ul><li><b>label</b> (default: Microwave) -- This changes the unique identifying name
 * for the Microwave. Example: <i>label Steve's_Microwave</i></li>
 *
 * <li> <b>state</b> (default: OFF) -- This changes the current state of the model. It can be
 * changed to OFF or ACTIVATED. Example: <i>state ACTIVATED</i>
 *
 * <li> <b>onPower</b> (default: 1500) -- This changes the power used while this
 * model is running. Units are in Watts. Example: <i>onPower 2000</i>
 *
 * <li> <b>offPower</b> (default: 0) -- This changes the power used by this model
 * while it is not running. Units are in Watts. Example: <i>offPower 4</i>
 *
 * </ul>
 */

#ifndef MICROWAVE_H_
#define MICROWAVE_H_

const int MAX_LENGTH_SECONDS = 120;
const int MAX_MIN_DIFFERENCE_SECONDS = 80;
const int SECONDS_IN_MINUTE = 60;

#include <map>
#include "Model.h"

class Microwave: public Model {

public:

	/**
	 * Creates a microwave based on the specified configuration.
	 */
	Microwave(map<string, string> configuration);

	/**
	 * Destructor
	 */
	virtual ~Microwave();

	/**
	 * Will run the microwave for a time between 40 and 120 seconds
	 */
	virtual void activate();

	/**
	 * Will run the microwave for the specified amount of time.
	 */
	virtual void activate(double);

	/**
	 * This will notify the microwave that one tick has passed.
	 * During this time it will update the power.
	 */
	virtual void tick();

protected:
	/**
	 * Attempts to apply the user specified values for this model.  If a
	 * particular variable is not given by the user or is invalid, then the
	 * default value will be used. In addition, if certain specified values
	 * cannot be applied, the user will be notified.
	 */
	virtual void applyConfiguration(map<string, string> configuration);

private:
	Microwave();

};

#endif /* MICROWAVE_H_ */
