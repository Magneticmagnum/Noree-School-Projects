#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../libs/Refrigerator.h"
#include "../libs/Model.h"
#include "../libs/Scheduler.h"
#include "../libs/default_configuration.h"



#define CALCULATE_OFF_PERIOD(onPeriod, dutyCycle) onPeriod*(1-dutyCycle)/dutyCycle;

using namespace std;
using log4cxx::Logger;
using log4cxx::LoggerPtr;
using log4cxx::PropertyConfigurator;
using log4cxx::File;


LoggerPtr refrigeratorLog (Logger::getLogger ("Refrigerator"));

/// Applies configuration to the Refrigerator after default values are set.
Refrigerator::Refrigerator (map<string, string> configuration) {
	srand (time (NULL));

	/* Apply default values */

	totalCost_ = 0;
	state_ = -1;
	timeCounter_ = 0;
	useUserTimeCounter_ = false;
	activeDutyCycle_ = REFRIGERATOR_ACTIVE_DUTY_CYCLE;
	inactiveDutyCycle_ = REFRIGERATOR_INACTIVE_DUTY_CYCLE;
	inactiveOnPeriod_ = REFRIGERATOR_ON_PERIOD;
	inactiveOffPeriod_ = CALCULATE_OFF_PERIOD (inactiveOnPeriod_, inactiveDutyCycle_);
	activeOnPeriod_ = REFRIGERATOR_ACTIVE_ON_PERIOD;
	activeOffPeriod_ = CALCULATE_OFF_PERIOD (activeOnPeriod_, activeDutyCycle_);
	onPower_ = REFRIGERATOR_ON_POWER;
	offPower_ = REFRIGERATOR_OFF_POWER;
	label_ = REFRIGERATOR_LABEL;

	applyConfiguration (configuration);

	// If no state was specified, pick one at random.
	if (state_ == -1)
		state_ = rand() % 2;

	if (state_ == ON){
		power_ = onPower_;
		onPeriod_ = inactiveOnPeriod_;
		offPeriod_ = inactiveOffPeriod_;
		if(!useUserTimeCounter_)
			timeCounter_ = (rand() % inactiveOnPeriod_) + 1;
	}

	if (state_ == OFF) {
		power_ = offPower_;
		onPeriod_ = inactiveOnPeriod_;
		offPeriod_ = inactiveOffPeriod_;
		if(!useUserTimeCounter_)
			timeCounter_ = (rand() % inactiveOffPeriod_) + 1;
	}

	LOG4CXX_INFO (refrigeratorLog, string ("Refrigerator of state: ") << state_
			<< string(" was created with the configuration constructor"));
}

///	Destructor
Refrigerator::~Refrigerator() {

	LOG4CXX_INFO (refrigeratorLog, string(
			"Refrigerator is leaving scope. Over its life it has consumed ")
			<< totalCost_ << "W of energy");
}

/// Calculates the total cost, energy used, and maintains the correct state.
void Refrigerator::tick() {

	//Once on cycle is completed, turn off.
	if (timeCounter_ >= onPeriod_ ) {

		timeCounter_ = 0 - offPeriod_;
		power_ = offPower_;

		if (state_ == ON)
			state_ = OFF;

		if (state_ == ACTIVATED)
			state_ = ACTIVATED_OFF;

		LOG4CXX_INFO (refrigeratorLog, string(
				"Refrigerator cycling OFF"));
	} else {

		//	After off cycle is completed, turn on
		if (timeCounter_ == 0) {
			LOG4CXX_INFO (refrigeratorLog, string(
					"Refrigerator cycling ON"));

			power_ = onPower_;

			if (state_ == OFF)
				state_ = ON;

			if (state_ == ACTIVATED_OFF) //gc
				state_ = ACTIVATED;
		}
	}


	totalCost_ += power_;

	//	Time still pass whatever happens
	timeCounter_++;

}

void Refrigerator::activate() {

	// If the refrigerator is in its OFF cycle when it is activated,
	// keep it off, but change its periods accordingly.
	if (state_ == OFF) {
		state_ = ACTIVATED_OFF;

		onPeriod_ = activeOnPeriod_;
		offPeriod_ = activeOffPeriod_;
		LOG4CXX_INFO (refrigeratorLog, "Refrigerator has been activated" <<
				" and is now in its cooking state.");

		// When the refrigerator is done being used and is currently on, leave it
		// running, but return to ON state and change periods accordingly.
	} else if (state_ == ACTIVATED) {
		state_ = ON;

		onPeriod_ = inactiveOnPeriod_;
		offPeriod_ = inactiveOffPeriod_;
		LOG4CXX_INFO (refrigeratorLog, "Refrigerator is no longer activated" <<
				" and is now in its 'not cooking' state.");


		// When the refrigerator is done being used and is currently off, leave it
		// off, but return to OFF state and change periods accordingly.
	} else if (state_ == ACTIVATED_OFF) {
		state_ = OFF;

		onPeriod_ = inactiveOnPeriod_;
		offPeriod_ = inactiveOffPeriod_;
		LOG4CXX_INFO (refrigeratorLog, "Refrigerator is no longer in its" <<
				" cooking state");

		// If the refrigerator is in its ON cycle when it is activated, keep it on,
		// but change periods accordingly.
	} else if (state_ == ON) {
		state_ = ACTIVATED;

		onPeriod_ = activeOnPeriod_;
		offPeriod_ = activeOffPeriod_;
		LOG4CXX_INFO (refrigeratorLog, "Refrigerator is now in its cooking state.");
	}


}

