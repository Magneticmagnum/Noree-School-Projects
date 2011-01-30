#ifndef COMPUTER_H_
#define COMPUTER_H_

#include "Model.h"

/**
 * @class Computer
 * @brief A model of an ordinary Computer. By default it uses 500W when on and
 * while off it draws 10W of power.
 *
 * <p>Computer can understand the following variables in its model-spec file: </p>
 * <ul><li><b>label</b> (default: Computer) -- This changes the unique identifying name
 * for the Computer. Example: <i>label HP_Pavilion</i></li>
 *
 * <li> <b>state</b> (default: OFF) -- This changes the current state of the model. It can be
 * changed to OFF or ACTIVATED. Example: <i>state ACTIVATED</i>
 *
 * <li> <b>onPower</b> (default: 400) -- This changes the power used while this
 * model is running. Units are in Watts. Example: <i>onPower 800</i>
 *
 * <li> <b>offPower</b> (default: 10) -- This changes the power used by this model
 * while it is not running. Units are in Watts. Example: <i>offPower 40</i>
 *
 * </ul>
 */

class Computer: public Model {

public:
	Computer(map<string, string>);

	/**
	 * Destructor
	 */
	virtual ~Computer();

	/**
	 * Notify the Computer that it has been activated by the user.
	 */
	virtual void activate();

	/**
	 * Notify the Computer one minute has passed, and the Computer should
	 * count the power consumption, write log file according to the field model.
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
	Computer();

};

#endif /* Computer_H_ */
