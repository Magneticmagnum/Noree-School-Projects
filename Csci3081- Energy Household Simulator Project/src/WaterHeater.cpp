#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <map>
#include <stdlib.h>
#include <string>
#include <stdio.h>

#include "../libs/WaterHeater.h"
#include "../libs/Refrigerator.h"
#include "../libs/Model.h"
#include "../libs/default_configuration.h"

using namespace std;
using log4cxx::Logger;
using log4cxx::LoggerPtr;
using log4cxx::PropertyConfigurator;
using log4cxx::File;

LoggerPtr waterHeaterLog(Logger::getLogger("WaterHeater"));

WaterHeater::WaterHeater(map<string, string> configuration) {
	power_ = 0;
	totalCost_ = 0;
	state_ = OFF;
	onPower_ = WATERHEATER_ON_POWER;
	offPower_ = WATERHEATER_OFF_POWER;
	maxLiters_ = WATERHEATER_MAX_LITERS;
	maxTemperature_ = WATERHEATER_HIGH_TEMP_CUTOFF;
	minTemperature_ = WATERHEATER_MIN_TEMP_CUTOFF;

	label_ = WATERHEATER_LABEL;

	applyConfiguration(configuration);

	temperature_ = maxTemperature_;
	litersInHeater_ = maxLiters_;
	heatGainPerTick_ = 60 * onPower_ / WATERHEATER_SPECIFIC_HEAT / litersInHeater_;

	LOG4CXX_INFO(waterHeaterLog, "WaterHeater has been created in its OFF state.");
}

///	Destructor
WaterHeater::~WaterHeater() {
	LOG4CXX_INFO(waterHeaterLog, string("WaterHeater has left scope.") <<
			" Over its life, it has consumed " << totalCost_ << "W of power.");
}

/**
 * The toaster begins off.  When activated, it will run for 2 minutes and
 * consume 900W over that time.
 *
 */
void WaterHeater::tick() {
	LOG4CXX_DEBUG(waterHeaterLog, "WH's current temperature is: " << temperature_);

	if(temperature_ <= minTemperature_ && state_ == OFF){

		power_ = onPower_;
		state_ = ON;

		LOG4CXX_INFO(waterHeaterLog, string("Waterheater is cycling ON."));
	}

	if(temperature_ >= maxTemperature_ && state_ == ON){

		power_ = offPower_;
		state_ = OFF;

		LOG4CXX_INFO(waterHeaterLog, string("Waterheater is cycling OFF."));
	}

	if(state_ == ON)
		temperature_ += heatGainPerTick_;

	totalCost_ += power_;
	temperature_ -= WATERHEATER_HEAT_LOSS_PER_TICK;

}

/// Updates water heater's temperature as a result of the request.
void WaterHeater::activate(double litersRequested, double temperatureRequested){
	double litersFromHeater = litersNeededFromHeater(temperature_,
			                                         litersRequested,
			                                         temperatureRequested);

	litersInHeater_ -= litersRequested;

	temperature_ = temperatureAfterIncoming(temperature_, litersInHeater_,
			                                litersFromHeater);

	LOG4CXX_INFO(waterHeaterLog, string("The temperature in the heater is ")
			<< temperature_ << "C after removing " << litersFromHeater <<"L.");
}

double WaterHeater::getLiters(){
	return litersInHeater_;
}


/// Returns the current temperature of water in the water heater.
double WaterHeater::getTemperature(){
	return temperature_;
}


/// Returns the liters of water needed from the heater to fulfill the
/// amount and temperature.
double WaterHeater::litersNeededFromHeater(double temperatureInHeater,
		                                   double litersRequested,
		                                   double temperatureRequested){

	// If the requested temperature is greater than what the waterheater has
	// available, it will do its best and return as hot of water as possible.
	if(temperatureRequested > temperatureInHeater){
		return litersRequested;
	}

	double numerator = (litersRequested * temperatureRequested)
			- (WATERHEATER_INCOMING_TEMPERATURE * litersRequested);

	double denominator = temperatureInHeater - WATERHEATER_INCOMING_TEMPERATURE;

	return (numerator/denominator);
}


/// Returns the temperature of water after the incoming water has been mixed.
double WaterHeater::temperatureAfterIncoming(double temperatureInHeater,
		                                     double litersInHeater,
		                                     double litersIncoming){

	double numerator = (temperatureInHeater * litersInHeater)
			         + (WATERHEATER_INCOMING_TEMPERATURE * litersIncoming);

	double totalWaterInHeater = litersInHeater_ + litersIncoming;

	litersInHeater_ += litersIncoming;

	return (numerator / totalWaterInHeater);
}



void WaterHeater::applyConfiguration(map<string, string> configuration){

	/* Take care of the valid map labels. Remove them when finished. */

	if(configuration.find("label") != configuration.end()){
		label_ = configuration["label"];
		configuration.erase("label");
	}

	if(configuration.find("offPower") != configuration.end()){
		offPower_ = atoi(configuration["offPower"].c_str());
		configuration.erase("offPower");

		// Ensure correct value range.
		if(offPower_ < 0){
			LOG4CXX_ERROR(waterHeaterLog, "Error creating Toaster" << offPower_ <<
					" is not a valid value for offPower.  Default used.");

			offPower_ = WATERHEATER_OFF_POWER;
		}
	}

	if(configuration.find("onPower") != configuration.end()){
		onPower_ = atoi(configuration["onPower"].c_str());
		configuration.erase("onPower");

		// Ensure correct value range.
		if(onPower_ < 0){
			LOG4CXX_ERROR(waterHeaterLog, "Error creating Toaster" << onPower_ <<
					" is not a valid value for onPower.  Default used.");

			onPower_ = WATERHEATER_ON_POWER;
		}
	}

	if(configuration.find("maxLiters") != configuration.end()){
		maxLiters_ = atoi(configuration["maxLiters"].c_str());
		configuration.erase("maxLiters");

		// Ensure correct value range.
		if(maxLiters_ < 0){
			LOG4CXX_ERROR(waterHeaterLog, "Error creating Waterheater" << maxLiters_ <<
					" is not a valid value for maxLiters.  Default used.");

			maxLiters_ = WATERHEATER_MAX_LITERS;
		}
	}

	if(configuration.find("maxTemperature") != configuration.end()){
		maxTemperature_ = atof(configuration["maxTemperature"].c_str());
		configuration.erase("maxTemperature");

		// Ensure correct value range.
		if(maxTemperature_ < 0){
			LOG4CXX_ERROR(waterHeaterLog, "Error creating Waterheater" << maxTemperature_ <<
					" is not a valid value for maxTemperature.  Default used.");

			maxTemperature_ = WATERHEATER_HIGH_TEMP_CUTOFF;
		}
	}

	if(configuration.find("minTemperature") != configuration.end()){
		minTemperature_ = atoi(configuration["minTemperature"].c_str());
		configuration.erase("minTemperature");

	}

	/* Run through the rest of the map and alert user that they weren't used */
	map<string, string>::iterator i;
	for (i = configuration.begin(); i != configuration.end(); i++){
		LOG4CXX_ERROR(waterHeaterLog, "Configuration entry: " << (*i).first <<
				" " << (*i).second << " is not valid for this model.");
	}
}