// If the refrigerator is running, return true, else, return false.
bool Refrigerator::isRunning(){

	if (state_ == ACTIVATED || state_ == ON)
		return true;

	return false;
}


void Refrigerator::applyConfiguration (map<string, string> configuration){

	/* Take care of the valid map labels. Remove them when finished. */

	if (configuration.find("label") != configuration.end()){
		label_ = configuration["label"];
		configuration.erase("label");
	}


	if (configuration.find("state") != configuration.end()){
		string state_string = configuration["state"];
		if (state_string == "ON")
			state_ = ON;

		if (state_string == "OFF")   //gc
			state_ = OFF;

		configuration.erase("state");
	}

	if (configuration.find("onPeriod") != configuration.end()){
		activeOnPeriod_ = atoi (configuration["onPeriod"].c_str());
		inactiveOnPeriod_ = atoi (configuration["onPeriod"].c_str());
		onPeriod_ = atoi(configuration["onPeriod"].c_str());
		configuration.erase("onPeriod");

		// Ensure correct value range.
		if (onPeriod_ < 0){
			LOG4CXX_ERROR(refrigeratorLog, "Error creating refrigerator" << onPeriod_ <<
					" is not a valid value for onPeriod.  Default used.");

			activeOnPeriod_ = REFRIGERATOR_ON_PERIOD;
			inactiveOnPeriod_ = REFRIGERATOR_ACTIVE_ON_PERIOD;
		}
	}


	if (configuration.find("inactiveOnPeriod") != configuration.end()){  //gc
		inactiveOnPeriod_ = atoi (configuration["inactiveOnPeriod"].c_str());
		configuration.erase("inactiveOnPeriod");

		// Ensure correct value range.
		if (inactiveOnPeriod_ < 0){
			LOG4CXX_ERROR(refrigeratorLog, "Error creating refrigerator" << inactiveOnPeriod_ <<
					" is not a valid value for inactiveOnPeriod.  Default used.");

			inactiveOnPeriod_ = REFRIGERATOR_ON_PERIOD;
		}
	}

	if (configuration.find("activeOnPeriod") != configuration.end()){
		activeOnPeriod_ = atoi (configuration["activeOnPeriod"].c_str());
		configuration.erase("activeOnPeriod");

		// Ensure correct value range.
		if (activeOnPeriod_ < 0){
			LOG4CXX_ERROR(refrigeratorLog, "Error creating refrigerator" << activeOnPeriod_ <<
					" is not a valid value for activeOnPeriod.  Default used.");

			activeOnPeriod_ = REFRIGERATOR_ON_PERIOD;
		}
	}

	if (configuration.find("activeDutyCycle") != configuration.end()){
		activeDutyCycle_ = atof (configuration["activeDutyCycle"].c_str());
		configuration.erase("activeDutyCycle");

		// Ensure correct value range.
		if (activeDutyCycle_ < 0){
			LOG4CXX_ERROR(refrigeratorLog, "Error creating refrigerator" << activeDutyCycle_ <<
					" is not a valid value for activeDutyCycle.  Default used.");

			activeDutyCycle_ = REFRIGERATOR_ACTIVE_DUTY_CYCLE;
		}
	}

	if (configuration.find("inactiveDutyCycle") != configuration.end()){
		inactiveDutyCycle_ = atof (configuration["inactiveDutyCycle"].c_str());
		configuration.erase("inactiveDutyCycle");

		// Ensure correct value range.
		if (inactiveDutyCycle_ < 0){  //gc
			LOG4CXX_ERROR(refrigeratorLog, "Error creating refrigerator" << inactiveDutyCycle_ <<
					" is not a valid value for inactiveDutyCycle.  Default used.");

			inactiveDutyCycle_ = REFRIGERATOR_INACTIVE_DUTY_CYCLE;
		}
	}

	if (configuration.find("onPower") != configuration.end()){
		onPower_ = atoi (configuration["onPower"].c_str());
		configuration.erase("onPower");

		// Ensure correct value range.
		if (onPower_ < 0){
			LOG4CXX_ERROR(refrigeratorLog, "Error creating computer" << onPower_ <<
					" is not a valid value for onPower.  Default used.");

			onPower_ = REFRIGERATOR_ON_POWER;
		}
	}

	if (configuration.find("offPower") != configuration.end()){
		offPower_ = atoi (configuration["offPower"].c_str());
		configuration.erase("offPower");

		// Ensure correct value range.
		if (offPower_ < 0){
			LOG4CXX_ERROR(refrigeratorLog, "Error creating computer" << offPower_ <<
					" is not a valid value for onPower.  Default used.");

			offPower_ = REFRIGERATOR_OFF_POWER;
		}
	}

	if (configuration.find("timeCounter") != configuration.end()){
		timeCounter_ = atoi (configuration ["timeCounter"].c_str());
		useUserTimeCounter_ = true;
		configuration.erase ("onPower");
	}

	/* Run through the rest of the map and alert user that they weren't used */
	map <string, string>::iterator i;
	for (i = configuration.begin(); i != configuration.end (); i++){
		LOG4CXX_ERROR (refrigeratorLog, "Configuration entry: " << (*i).first <<
				" " << (*i).second << " is not valid for this model.");
	}
}

