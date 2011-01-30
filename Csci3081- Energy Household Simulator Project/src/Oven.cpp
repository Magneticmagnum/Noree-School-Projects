#include "../libs/Model.h"
#include "../libs/Oven.h"
#include "../libs/default_configuration.h"

#include <iostream>
#include <stdlib.h>
#include <map>
#include <stdio.h>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>

using namespace std;
using log4cxx::Logger;
using log4cxx::LoggerPtr;
using log4cxx::PropertyConfigurator;
using log4cxx::File;

LoggerPtr ovenLog(Logger::getLogger("Oven"));

///	Default constructor, set origin state to ON;
Oven::Oven(map<string, string> configuration){



	power_ = 0;
	totalCost_ = 0;
	timeCounter_ = 0;
	type_ = "Oven";

	offPower_ = OVEN_OFF_POWER;
	preHeatTime_ = OVEN_PRE_HEAT_TIME;
	preHeatPower_ = OVEN_PRE_HEAT_POWER;
	cookPower_ = OVEN_COOK_POWER;
	state_ = OVEN_STATE;
	label_ = OVEN_LABEL;

	applyConfiguration(configuration);

}

///	Destructor
Oven::~Oven()
{
	LOG4CXX_INFO(ovenLog, string("Oven is leaving scope."
			"Over its life it has consumed ")
			<< totalCost_ << "W of energy");
}


void Oven::tick()
{
	if (state_ == ACTIVATED){
		timeCounter_++;

		if (timeCounter_ >= preHeatTime_ &&
				timeCounter_ <= cookTime_ &&
				!(power_ == cookPower_)){
			LOG4CXX_INFO(ovenLog, string("Oven switching to bake mode"));
			power_ = cookPower_;
		}

		if (timeCounter_ >= cookTime_)	{
			LOG4CXX_INFO(ovenLog, string("Oven is turning off."));
			power_ = offPower_;
			state_ = OFF;
		}
	}

	totalCost_ += power_;
}

void Oven::activate(double cookTime)
{
	if (state_ == OFF){

		timeCounter_ = 0;
		state_ = ACTIVATED;
		power_ = preHeatPower_;
		cookTime_ = (int)cookTime + preHeatTime_;

		LOG4CXX_INFO(ovenLog, string("The oven has been activated."));

		return;
	}

	LOG4CXX_ERROR(ovenLog, string("The oven is already running."));

}

void Oven::applyConfiguration(map<string, string> configuration){

	/* Take care of the valid map labels. Remove them when finished. */

	if (configuration.find("label") != configuration.end()){
		label_ = configuration["label"];
		configuration.erase("label");
	}

	if (configuration.find("state") != configuration.end()){
		string state_string = configuration["state"];

		if (state_string == "OFF")
			state_ = OFF;

		if (state_string == "ACTIVATED")
			state_ = ACTIVATED;

		configuration.erase("state");
	}

	if (configuration.find("preHeatPower") != configuration.end()){
		preHeatPower_ = atoi(configuration["preHeatPower"].c_str());
		configuration.erase("preHeatPower");

		// Ensure correct value range.
		if (preHeatPower_ < 0){
			LOG4CXX_ERROR(ovenLog, "Error creating oven" << preHeatPower_ <<
					" is not a valid value for onPower.  Default used.");

			preHeatPower_ = OVEN_PRE_HEAT_POWER;
		}
	}

	if (configuration.find("cookPower") != configuration.end()){
		cookPower_ = atoi(configuration["cookPower"].c_str());
		configuration.erase("cookPower");

		// Ensure correct value range.
		if (cookPower_ < 0){
			LOG4CXX_ERROR(ovenLog, "Error creating oven" << cookPower_ <<
					" is not a valid value for onPower.  Default used.");

			cookPower_ = OVEN_COOK_POWER;
		}
	}

	if (configuration.find("preHeatTime") != configuration.end()){
		preHeatTime_ = atoi(configuration["preHeatTime"].c_str());
		configuration.erase("preHeatTime");

		// Ensure correct value range.
		if (preHeatTime_ < 0){
			LOG4CXX_ERROR(ovenLog, "Error creating oven" << preHeatTime_ <<
					" is not a valid value for onPower.  Default used.");

			preHeatTime_ = OVEN_PRE_HEAT_TIME;
		}
	}


	if (configuration.find("offPower") != configuration.end()){
		offPower_ = atoi(configuration["offPower"].c_str());
		configuration.erase("offPower");

		// Ensure correct value range.
		if (offPower_ < 0){
			LOG4CXX_ERROR(ovenLog, "Error creating oven" << offPower_ <<
					" is not a valid value for onPower.  Default used.");

			offPower_ = OVEN_OFF_POWER;
		}
	}

	/* Run through the rest of the map and alert user that they weren't used */
	map<string, string>::iterator i;
	for (i = configuration.begin(); i != configuration.end(); i++){
		LOG4CXX_ERROR(ovenLog, "Configuration entry: " << (*i).first <<
				" " << (*i).second << " is not valid for this model.");
	}
}

