/**
 * @class Oven
 * @brief A model of an Oven.
 *
 * <p>This class derives from the Model class.  It describes an Oven that when
 * activated will preheat itself at one powerlevel before changing to a different
 * power level to "bake" for a specified amount of time.</p>
 *
 * <p>Oven can understand the following variables in its model-spec file: </p>
 * <ul><li><b>label</b> (default: Oven) -- This changes the unique identifying name
 * for the oven. Example: <i>label Kenmore_Gemini</i></li>
 *
 * <li> <b>state</b> (default: OFF) -- This changes the current state of the oven. It can be
 * changed to OFF or ACTIVATED. Example: <i>state ACTIVATED</i>
 *
 * <li> <b>preHeatPower</b> (default: 7500) -- This changes the power used by the oven during
 * its preHeat state. Units are in Watts. Example: <i>preHeatPower 2000</i>
 *
 * <li> <b>cookPower</b> (default: 2500) -- This changes the power used by the oven during
 * its cooking state. Units are in Watts. Example: <i>cookPower 2000</i>
 *
 * <li> <b>offPower</b> (default: 0) -- This changes the power used by the oven while it is
 * not running. Units are in Watts. Example: <i>offPower 4</i>
 *
 * <li> <b>preHeatTime</b> (default: 10) -- This changes the length of time the oven needs to
 * spend preheating. Units are in minutes. Example: <i>preHeatTime 10</i>
 *
 * </ul>
 */

#ifndef OVEN_H_
#define OVEN_H_

#include <map>
#include "Model.h"

class Oven: public Model {

public:
	/**
	 * Creates an oven based on the given configuration.
	 */
	Oven(map<string, string> configuration);

	/*
	 *  Destructor
	 */
	~Oven();

	/**
	 * Notify the Oven one minute has passed, and the oven should
	 * count the power consumption, write log file according to the field model.
	 */
	virtual void tick();

	/**
	 * Will run the oven for the specified bake time plus an additional preheat
	 * time.
	 */
	virtual void activate(double);

protected:

	/// The pre-heat period of the oven using more energy.
	int preHeatTime_;

	/**
	 * The period where oven is on and requires only the energy to sustain
	 * the desired temperature.
	 */
	int cookTime_;

	/// The amount of power used during the cooking period.
	int cookPower_;

	/// The amount of power used during the preheating period.
	int preHeatPower_;

	/**
	 * Attempts to apply the user specified values for this model.  If a
	 * particular variable is not given by the user or is invalid, then the
	 * default value will be used. In addition, if certain specified values
	 * cannot be applied, the user will be notified.
	 */
	virtual void applyConfiguration(map<string, string> configuration);

private:
	Oven();

};

#endif /* OVER_H_ */
