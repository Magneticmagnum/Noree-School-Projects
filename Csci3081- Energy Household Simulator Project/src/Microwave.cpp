#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <map>

#include "../libs/Microwave.h"
#include "../libs/default_configuration.h"




using namespace std;
using log4cxx::Logger;
using log4cxx::LoggerPtr;
using log4cxx::PropertyConfigurator;
using log4cxx::File;

LoggerPtr microwaveLog (Logger::getLogger ("Microwave"));

Microwave::Microwave(map<string, string> configuration) {

	power_ = 0;
	onPeriod_ = 0;
	totalCost_ = 0;
	timeCounter_ = 0;
	type_ = "Microwave";

	onPower_ = MICROWAVE_ON_POWER;
	offPower_ = MICROWAVE_OFF_POWER;
	state_ = MICROWAVE_STATE;
	label_ = MICROWAVE_LABEL;

	applyConfiguration(configuration);

	// Set power usage according to current state.
	if (state_ == ACTIVATED){
		power_ = onPower_;
		LOG4CXX_INFO (microwaveLog, "Microwave has been created in state ACTIVATED");
	}
	else if (state_ == OFF){
		power_ = offPower_;
		LOG4CXX_INFO (microwaveLog, "Microwave has been created in state OFF");
	}


}

///	Destructor
Microwave::~Microwave() {
	LOG4CXX_INFO (microwaveLog, string ("Microwave has left scope. ")
			<< "It has used a total of " << totalCost_ << "W of power.");
}


// Notifies the microwave that one tick has passed.
void Microwave::tick() {
	totalCost_ += power_;

	// Special actions needed only if the toaster is activated.
	if (state_ == ACTIVATED) {
		timeCounter_++;

		// Turn microwave off once the cycle is over.
		if (timeCounter_ >= onPeriod_) {

			power_ = 0;
			timeCounter_ = 0;
			state_ = OFF;
			LOG4CXX_INFO (microwaveLog, "Microwave is finished.");
		}
	}
}


// Primary activate to be used by person. The run length is random.
void Microwave::activate() {

	// Only take action if the state is off.
	if (state_ == OFF) {
		state_ = ACTIVATED;

		// Generate the time randomly betweeen 40 and 80 seconds.
		srand (time (NULL));
		double length = (MAX_LENGTH_SECONDS
				         -(rand() % MAX_MIN_DIFFERENCE_SECONDS))
				         / SECONDS_IN_MINUTE;

		LOG4CXX_INFO (microwaveLog, string ("Microwave has been activated. ")
				<< "It will run for a total of " << length << " minutes.");

		int totalPower = length * onPower_;

		// Distributes the power usage across two ticks, if the run time is
		// greater than a minute, across one tick if it is less.

		if (length <= 1) {
			power_ = totalPower / 1;
			onPeriod_ = 1;
		}

		if (length > 1) {
			power_ = totalPower / 2;
			onPeriod_ = 2;
		}
	}
}

// The activate that will be used by testing. The run length is set by user.
void Microwave::activate(double length) {

	// Only take action if the microwave is off.
	if (state_ == OFF) {
		state_ = ACTIVATED;

		LOG4CXX_INFO (microwaveLog, string ("Microwave has been activated. ")
				<< "It will run for a total of " << length << " minutes.");

		int totalPower = length * onPower_;

		// Distributes the power usage across two ticks, if the run time is
		// greater than a minute, across one tick if it is less.

		if (length <= 1) {
			power_ = totalPower / 1;
			onPeriod_ = 1;
		}

		if (length > 1) {
			power_ = totalPower / 2;
			onPeriod_ = 2;
		}
	}
}

void Microwave::applyConfiguration(map<string, string> configuration){

	/* Take care of the valid map labels. Remove them when finished. */

	if (configuration.find ("label") != configuration.end()){
		label_ = configuration ["label"];
		configuration.erase ("label");
	}



	if (configuration.find("state") != configuration.end()){
		string state_string = configuration["state"];

		if (state_string == "OFF")
			state_ = OFF;

		if (state_string == "ACTIVATED")
			state_ = ACTIVATED;

		configuration.erase("state");
	}

	if (configuration.find("onPower") != configuration.end()){
		onPower_ = atoi (configuration["onPower"].c_str());
		configuration.erase("onPower");

		// Ensure correct value range.
		if (onPower_ < 0){
			LOG4CXX_ERROR(microwaveLog, "Error creating microwave" << onPower_ <<
					" is not a valid value for onPower.  Default used.");

			onPower_ = MICROWAVE_ON_POWER;
		}
	}

	if (configuration.find("offPower") != configuration.end()){
		offPower_ = atoi (configuration["offPower"].c_str());
		configuration.erase("offPower");

		// Ensure correct value range.
		if (offPower_ < 0){
			LOG4CXX_ERROR(microwaveLog, "Error creating microwave" << offPower_ <<
					" is not a valid value for onPower.  Default used.");

			offPower_ = MICROWAVE_OFF_POWER;
		}
	}

	/* Run through the rest of the map and alert user that they weren't used */
	map<string, string>::iterator i;
	for (i = configuration.begin(); i != configuration.end(); i++){
		LOG4CXX_ERROR(microwaveLog, "Configuration entry: " << (*i).first <<
				" " << (*i).second << " is not valid for this model.");
	}
}

