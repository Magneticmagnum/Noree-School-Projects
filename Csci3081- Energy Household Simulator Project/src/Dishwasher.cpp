#include <stdio.h>
#include <stdlib.h>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>

#include "../libs/Model.h"
#include "../libs/Scheduler.h"
#include "../libs/Dishwasher.h"
#include "../libs/default_configuration.h"

using namespace std;
using log4cxx::Logger;
using log4cxx::LoggerPtr;
using log4cxx::PropertyConfigurator;
using log4cxx::File;

LoggerPtr dishwasherLog(Logger::getLogger("Dishwasher"));

///	Default constructor, set origin state to OFF;
Dishwasher::Dishwasher(map<string, string> configuration, House& house) {

	// Set with default values
	totalCost_ = 0;
	timeCounter_ = 0;
	type_ = "Dishwasher";

	power_ = DISHWASHER_OFF_POWER;
	onPeriod_ = DISHWASHER_ON_PERIOD;
	firstCycleLength_ = DISHWASHER_FIRST_CYCLE_LENGTH;
	waterDraw_ = DISHWASHER_WATER_DRAW;
	state_ = DISHWASHER_STATE;
	label_ = DISHWASHER_LABEL;
	waterHeaterName_ = DISHWASHER_ASSOCIATED_WATERHEATER;
	onPower_ = DISHWASHER_ON_POWER;
	offPower_ = DISHWASHER_OFF_POWER;
	house_ = &house;

	applyConfiguration(configuration);

	// If state was set to ON, then change power usage accordingly.
	if (state_ == ON) {
		power_ = onPower_;
		LOG4CXX_INFO(dishwasherLog, "Dishwasher has been created in state ON");
	}

	else if (state_ == OFF) {
		power_ = offPower_;
		LOG4CXX_INFO(dishwasherLog, "Dishwasher has been created in state OFF");
	}

}

///	Destructor
Dishwasher::~Dishwasher() {
	LOG4CXX_INFO(dishwasherLog, string("Dishwasher is leaving scope."
		"Over its life it has consumed ") << totalCost_ << "W of energy");

}

// Updates total cost and state based upon time passing
void Dishwasher::tick() {
	totalCost_ += power_;

	// When the dishwasher is running ...
	if (state_ == ON) {
		timeCounter_++;

		// While in the first cycle, draw hottest possible water from heater.
		if (timeCounter_ <= firstCycleLength_) {

			double argument1 = (waterDraw_ / firstCycleLength_);
			double argument2 = MAX_TEMP;
			house_->activateModel(waterHeaterName_, argument1, argument2);
		}

		// When completely finished, turn off.
		if (timeCounter_ >= onPeriod_) {

			power_ = offPower_;
			timeCounter_ = 0;
			state_ = OFF;

			LOG4CXX_INFO(dishwasherLog, string("Dishwasher is turning off."));
		}
	}
}

// Turns the dishwasher on. Sets state and power accordingly.
void Dishwasher::activate() {
	if (state_ == OFF) {
		state_ = ON;
		power_ = onPower_;
		house_->dishwasherRan();

		LOG4CXX_INFO(dishwasherLog, string("Dishwasher has been activated"));
	} else {
		LOG4CXX_ERROR(dishwasherLog, "Dishwasher is already activated");
	}
}

void Dishwasher::applyConfiguration(map<string, string> configuration) {

	/* Take care of the valid map labels. Remove them when finished. */

	if (configuration.find("waterHeaterName") != configuration.end()) {
		waterHeaterName_ = configuration["waterHeaterName"];
		configuration.erase("waterHeaterName");
	}

	if (configuration.find("waterDraw") != configuration.end()) {
		waterDraw_ = atoi(configuration["waterDraw"].c_str());
		configuration.erase("waterDraw");

		// Ensure correct value range.
		if (waterDraw_ < 0) {
			LOG4CXX_ERROR(dishwasherLog, "Error creating dishwasher"
					<< waterDraw_
					<< " is not a valid value for waterDraw.  Default used.");

			waterDraw_ = DISHWASHER_WATER_DRAW;
		}
	}

	if (configuration.find("firstCycleLength") != configuration.end()) {
		firstCycleLength_ = atoi(configuration["firstCycleLength"].c_str());
		configuration.erase("firstCycleLength");

		// Ensure correct value range.
		if (firstCycleLength_ < 0) {
			LOG4CXX_ERROR(
					dishwasherLog,
					"Error creating dishwasher" << firstCycleLength_
							<< " is not a valid value for firstCycleLength.  Default used.");

			firstCycleLength_ = DISHWASHER_FIRST_CYCLE_LENGTH;
		}
	}

	if (configuration.find("onPeriod") != configuration.end()) {
		onPeriod_ = atoi(configuration["onPeriod"].c_str());
		configuration.erase("onPeriod");

		// Ensure correct value range.
		if (onPeriod_ < 0) {
			LOG4CXX_ERROR(dishwasherLog, "Error creating dishwasher"
					<< onPeriod_
					<< " is not a valid value for onPeriod.  Default used.");

			onPeriod_ = DISHWASHER_ON_PERIOD;
		}
	}

	if (configuration.find("label") != configuration.end()) {
		label_ = configuration["label"];
		configuration.erase("label");
	}

	if (configuration.find("state") != configuration.end()) {
		string state_string = configuration["state"];
		if (state_string == "ON")
			state_ = ON;

		if (state_string == "OFF")
			state_ = OFF;

		configuration.erase("state");
	}

	if (configuration.find("onPower") != configuration.end()) {
		onPower_ = atoi(configuration["onPower"].c_str());
		configuration.erase("onPower");

		// Ensure correct value range.
		if (onPower_ < 0) {
			LOG4CXX_ERROR(dishwasherLog, "Error creating dishwasher"
					<< onPower_
					<< " is not a valid value for onPower.  Default used.");

			onPower_ = DISHWASHER_ON_POWER;
		}
	}

	if (configuration.find("offPower") != configuration.end()) {
		offPower_ = atoi(configuration["offPower"].c_str());
		configuration.erase("offPower");

		// Ensure correct value range.
		if (offPower_ < 0) {
			LOG4CXX_ERROR(dishwasherLog, "Error creating dishwasher"
					<< offPower_
					<< " is not a valid value for onPower.  Default used.");

			offPower_ = DISHWASHER_OFF_POWER;
		}
	}

	/* Run through the rest of the map and alert user that they weren't used */
	map<string, string>::iterator i;
	for (i = configuration.begin(); i != configuration.end(); i++) {
		LOG4CXX_ERROR(dishwasherLog, "Configuration entry: " << (*i).first
				<< " " << (*i).second << " is not valid for this model."
				<< " and therefore was not used");
	}
}
