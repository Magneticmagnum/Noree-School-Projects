#ifndef TOASTER_H_
#define TOASTER_H_
#include <map>
#include "Model.h"

/**
 * @class Toaster
 * @brief A model of an ordinary toaster.
 *
 * A Toaster is a Model. By default, when activated, it uses 900 W for 2 min.
 *
 * <p>Toaster can understand the following variables in its model-spec file: </p>
 * <ul><li><b>label</b> (default: Toaster) -- This changes the unique identifying name
 * for the Microwave. Example: <i>label Steve's_Microwave</i></li>
 *
 * <li> <b>state</b> (default: OFF) -- This changes the current state of the model. It can be
 * changed to OFF or ACTIVATED. Example: <i>state ACTIVATED</i>
 *
 * <li> <b>onPower</b> (default: 450) -- This changes the power used while this
 * model is running. Units are in Watts. Example: <i>onPower 2000</i>
 *
 * <li> <b>offPower</b> (default: 0) -- This changes the power used by this model
 * while it is not running. Units are in Watts. Example: <i>offPower 4</i>
 *
 * <li> <b>onPeriod</b> (default: 2) -- This changes the length of time it takes
 * for the toaster to convert bread to toast. Units are in minutes.
 * Example: <i>onPeriod 3</i>
 *
 * </ul>
 */

class Toaster: public Model {

public:
	/**
	 * Creates a toaster based on the specified configuration.
	 */
	Toaster(map<string, string> configuration);

	/**
	 * Destructor
	 */
	virtual ~Toaster();

	/**
	 * Notify the Toaster that it has been activated by the user.
	 */
	virtual void activate();

	/**
	 * Notify the toaster one minute has passed, and the toaster should
	 * count the power consumption, write log file according to the field model.
	 */
	virtual void tick();

	/**
	 * Attempts to apply the user specified values for this model.  If a
	 * particular variable is not given by the user or is invalid, then the
	 * default value will be used. In addition, if certain specified values
	 * cannot be applied, the user will be notified.
	 */
	virtual void applyConfiguration(map<string, string> configuration);

protected:

	/// The duration for which the toaster will be on.
	int onPeriod_;

	/// The amount of power that will be drawn when the toaster is on.
	int onPower_;

	/// The amount of power that will be drawn when the toaster is off.
	int offPower_;

private:
	Toaster();
};

#endif /* TOASTER_H_ */
