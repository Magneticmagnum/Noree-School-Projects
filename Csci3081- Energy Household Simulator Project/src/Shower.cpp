#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>

#include "../libs/Model.h"
#include "../libs/House.h"
#include "../libs/Shower.h"
#include "../libs/default_configuration.h"

using namespace std;
using log4cxx::Logger;
using log4cxx::LoggerPtr;
using log4cxx::PropertyConfigurator;
using log4cxx::File;

LoggerPtr showerLog(Logger::getLogger("Shower"));

Shower::Shower(map<string, string> configuration, House& house) {

	power_ = 0;
	totalCost_ = 0;
	state_ = OFF;
	waterHeaterName_ = WATERHEATER_LABEL;
	waterDraw_ = SHOWER_WATER_DRAW;
	tempDraw_ = SHOWER_WATER_TEMPERATURE;
	label_ = SHOWER_LABEL;
	house_ = &house;

	applyConfiguration(configuration);
	LOG4CXX_INFO(showerLog, string("Shower has been created in its OFF state"));
}



///	Destructor
Shower::~Shower() {
	LOG4CXX_INFO(showerLog, string("Shower is leaving scope."
			"Over its life it has consumed ") << totalCost_ << "W of energy");
}


/**
 * While active, with each tick's passage, will draw water from the specified
 * waterheater.
 */
void Shower::tick(){
	if(state_ == ACTIVATED){
		house_->activateModel(waterHeaterName_, waterDraw_, tempDraw_);
	}
}

/**
 * Toggles the waterheater on or off.
 */
void Shower::activate(){
	if(state_ == ACTIVATED){
		state_ = OFF;
		LOG4CXX_INFO(showerLog, string("Shower has been turned off"));
	} else if(state_ == OFF){
		state_ = ACTIVATED;
		LOG4CXX_INFO(showerLog, string("Shower has been turned on"));
	}
}

/**
 * Runs through all of the possible configurations and if possible, apply them
 * to the Shower while it is being created.
 */
void Shower::applyConfiguration(map<string, string> configuration){

	/* Take care of the valid map labels. Remove them when finished. */

	if(configuration.find("label") != configuration.end()){
		label_ = configuration["label"];
		configuration.erase("label");
	}

	if(configuration.find("waterHeaterName") != configuration.end()){
		waterHeaterName_ = configuration["waterHeaterName"];
		configuration.erase("waterHeaterName");
	}

	if(configuration.find("waterDraw") != configuration.end()){
		waterDraw_ = atoi(configuration["waterDraw"].c_str());
		configuration.erase("waterDraw");

		if(waterDraw_ < 0){
			LOG4CXX_ERROR(showerLog, "waterDraw cannot be negative. Default used.");

			waterDraw_ = SHOWER_WATER_DRAW;
		}
	}


	if(configuration.find("tempDraw") != configuration.end()){
		tempDraw_ = atoi(configuration["tempDraw"].c_str());
		configuration.erase("tempDraw");

		if(tempDraw_ < 0){
			LOG4CXX_ERROR(showerLog, "tempDraw cannot be negative. Default used.");

			tempDraw_ = SHOWER_WATER_TEMPERATURE;
		}
	}

	/* Run through the rest of the map and alert user that they weren't used */
	map<string, string>::iterator i;
	for (i = configuration.begin(); i != configuration.end(); i++){
		LOG4CXX_ERROR(showerLog, "Configuration entry: " << (*i).first <<
				" " << (*i).second << " is not valid for this model.");
	}
}
