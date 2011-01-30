/**
 * @class Stove
 * @brief A model of an ordinary stove.
 *
 * A stove has a strength and a duration that it will burn.
 *
 * <p>Stove can understand the following variables in its model-spec file: </p>
 * <ul><li><b>label</b> (default: Stove) -- This changes the unique identifying name
 * for the Microwave. Example: <i>label Kenmore_Electric_Range</i></li>
 *
 * <li> <b>numberOfBurners</b> (default: 4) -- This changes the total number
 * of burners on the stove. Example: <i>numberOfBurners 2</i>
 *
 * <li> <b>highPower</b> (default: 1400) -- This changes the power used while
 * while a burner is running at the high power level. Units are in Watts.
 * Example: <i>highPower 1200</i>
 *
 * <li> <b>mediumPower</b> (default: 700) -- This changes the power used while
 * while a burner is running at the medium power level. Units are in Watts.
 * Example: <i>mediumPower 300</i>
 *
 * <li> <b>lowPower</b> (default: 350) -- This changes the power used while
 * while a burner is running at the low power level. Units are in Watts.
 * Example: <i>lowPower 100</i>
 *
 * <li> <b>offPower</b> (default: 0) -- This changes the power used by a burner
 * is not on. Units are in Watts. Example: <i>offPower 4</i>
 *
 * </ul>
 */

#ifndef STOVE_H_
#define STOVE_H_

#define COST_LOW 350
#define COST_MEDIUM 700
#define COST_HIGH 1400
#define NUMBER_OF_BURNERS 4

#include "Model.h"

class Stove: public Model {

public:
	/**
	 * Given a configuration it will create the stove.
	 */
	Stove(map<string, string> configuration);

	/**
	 * Destructor
	 */
	virtual ~Stove();

	/**
	 * Turns on an unused burner (if available) with
	 * powerlevel (low, med, or high) and number of
	 * minutes to run
	 */
	virtual void activate(double powerlevel, double length);

	/**
	 * This will notify the microwave that one tick has passed.
	 * During this time it will update the power.
	 */
	virtual void tick();

	/// Returns an open burner.  If no burners are found, returns -1
	virtual int find_burner();

	/**
	 * Attempts to apply the user specified values for this model.  If a
	 * particular variable is not given by the user or is invalid, then the
	 * default value will be used. In addition, if certain specified values
	 * cannot be applied, the user will be notified.
	 */
	virtual void applyConfiguration(map<string, string> configuration);

protected:

	/// Holds the power of each burner.
	int burners_[4];

	/// Holds the number of ticks to run for each burner.
	int burnerLength_[4];

	/// The number of burners on the stove.
	int numberOfBurners_;

	/// The power cost of a burner on low.
	int lowPower_;

	/// The power cost of a burner on medium.
	int mediumPower_;

	/// The power cost of a burner on high.
	int highPower_;

private:
	Stove();
};

#endif /* STOVE_H_ */
