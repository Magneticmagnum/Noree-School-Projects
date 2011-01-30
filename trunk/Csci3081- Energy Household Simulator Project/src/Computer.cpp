#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <iostream>
#include <stdlib.h>

#include "../libs/Television.h"
#include "../libs/Refrigerator.h"
#include "../libs/Model.h"
#include "../libs/default_configuration.h"
#include "../libs/Scheduler.h"
#include "../libs/Computer.h"

using namespace std;
using log4cxx::Logger;
using log4cxx::LoggerPtr;
using log4cxx::PropertyConfigurator;
using log4cxx::File;

LoggerPtr ComputerLog(Logger::getLogger("Computer"));

///	Default constructor, set origin state to OFF;
Computer::Computer(map<string, string> configuration) {

	// Set with default values
	totalCost_ = 0;
	type_ = "Computer";

	state_ = COMPUTER_STATE;
	label_ = COMPUTER_LABEL;
	onPower_ = COMPUTER_ON_POWER;
	offPower_ = COMPUTER_OFF_POWER;

	applyConfiguration(configuration);

	// If state was set to ON, then change power usage accordingly.
	if(state_ == ACTIVATED){
		power_ = onPower_;
		LOG4CXX_INFO(ComputerLog, "Computer has been created in state ON");
	}

	else if(state_ == OFF){
		power_ = offPower_;
		LOG4CXX_INFO(ComputerLog, "Computer has been created in state OFF");
	}


}

///	Destructor
Computer::~Computer() {

	LOG4CXX_INFO(ComputerLog, string("Computer has left scope.")
			<< " Over its life, it has consumed " << totalCost_
			<< "W of power.");
}

/**
 * The Computer begins off.  When activated, it will run until deactivated
 * by calling deactivate()
 *
 */
void Computer::tick() {

	totalCost_ += power_;
}

void Computer::activate() {

	if (state_ == OFF) {

		state_ = ACTIVATED;
		power_ = onPower_;
		LOG4CXX_INFO(ComputerLog, string("Computer has been activated."));
	} else 	if (state_ == ACTIVATED) {

		state_ = OFF;
		power_ = offPower_;
		LOG4CXX_INFO(ComputerLog, string("Computer has been turned off."));
	}
}

void Computer::applyConfiguration(map<string, string> configuration){

	/* Take care of the valid map labels. Remove them when finished. */

	if(configuration.find("label") != configuration.end()){
		label_ = configuration["label"];
		configuration.erase("label");
	}

	if(configuration.find("state") != configuration.end()){
		string state_string = configuration["state"];

		if(state_string == "OFF")
			state_ = OFF;

		if(state_string == "ACTIVATED")
			state_ = ACTIVATED;

		configuration.erase("state");
	}

	if(configuration.find("onPower") != configuration.end()){
		onPower_ = atoi(configuration["onPower"].c_str());
		configuration.erase("onPower");

		// Ensure correct value range.
		if(onPower_ < 0){
			LOG4CXX_ERROR(ComputerLog, "Error creating computer" << onPower_ <<
					" is not a valid value for onPower.  Default used.");

			onPower_ = COMPUTER_ON_POWER;
		}
	}

	if(configuration.find("offPower") != configuration.end()){
		offPower_ = atoi(configuration["offPower"].c_str());
		configuration.erase("offPower");

		// Ensure correct value range.
		if(offPower_ < 0){
			LOG4CXX_ERROR(ComputerLog, "Error creating computer" << offPower_ <<
					" is not a valid value for onPower.  Default used.");

			offPower_ = COMPUTER_OFF_POWER;
		}
	}

	/* Run through the rest of the map and alert user that they weren't used */
	map<string, string>::iterator i;
	for (i = configuration.begin(); i != configuration.end(); i++){
		LOG4CXX_ERROR(ComputerLog, "Configuration entry: " << (*i).first <<
				" " << (*i).second << " is not valid for this model.");
	}
}
