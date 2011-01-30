/**
 * @class WaterHeater
 * @brief Provides hot water to other models.
 *
 * <p>WaterHeater can understand the following variables in its model-spec file: </p>
 * <ul><li><b>label</b> (default: Waterheater) -- This changes the unique identifying name
 * for the WaterHeater. NOTE: If this changes, make sure you also change the
 * waterHeaterName used by the Dishwasher and Shower.
 * Example: <i>label Primary_WaterHeater</i></li>
 *
 * <li> <b>state</b> (default: OFF) -- This changes the current state of the model.
 * It can be changed to OFF or ON. Example: <i>state ON</i>
 *
 * <li> <b>onPower</b> (default: 4500) -- This changes the power used while this
 * model is running. Units are in Watts. Example: <i>onPower 2000</i>
 *
 * <li> <b>offPower</b> (default: 0) -- This changes the power used by this model
 * while it is not running. Units are in Watts. Example: <i>offPower 4</i>
 *
 * <li> <b>maxLiters</b> (default: 200) -- This changes the maximum amount of
 * water that can be held in the waterheater.  Units are in Liters.
 * Example: <i>maxLiters 100</i>
 *
 * <li> <b>maxTemperature</b> (default: 50) -- At any point above this number,
 * the WaterHeater will stop heating. Units are in degrees Celsius.
 * Example: <i>maxTemperature 55</i>
 *
 * <li> <b>minTemperature</b> (default: 40) -- At any point below this number,
 * the waterheater will switch to its ON state. Units are in degrees Celsius.
 * Example: <i>minTemperature 45</i>
 *
 * </ul>
 */

#ifndef WATERHEATER_H_
#define WATERHEATER_H_
#include <map>
#include "Model.h"

class WaterHeater: public Model {

public:
	/// Creates a waterheater based on the specified configuration.
	WaterHeater(map<string, string> configuration);

	//	Destructor
	virtual ~WaterHeater();

	/// Notifies the water heater that a tick has passed.
	virtual void tick();

	///Returns the amount of water currently in the heater
	double getLiters();

	/// Returns the current temperature in the water heater
	double getTemperature();

	/// Simulates the water heater being "asked" for an amount of water at a given temperature.
	virtual void activate(double amount, double temperature);

	/// Returns the liters of water needed from the heater to fulfill the requested amount and temperature.
	double litersNeededFromHeater(double temperatureInHeater, double litersRequested, double temperatureRequested);

	/// Returns the temperature in the water heater after the incoming water has been mixed.
	double temperatureAfterIncoming(double temperatureInHeater, double litersInHeater, double litersIncoming);

protected:

	/// The maximum temperature allowed before the heating element turns off.
	double maxTemperature_;

	/// The minimum temperature reachable before the heating element turns on.
	double minTemperature_;

	/// The current temperature in the waterheater.
	double temperature_;

	/// The current amount of water in the heater.
	double litersInHeater_;

	/// The amount of heat gained per tick while heater is on.
	double heatGainPerTick_;// Depends on the volume and power of the waterheater

	/// The maximum amount of water held by the waterheater.
	int maxLiters_;

	/**
	 * Attempts to apply the user specified values for this model.  If a
	 * particular variable is not given by the user or is invalid, then the
	 * default value will be used. In addition, if certain specified values
	 * cannot be applied, the user will be notified.
	 */
	virtual void applyConfiguration(map<string, string> configuration);


private:
	WaterHeater();

};

#endif /* WATERHEATER_H_ */
