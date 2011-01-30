#include "../libs/Model.h"
#include "../libs/Scheduler.h"
#include "../libs/Stove.h"
#include "../libs/default_configuration.h"

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>

using namespace std;
using log4cxx::Logger;
using log4cxx::LoggerPtr;
using log4cxx::PropertyConfigurator;
using log4cxx::File;


LoggerPtr stoveLog (Logger::getLogger ("Stove"));

/// Apply given configuration after setting default values.
Stove::Stove(map <string, string> configuration) {

	/* Set default values */
	totalCost_ = 0;
	timeCounter_ = 0;
	label_ = STOVE_LABEL;
	numberOfBurners_ = STOVE_NUMBER_OF_BURNERS;
	offPower_ = STOVE_OFF_POWER_COST;
	lowPower_ = STOVE_LOW_POWER_COST;
	mediumPower_ = STOVE_MEDIUM_POWER_COST;
	highPower_ = STOVE_HIGH_POWER_COST;

	// Initialize the burners
	for (int i = 0; i < NUMBER_OF_BURNERS; i++) {
		burners_[i] = OFF;
		burnerLength_[i] = 0;
	}

	applyConfiguration(configuration);

	power_ = numberOfBurners_ * offPower_;

	LOG4CXX_INFO(stoveLog, "Stove has been created with all burners turned off");
}

///	Destructor
Stove::~Stove() {
	LOG4CXX_INFO (stoveLog, string ("Stove has left scope. ")
			<< "It has used a total of " << totalCost_ << "W of power.");
}

/// Every tick, recalculate power usage and add it to total power.
void Stove::tick() {

	power_ = 0;

	// Go through all burners and calculate their power and time remaining.
	for (int burner = 0; burner < numberOfBurners_; burner++) {

		if (burners_[burner] == OFF)
			power_ += offPower_;

		if (burners_[burner] == LOW)
			power_ += lowPower_;

		if (burners_[burner] == MEDIUM)
			power_ += mediumPower_;

		if (burners_[burner] == HIGH)
			power_ += highPower_;

		if (burners_[burner] != OFF)
			burnerLength_[burner]--;

		if (burnerLength_[burner] <= 0 && burners_[burner] != OFF){
			burners_[burner] = OFF;
			LOG4CXX_INFO(stoveLog, string("Burner ") << burner << " has turned off.");
		}
	}

	totalCost_ += power_;

}

// Activate a burner at the specified power level for the duration.
void Stove::activate (double powerLevel, double length) {

	int burner = find_burner();

	if (burner == -1){
		LOG4CXX_ERROR(stoveLog, "Not enough open burners to fill request.");
		return;
	}

	burners_[burner] = (int) powerLevel;
	burnerLength_[burner] = (int) length;
	LOG4CXX_INFO (stoveLog, string ("Burner #") << burner <<
			" has been activated at power level " << burners_[burner] <<
			" for " << burnerLength_[burner] << " minutes");
}

// Returns an open burner.  If no burners are found, returns -1
int Stove::find_burner(){

	// Search through all the burners.
	for (int burner = 0; burner < numberOfBurners_; burner++){

		if (burners_[burner] == OFF)
			return burner;
	}
	return -1;
}

void Stove::applyConfiguration(map <string, string> configuration){

	/* Take care of the valid map labels. Remove them when finished. */

	if(configuration.find ("label") != configuration.end()){
		label_ = configuration ["label"];
		configuration.erase ("label");
	}

	if(configuration.find("numberOfBurners") != configuration.end()){
		numberOfBurners_ = atoi(configuration["numberOfBurners"].c_str());
		configuration.erase("numberOfBurners");

		// Ensure correct value range.
		if(numberOfBurners_ < 0){
			LOG4CXX_ERROR(stoveLog, "Error creating stove" << numberOfBurners_ <<
					" is not a valid value for numberOfBurners.  Default used.");

			numberOfBurners_ = STOVE_NUMBER_OF_BURNERS;
		}
	}

	if(configuration.find("highPower") != configuration.end()){
		highPower_ = atoi(configuration["highPower"].c_str());
		configuration.erase("highPower");

		// Ensure correct value range.
		if(highPower_ < 0){
			LOG4CXX_ERROR(stoveLog, "Error creating stove" << highPower_ <<
					" is not a valid value for highPower.  Default used.");

			highPower_ = STOVE_HIGH_POWER_COST;
		}
	}

	if(configuration.find("mediumPower") != configuration.end()){
		mediumPower_ = atoi(configuration["mediumPower"].c_str());
		configuration.erase("mediumPower");

		// Ensure correct value range.
		if(mediumPower_ < 0){
			LOG4CXX_ERROR(stoveLog, "Error creating stove" << mediumPower_ <<
					" is not a valid value for mediumPower.  Default used.");

			mediumPower_ = STOVE_MEDIUM_POWER_COST;
		}
	}


	if(configuration.find("lowPower") != configuration.end()){
		lowPower_ = atoi(configuration["lowPower"].c_str());
		configuration.erase("lowPower");

		// Ensure correct value range.
		if(lowPower_ < 0){
			LOG4CXX_ERROR(stoveLog, "Error creating stove" << lowPower_ <<
					" is not a valid value for lowPower.  Default used.");

			lowPower_ = STOVE_LOW_POWER_COST;
		}
	}

	if(configuration.find("offPower") != configuration.end()){
		offPower_ = atoi(configuration["offPower"].c_str());
		configuration.erase("offPower");

		// Ensure correct value range.
		if(offPower_ < 0){
			LOG4CXX_ERROR(stoveLog, "Error creating stove" << offPower_ <<
					" is not a valid value for onPower.  Default used.");

			offPower_ = STOVE_OFF_POWER_COST;
		}
	}

	/* Run through the rest of the map and alert user that they weren't used */
	map<string, string>::iterator i;
	for (i = configuration.begin(); i != configuration.end(); i++){
		LOG4CXX_ERROR(stoveLog, "Configuration entry: " << (*i).first <<
				" " << (*i).second << " is not valid for this model.");
	}
}
