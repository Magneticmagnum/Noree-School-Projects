/**
 * @class Lights
 * @brief A model of an ordinary Lights.
 *
 * An average Lightbulb which uses 60W per hour, or 1W per minute.
 * Each light turned on consumes 1W per minute until turned off.
 * A person can turn as many lights on and off as necessary up
 * to 100 lights.
 *
 * <p>Lights can understand the following variables in its model-spec file: </p>
 * <ul><li><b>label</b> (default: Lights) -- This changes the unique identifying name
 * for the Microwave. Example: <i>label Indoor_lights</i></li>
 *
 * <li> <b>onPower</b> (default: 1) -- This changes the power used while this
 * model is running. Units are in Watts. Example: <i>onPower 2000</i>
 *
 * <li> <b>activatedLights</b> (default: 0) -- This changes the number of lights
 * that are started out in their activate state. Example: <i>activatedLights 4</i>
 *
 * </ul>
 */

#ifndef Lights_H_
#define Lights_H_

#include <map>
#include "Model.h"

class Lights: public Model {

public:
	/**
	 * Creates lights based on the given configuration.
	 */
	Lights (map<string, string> configuration);

	/**
	 * Destructor
	 */
	virtual ~Lights();

	/**
	 * Activates numLights lights to be of state state. For example (10 OFF) would
	 * turn 10 lights off.  (5 ON) would turn 5 lights on.
	 */
	virtual void activate (int numLights, int state);


	/**
	 * This will notify the Lights that one tick has passed.
	 * During this time it will update the power.
	 */
	virtual void tick();


protected:

	///Number of activated lights in household.
	int activatedLights_;

	/**
	 * Attempts to apply the user specified values for this model.  If a
	 * particular variable is not given by the user or is invalid, then the
	 * default value will be used. In addition, if certain specified values
	 * cannot be applied, the user will be notified.
	 */
	virtual void applyConfiguration (map<string, string> configuration);

	int numberOfLights_;

private:
	Lights();
};

#endif /* Lights_H_ */
