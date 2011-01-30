/**
 * @class Dishwasher
 * @brief A model of a Dishwasher
 *
 * This class derives from the Model class.  It describes a dishwasher that
 * consumes 2kW-hours when throughout its 45-minute cycle when it is turned on.
 * Also for the first 20-minutes of that cycle, it consumes a total of 22L of hot
 * water.
 *
 * <p>Dishwasher can understand the following variables in its model-spec file: </p>
 * <ul><li><b>label</b> (default: Dishwasher) -- This changes the unique
 * identifying name for the Dishwasher. Example: <i>label Dishcleaner9000</i></li>
 *
 * <li> <b>state</b> (default: OFF) -- This changes the current state of the model. It can be
 * changed to OFF or ON. Example: <i>state ON</i>
 *
 * <li> <b>onPower</b> (default: 2000) -- This changes the power used while this
 * model is running. Units are in Watts. Example: <i>onPower 1000</i>
 *
 * <li> <b>offPower</b> (default: 4) -- This changes the power used by this model
 * while it is not running. Units are in Watts. Example: <i>offPower 4</i>
 *
 * <li> <b>waterHeaterName</b> (default: Waterheater) -- This changes the
 * waterheater from which the Dishwasher will draw water.
 * Example: <i>waterHeaterName outdoorWaterHeater</i>
 *
 * <li> <b>waterDraw</b> (default: 22) -- This changes the total amount of water drawn
 * by the Dishwasher during its first cycle. Units are in Liters.
 * Example: <i>waterDraw 25</i>
 *
 * <li> <b>firstCycleLength</b> (default: 20) -- This changes the length of
 * the dishwasher's first cycle. Example: <i>firstCycleLength 4</i>
 *
 * </ul>
 */

#ifndef DISHWASHER_H_
#define DISHWASHER_H_

const int OFF_POWER = 4;
const int ACTIVATED_POWER = 2000;
const int ON_PERIOD_LENGTH = 45;
const int LENGTH_OF_FIRST_CYCLE = 20;
const int WATER_DRAW = 22;
class House;

#include "Model.h"

#include "House.h"
using namespace std;

class Dishwasher: public Model {

public:

	/**
	 * Constructor for Dishwasher.  House is required because the
	 * dishwasher needs to get water from the WaterHeater during its
	 * cycles.  This is done through the house. The configuration
	 * specifies attributes for the Dishwasher.
	 */
	Dishwasher(map<string, string> configuration, House& house);

	/**
	 * Destructor for Dishwasher.
	 */
	virtual ~Dishwasher();

	/**
	 * This will activate the dishwasher.  Upon activation, the dishwasher
	 * will run for 45 minutes. If the dishwasher is already on, nothing happens
	 */
	virtual void activate();


	/**
	 * Notify the refrigerator one minute has passed, and the dishwasher should
	 * count the power consumption, write log file according to the field model.
	 */
	virtual void tick();

protected:

	/// Length of the first cycle.
	int firstCycleLength_;

	/// The amount of water drawn during the first cycle.
	int waterDraw_;

	/// A reference to the house which the Dishwasher will draw water from.
	House* house_;

	/// The name of the water heater to be used to draw water.
	string waterHeaterName_;

	/**
	 * Attempts to apply the user specified values for this model.  If a
	 * particular variable is not given by the user or is invalid, then the
	 * default value will be used. In addition, if certain specified values
	 * cannot be applied, the user will be notified.
	 */
	virtual void applyConfiguration(map<string, string> configuration);

private:

	/// The zero-argument constructor should never be used (see above)
	/// and is therefor private.
	Dishwasher();

};

#endif /* DISHWASHER_H_ */
