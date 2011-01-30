#ifndef TELEVISION_H_
#define TELEVISION_H_

#include <map>
#include "Model.h"
/**
 * @class Television
 * @brief A model of an ordinary television.
 *
 * A Television is a Model. When "on" it uses 180 W, and when "off" it uses 10 W.
 *
 * <p>Television can understand the following variables in its model-spec file: </p>
 * <ul><li><b>label</b> (default: Television) -- This changes the unique identifying name
 * for the Microwave. Example: <i>label 1080p_Plasma</i></li>
 *
 * <li> <b>size</b> (default: 27) -- This changes the size of the model. It is
 * specified in inches. Example: <i>size 56</i>
 *
 * <li> <b>onPower</b> (default: 200) -- This changes the power used while this
 * model is running. Units are in Watts. Example: <i>onPower 2000</i>
 *
 * <li> <b>offPower</b> (default: 25) -- This changes the power used by this model
 * while it is not running. Units are in Watts. Example: <i>offPower 4</i>
 *
 * </ul>
 */

class Television: public Model {

public:

	/**
	 * Creates a television according to the specified configuration
	 *
	 */
	Television(map<string, string> configuration);

	/**
	 * Destructor
	 */
	virtual ~Television();

	/// Depending on current state will either turn TV on or off.
	virtual void activate();

	/**
	 * Notify the television one minute has passed, and the television should
	 * count the power consumption, write log file according to the field model.
	 */
	virtual void tick();

protected:

	/// The size of the TV in inches.
	int size_;

	/**
	 * Attempts to apply the user specified values for this model.  If a
	 * particular variable is not given by the user or is invalid, then the
	 * default value will be used. In addition, if certain specified values
	 * cannot be applied, the user will be notified.
	 */
	virtual void applyConfiguration(map<string, string> configuration);

private:
	Television();
};

#endif /* TELEVISION_H_ */
