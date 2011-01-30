/**
 * @class Refrigerator
 * @brief A model of an ordinary refrigerator.
 *
 * A Refrigerator is a Model. It is always either running and off. When
 * undisturbed (nobody cooking), it has a duty cycle (on/off ratio) of 0.3.
 * When a Person is cooking, it has a duty cycle of 0.5. Each on cycle is 3
 * minutes. When it is on, it shall consume 300 watts. When a Refrigerator
 * starts up, it shall be at a random point in its cycle.
 *
 * <p>Refrigerator can understand the following variables in its model-spec file: </p>
 * <ul><li><b>label</b> (default: Refrigerator) -- This changes the unique identifying name
 * for the Refrigerator. Example: <i>label Refrigerator</i></li>
 *
 * <li> <b>state</b> (default: OFF) -- This changes the current state of the model.
 * It can be changed to OFF, ON, ACTIVATED_OFF, or ACTIVATED_ON.
 * Example: <i>state ON</i>
 *
 * <li> <b>onPeriod</b> (default: none) -- This special variable changes both
 * the active onPeriod and inactive onPeriod of the refrigerator.
 * Example: <i>onPeriod 3</i>
 *
 * <li> <b>inactiveOnPeriod</b> (default: 3) -- This changes the number of
 * ticks per ON cycle when the refrigerator is not in its cooking state.
 * Units are in ticks. Example: <i>inactiveOnPeriod 4</i>
 *
 * <li> <b>activeOnPeriod</b> (default: 3) -- This changes the number of
 * ticks per ON cycle when the refrigerator is in its cooking state.
 * Units are in ticks. Example: <i>activeOnPeriod 4</i>
 *
 * <li> <b>activeDutyCycle</b> (default: 0.5) -- This changes the ratio of
 * on ticks to total ticks when the refrigerator is being used while cooking.
 * Example: <i>activeDutyCycle 0.4</i>
 *
 * <li> <b>inactiveDutyCycle</b> (default: 0.3) -- This changes the ratio of
 * on ticks to total ticks when the refrigerator is being used while not cooking.
 * Example: <i>inactiveDutyCycle 0.4</i>
 *
 * <li> <b>onPower</b> (default: 300) -- This changes the power used while this
 * model is running. Units are in Watts. Example: <i>onPower 2000</i>
 *
 * <li> <b>offPower</b> (default: 0) -- This changes the power used by this model
 * while it is not running. Units are in Watts. Example: <i>offPower 4</i>
 *
 * <li> <b>timeCounter</b> (default: none) -- This attribute is best used for
 * debugging.  Rather than starting in a point in its cycle, the Refrigerator
 * will start at the specified point. Units are in minutes.
 * Example: <i>timeCounter 0</i>
 *
 * </ul>
 */

#ifndef REFRIGERATOR_H_
#define REFRIGERATOR_H_

#include "Model.h"
#include <map>
#include <string>

using namespace std;

class Refrigerator: public Model {

public:

	///Constructor that takes in a map created from the configuration file
	Refrigerator (map<string, string> configuration);

    /// Deconstructor
	virtual ~Refrigerator();

	///Notify the refrigerator that it has been activated by the user.
	virtual void activate();

	///Returns true if the Refrigerator is currently on its "on" cycle
	bool isRunning();

	/**
	 * Notify the refrigerator one minute has passed, and the refrigerator should
	 * count the power consumption, write log file according to the field model.
	 */
	virtual void tick();


protected:

	/// The ratio of on to total ticks when not cooking.
	double inactiveDutyCycle_;

	/// The ratio of on to total ticks when cooking.
	double activeDutyCycle_;

	/// The number of on ticks per cycle while not cooking.
	int inactiveOnPeriod_;

	/// The number of off ticks per cycle while not cooking.
	int inactiveOffPeriod_;

	/// The number of on ticks per cycle while cooking.
	int activeOnPeriod_;

	/// The number of off ticks per cycle while cooking.
	int activeOffPeriod_;

	/// Indicates to the refrigerator if it should use default TimeCounter
	bool useUserTimeCounter_;

	/**
	 * Attempts to apply the user specified values for this model.  If a
	 * particular variable is not given by the user or is invalid, then the
	 * default value will be used. In addition, if certain specified values
	 * cannot be applied, the user will be notified.
	 */
	virtual void applyConfiguration(map<string, string> configuration);

private:
	Refrigerator();
};

#endif /* REFRIGERATOR_H_ */
