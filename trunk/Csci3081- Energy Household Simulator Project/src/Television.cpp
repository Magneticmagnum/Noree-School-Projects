#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>

#include "../libs/Television.h"
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

LoggerPtr televisionLog(Logger::getLogger("Television"));

///	Default constructor, set origin state to off;
Television::Television(map<string, string> configuration) {

	offPower_ = TELEVISION_OFF_POWER;
	onPower_ = TELEVISION_ON_POWER;
	size_ = TELEVISION_SIZE;
	totalCost_ = 0;
	state_ = OFF;
	label_ = "Television";

	applyConfiguration(configuration);

	power_ = offPower_;
	LOG4CXX_INFO(televisionLog, "Default Television has been created in its OFF state");

}

///	Destructor
Television::~Television() {
	LOG4CXX_INFO(televisionLog, string("Television has left scope.")
			<< " Over its life, it has consumed " << totalCost_
			<< "W of power.");
}

/**
 * The television begins off.  When activated, it will run until deactivated.
 * When "on" it uses 180 W, and when "off" it uses 10 W.
 */
void Television::tick() {

	totalCost_ += power_;
}

void Television::activate(){

	if(state_ == OFF){

		state_ = ACTIVATED;
		power_ = onPower_;

		LOG4CXX_INFO(televisionLog, string("Television has been activated."));

	} else if(state_ == ACTIVATED){

		state_ = OFF;
		power_ = offPower_;

		LOG4CXX_INFO(televisionLog, string("Television has been deactivated."));
	}
}

void Television::applyConfiguration(map<string, string> configuration){

	/* Take care of the valid map labels. Remove them when finished. */

	if(configuration.find("label") != configuration.end()){
		label_ = configuration["label"];
		configuration.erase("label");
	}

	if(configuration.find("size") != configuration.end()){
		size_ = atoi(configuration["size"].c_str());
		configuration.erase("size");
	}

	if(configuration.find("offPower") != configuration.end()){
		offPower_ = atoi(configuration["offPower"].c_str());
		configuration.erase("offPower");

		// Ensure correct value range.
		if(offPower_ < 0){
			LOG4CXX_ERROR(televisionLog, "Error creating Television" << offPower_ <<
					" is not a valid value for offPower.  Default used.");

			offPower_ = TELEVISION_OFF_POWER;
		}
	}

	if(configuration.find("onPower") != configuration.end()){
		onPower_ = atoi(configuration["onPower"].c_str());
		configuration.erase("onPower");

		// Ensure correct value range.
		if(onPower_ < 0){
			LOG4CXX_ERROR(televisionLog, "Error creating Television" << onPower_ <<
					" is not a valid value for onPower.  Default used.");

			onPower_ = TELEVISION_ON_POWER;
		}
	}

	/* Run through the rest of the map and alert user that they weren't used */
	map<string, string>::iterator i;
	for (i = configuration.begin(); i != configuration.end(); i++){
		LOG4CXX_ERROR(televisionLog, "Configuration entry: " << (*i).first <<
				" " << (*i).second << " is not valid for this model.");
	}
}
