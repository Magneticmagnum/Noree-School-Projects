#include "../libs/Lights.h"
#include "../libs/Refrigerator.h"
#include "../libs/Model.h"
#include "../libs/default_configuration.h"

#include "../libs/Scheduler.h"
#include "../libs/Toaster.h"
#include <iostream>
#include <stdlib.h>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>

using namespace std;
using log4cxx::Logger;
using log4cxx::LoggerPtr;
using log4cxx::PropertyConfigurator;
using log4cxx::File;

LoggerPtr LightsLog(Logger::getLogger("Lights"));

///	Configuration-based constructor
Lights::Lights(map<string, string> configuration) {
	totalCost_ = 0;
	power_ = 0;
	type_ = "Lights";

	onPower_ = LIGHTS_ON_POWER;
	offPower_ = LIGHTS_OFF_POWER;
	state_ = LIGHTS_STATE;
	label_ = LIGHTS_LABEL;
	numberOfLights_ = LIGHTS_NUMBER_OF;
	activatedLights_ = LIGHTS_ACTIVATED;

	applyConfiguration(configuration);

	LOG4CXX_INFO(LightsLog, "Lights have been created. In its initial" <<
			" state there are " << activatedLights_ << " on.");
}

///	Destructor
Lights::~Lights() {
	LOG4CXX_INFO(LightsLog, string("Lights have left scope.")
			<< " Over its life, it has consumed " << totalCost_
			<< "W of power.");
}

/**
 * The Lights begins off.  When activated, it will run until deactivated.
 * When "on" each activated light will consume 1W.
 */
void Lights::tick() {

	totalCost_ += (onPower_ * activatedLights_);
}

void Lights::activate(int numLights, int state) {

	if (state == ON) {

		if (activatedLights_ == 0)
			state_ = ACTIVATED;

		if (numLights > numberOfLights_) {
			LOG4CXX_WARN(LightsLog, "Attempting to turn on too many" <<
					" lights. Turning on maximum possible number.");

			activatedLights_ = numberOfLights_;

		} else {

			activatedLights_ += numLights;

			LOG4CXX_INFO(LightsLog, string("Number of lights activated is")
					<< activatedLights_);
		}
	}

	if (state == OFF) {
		if (numLights > activatedLights_) {
			LOG4CXX_WARN(LightsLog, "Cannot turn off more lights than"
					<< " are on. Turning off as many as possible");

			activatedLights_ = 0;
			state_ = OFF;

		} else {
			activatedLights_ -= numLights;

			if (activatedLights_ == 0)
				state_ = OFF;
		}

		LOG4CXX_INFO(LightsLog, "After turning off " << numLights <<
				"There are " << activatedLights_ << " lights on");
	}

	// Regardless of change, update power usage.
	power_ = onPower_ * activatedLights_;
}

void Lights::applyConfiguration(map<string, string> configuration) {

	/* Take care of the valid map labels. Remove them when finished. */

	if (configuration.find("label") != configuration.end()) {
		label_ = configuration["label"];
		configuration.erase("label");
	}

	if (configuration.find("onPower") != configuration.end()) {
		onPower_ = atoi(configuration["onPower"].c_str());
		configuration.erase("onPower");

		// Ensure correct value range.
		if (onPower_ < 0) {
			LOG4CXX_ERROR(LightsLog, "Error creating lights" << onPower_
					<< " is not a valid value for onPower.  Default used.");

			onPower_ = LIGHTS_ON_POWER;
		}
	}

	if (configuration.find("activatedLights") != configuration.end()) {
		activatedLights_ = atoi(configuration["activatedLights"].c_str());
		configuration.erase("activatedLights");

		// Ensure correct value range.
		if (activatedLights_ < 0) {
			LOG4CXX_ERROR(
					LightsLog,
					"Error creating lights" << activatedLights_
					<< " is not a valid value for activatedLights.  Default used.");

			activatedLights_ = LIGHTS_ACTIVATED;
		}
	}

	/* Run through the rest of the map and alert user that they weren't used */
	map<string, string>::iterator i;
	for (i = configuration.begin(); i != configuration.end(); i++) {
		LOG4CXX_ERROR(LightsLog, "Configuration entry: " << (*i).first << " "
				<< (*i).second << " is not valid for this model.");
	}
}
