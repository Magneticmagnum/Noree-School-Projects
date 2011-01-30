#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <iostream>
#include <stdlib.h>

#include "../libs/Toaster.h"
#include "../libs/Refrigerator.h"
#include "../libs/Model.h"
#include "../libs/default_configuration.h"
#include "../libs/Scheduler.h"
#include "../libs/Toaster.h"

using namespace std;
using log4cxx::Logger;
using log4cxx::LoggerPtr;
using log4cxx::PropertyConfigurator;
using log4cxx::File;

LoggerPtr toasterLog(Logger::getLogger("Toaster"));

///	Default constructor, set origin state to OFF;
Toaster::Toaster(map<string, string> configuration) {

	totalCost_ = 0;
	timeCounter_ = 0;
	state_ = OFF;
	label_ = TOASTER_LABEL;
	onPeriod_ = TOASTER_COOK_TIME;
	onPower_ = TOASTER_ON_POWER;
	offPower_ = TOASTER_OFF_POWER;

	applyConfiguration(configuration);

	power_ = offPower_;

	LOG4CXX_INFO(toasterLog, string("Toaster has been created in its OFF state."));
}

///	Destructor
Toaster::~Toaster() {

	LOG4CXX_INFO(toasterLog, string("Toaster has left scope.") <<
			" Over its life, it has consumed " << totalCost_ << "W of power.");

}

/**
 * The toaster begins off.  When activated, it will run for 2 minutes and
 * consume 900W over that time.
 *
 */
void Toaster::tick() {

	// Once the on period was reached, turn off the toaster.
	if (timeCounter_ == onPeriod_ -1) {

		timeCounter_ = 0;
		state_ = OFF;
		power_ = offPower_;

		LOG4CXX_INFO(toasterLog, string("Toaster has turned off."));
	}

	if (state_ == ACTIVATED)
		timeCounter_++;

	totalCost_ += power_;
}

void Toaster::activate(){

	if(state_ == OFF){

		state_ = ACTIVATED;
		power_ = onPower_;

		LOG4CXX_INFO(toasterLog, string("Toaster has been activated."));
	}

}

void Toaster::applyConfiguration(map<string, string> configuration){

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
			LOG4CXX_ERROR(toasterLog, "Error creating Toaster" << offPower_ <<
					" is not a valid value for offPower.  Default used.");

			offPower_ = TOASTER_OFF_POWER;
		}
	}

	if(configuration.find("onPower") != configuration.end()){
		onPower_ = atoi(configuration["onPower"].c_str());
		configuration.erase("onPower");

		// Ensure correct value range.
		if(onPower_ < 0){
			LOG4CXX_ERROR(toasterLog, "Error creating Toaster" << onPower_ <<
					" is not a valid value for onPower.  Default used.");

			onPower_ = TOASTER_ON_POWER;
		}
	}

	if(configuration.find("onPeriod") != configuration.end()){
		onPeriod_ = atoi(configuration["onPeriod"].c_str());
		configuration.erase("onPeriod");

		// Ensure correct value range.
		if(onPeriod_ < 0){
			LOG4CXX_ERROR(toasterLog, "Error creating Toaster" << onPeriod_ <<
					" is not a valid value for onPeriod.  Default used.");

			onPeriod_ = TOASTER_COOK_TIME;
		}
	}

	/* Run through the rest of the map and alert user that they weren't used */
	map<string, string>::iterator i;
	for (i = configuration.begin(); i != configuration.end(); i++){
		LOG4CXX_ERROR(toasterLog, "Configuration entry: " << (*i).first <<
				" " << (*i).second << " is not valid for this model.");
	}
}

