/**
 * @class Shower
 * @brief A model of a Shower
 *
 * This class derives from the Model class.  It describes a Shower that
 * consumes draws 8 liters a minute at 40 degrees Celcius from the
 * WaterHeater.
 *
 * <p>Shower can understand the following variables in its model-spec file: </p>
 * <ul><li><b>label</b> (default: Shower) -- This changes the unique identifying name
 * for the Shower. Example: <i>label Shower</i></li>
 *
 * <li> <b>waterDraw</b> (default: 8) -- This changes the amount of water that
 * the shower draws per minute. Units are in liters. Example: <i>waterDraw 1</i>
 *
 * <li> <b>tempDraw</b> (default: 40) -- This changes the temperature of the
 * water drawn. Units are in degrees Celsius. Example: <i>tempDraw 44</i>
 *
 * <li> <b>waterHeaterName</b> (default: Waterheater) -- This changes the
 * name of the waterheater from which the water will be drawn.
 * Example: <i>waterHeaterName Primary_Waterheater</i>
 *
 * </ul>
 */

#ifndef SHOWER_H_
#define SHOWER_H_

class House;

#include "Model.h"
#include "Scheduler.h"
#include "House.h"

class Shower: public Model {

public:

	/**
	 * Configuration based constructor for a shower.  House is required because
	 * that is where the Shower draws water from.
	 */
	Shower(map<string, string> configuration, House& house);
	/**
	 * Destructor for Shower.
	 */
	virtual ~Shower();

	/**
	 * Turns on the shower if it is off and will turn off the shower if it is
	 * currently on.
	 */
	virtual void activate();

	/**
	 * Notify the Shower one minute has passed, and the Shower should
	 * count the power consumption, write log file according to the field model.
	 */
	virtual void tick();

protected:

	/// The amount of water drawn every tick while active.
	int waterDraw_;

	/// The temperature of water that will be drawn from the shower when on.
	double tempDraw_;

	/// The name of the waterheater from which water will be drawn.
	string waterHeaterName_;

	/// A reference to the house from which the waterheater is registered.
	House* house_;

	/**
	 * Attempts to apply the user specified values for this model.  If a
	 * particular variable is not given by the user or is invalid, then the
	 * default value will be used. In addition, if certain specified values
	 * cannot be applied, the user will be notified.
	 */
	void applyConfiguration(map<string, string> configuration);

private:

	/// The zero-argument constructor should never be used (see above)
	/// and is therefore private.
	Shower();

};

#endif /* SHOWER_H_ */
