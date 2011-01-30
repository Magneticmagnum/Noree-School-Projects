#include "../libs/Refrigerator.h"
#include "../libs/Oven.h"
#include "../libs/WaterHeater.h"
#include "../libs/Toaster.h"
#include "../libs/Microwave.h"
#include "../libs/Dishwasher.h"
#include "../libs/Television.h"
#include "../libs/Model.h"
#include "../libs/House.h"
#include "../libs/Stove.h"
#include "../libs/utilities.h"
#include "../libs/Person.h"
#include "../libs/default_configuration.h"

#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <stdlib.h>
#include <sys/time.h>
#include <iostream>
#include <string>
#include <map>

using namespace std;
using log4cxx::Logger;
using log4cxx::LoggerPtr;
using log4cxx::PropertyConfigurator;
using log4cxx::File;

LoggerPtr personLog (Logger::getLogger ("Person"));

///	Default constructor, set origin state to OFF;
Person::Person (House& s, map<string, string> configuration){

	todaysCounter_ = 0;
	totalCounter_ = 0;
	type_ = "Person";
	label_ = "Person";
	house_ = &s;
	power_ = 0;
	totalCost_ = 0;
	invalid_ = false;
	state_ = "#";

	applyConfiguration(configuration);

	today_ = planDay ((filename_).c_str());

	LOG4CXX_INFO (personLog, "Person " << label_ << " has been created and will" <<
			" start in state: " << state_);

}

///	Destructor
Person::~Person() {
	LOG4CXX_INFO(personLog, string ("The Person has left scope."));

}


string Person::handleState (event_t &currentEvent){

	// Generates the random seed based on the time in milliseconds.
	struct timeval time;
	gettimeofday (&time, NULL);
	srand ( time.tv_usec );

	/****
	 *****  Code for handling random time states
	 *****/
	if (currentEvent.type == "RandomTime"){

		// If a time has not randomly decided, do so.
		if (currentEvent.randomTimeEvent.decision == -1){

			int range = currentEvent.randomTimeEvent.maximumTime -
					currentEvent.randomTimeEvent.minimumTime;

			currentEvent.randomTimeEvent.decision =
					currentEvent.randomTimeEvent.minimumTime
					+ rand() % (range + 1);
		}

		// Once that time has been reached, move onto the next event.
		// The modulo is used to account for new days being reached.
		if (todaysCounter_ % 1440 == currentEvent.randomTimeEvent.decision)
			return resetEvent (currentEvent);

		// Until then, remain on this state.
		return currentEvent.eventName;
	}

	/****
	 *****  Code for handling delay states
	 *****/
	if (currentEvent.type == "Delay"){


		// Move on once 'delay' number of ticks have passed
		if (currentEvent.delayEvent.timeSinceEventStart ==
				currentEvent.delayEvent.delayLength){
			return resetEvent (currentEvent);

		}

		// Until then, increment the time that has passed and stay on this state
		currentEvent.delayEvent.timeSinceEventStart++;
		return currentEvent.eventName;

	}

	/****
	 *****  Code for handling choice states
	 *****/
	if (currentEvent.type == "Choice"){
		int number = rand() % 100 + 1;

		if (number < (currentEvent.choiceEvent.probabilityOfFirstChoice * 100))
			return currentEvent.nextEvent;

		return currentEvent.choiceEvent.alternateEvent;

	}

	/****
	 *****  Code for handling activation states
	 *****/
	if (currentEvent.type == "Activate"){

		// Setup variables for ease of reading and coding.
		int numberOfArguments = currentEvent.activateEvent.numberOfArguments;
		string modelname = currentEvent.activateEvent.modelName;
		double argument1 = currentEvent.activateEvent.argument1;
		double argument2 = currentEvent.activateEvent.argument2;

		// Activate the specified model with the appropriate activator.

		if (numberOfArguments == 0)
			house_->activateModel (modelname);

		else if (numberOfArguments == 1)
			house_->activateModel (modelname,argument1);

		else if (numberOfArguments == 2)
			house_->activateModel (modelname, argument1, argument2);

		else
			cout << "Error, invalid number of arguments" << endl << endl;

		return currentEvent.nextEvent;
	}

	/****
	 *****  Code for handling random delay states
	 *****/
	if (currentEvent.type == "RandomDelay"){

		// Decide on random number, if not already done so.
		if (currentEvent.randomDelayEvent.decision == -1){

			int range = currentEvent.randomDelayEvent.maximum -
					currentEvent.randomDelayEvent.minimum;

			currentEvent.randomDelayEvent.decision =
					currentEvent.randomDelayEvent.minimum
					+ rand() % (range + 1);
		}

		// Once the decided number of ticks have passed, move on.
		if (currentEvent.randomDelayEvent.timeSinceEventStart ==
				currentEvent.randomDelayEvent.decision){
			return resetEvent (currentEvent);
		}

		// Until then, increment the count and stay on this event.
		currentEvent.randomDelayEvent.timeSinceEventStart++;
		return currentEvent.eventName;

	}

	/****
	 *****  Code for handling dishes states
	 *****/
	if (currentEvent.type == "Dishes"){
		if(house_->getDaysSinceDishwasherRan() >= 3){
			house_ -> activateModel ("Dishwasher");

		}
		return currentEvent.nextEvent;
	}

}

void Person::tick()
{
	LOG4CXX_INFO(personLog, " " << label_ << " is currently in state: " << state_);

	// Make sure that the state being told to go to is valid.
	if(today_.find(state_) != today_.end() && !invalid_){
		state_ = handleState (today_[state_]);

		todaysCounter_++;
	} else {

		return;
	}




}


/***************************************************
 * Functions associated with creating the event list
 ****************************************************/


/**
 * Parses a given file and creates a map between the event names and the event.
 */
map <string, event> Person::planDay (string filePath)
{
	// Creates a map between the event name and the event itself
	map <string, event> events;

	if(invalid_)
		return events;

	string line;
	ifstream file (filePath.c_str());

	// Open file and read it line by line
	if (file.is_open())	{
		while (file.good()){

			getline (file,line);
			if(line != "" && line != " " && line != "\n"){
				string label = extractLabel(line);

				// Ignore lines that begin with #, add all others to events
				if (label[0] != '#'){
					events[label] = parseLine(line);
					if(state_ == "#"){
						state_ = label;
					}
				}
			}
		}
	}
	file.close();
	return events;
}

/**
 * Given a random event, initialize the event struct's variables.
 */

event_t Person::createRandomTimeEvent (vector <string> parts){

	// has form of: eventName RandomTime nextState minimum maximum
	event_t randomEvent;

	randomEvent.parts = parts;
	randomEvent.eventName = parts[0];
	randomEvent.type = parts[1];
	randomEvent.nextEvent = parts[2];
	randomEvent.randomTimeEvent.minimumTime = convertTimeToTicks(parts[3]);
	randomEvent.randomTimeEvent.maximumTime = convertTimeToTicks(parts[4]);
	randomEvent.randomTimeEvent.decision = -1;

	return randomEvent;
}

/**
 * Given a create delay event, initialize the event struct's variables.
 */

event_t Person::createDelayEvent (vector<string> parts){
	event_t delayEvent;

	delayEvent.parts = parts;
	delayEvent.eventName = parts[0];
	delayEvent.type = parts[1];
	delayEvent.nextEvent = parts[2];
	delayEvent.delayEvent.delayLength = atoi (parts[3].c_str());
	delayEvent.delayEvent.timeSinceEventStart = 0;

	return delayEvent;
}


/**
 * Given a choice event, initialize the event struct's variables.
 */
event_t Person::createChoiceEvent (vector<string> parts){
	event_t choiceEvent;

	choiceEvent.parts = parts;
	choiceEvent.eventName = parts[0];
	choiceEvent.type = parts[1];
	choiceEvent.nextEvent = parts[2];
	choiceEvent.choiceEvent.alternateEvent = parts[3];
	choiceEvent.choiceEvent.probabilityOfFirstChoice = atof (parts[4].c_str());

	return choiceEvent;
}

/**
 * Given an activate event, initialize the event struct's variables.
 */
event_t Person::createActivateEvent(vector<string> parts){
	event_t activateEvent;

	int  numberOfArguments = atoi (parts[4].c_str());
	activateEvent.parts = parts;
	activateEvent.eventName = parts[0];
	activateEvent.type = parts[1];
	activateEvent.nextEvent = parts[2];
	activateEvent.activateEvent.modelName = parts[3];
	activateEvent.activateEvent.numberOfArguments = numberOfArguments;

	if (numberOfArguments > 0){
		double arg1 = atof (parts[5].c_str());

		if(parts[5] == "ON")
			arg1 = ON;
		if(parts[5] == "OFF")
			arg1 = OFF;

		activateEvent.activateEvent.argument1 = arg1;
	}

	if (numberOfArguments > 1){
		double arg2 = atof (parts[6].c_str());

		if(parts[6] == "LOW")
			arg2 = LOW;
		if(parts[6] == "MEDIUM")
			arg2 = MEDIUM;
		if(parts[6] == "HIGH")
			arg2 = HIGH;
		if(parts[6] == "OFF")
			arg2 = OFF;

		activateEvent.activateEvent.argument2 = arg2;
	}
	return activateEvent;
}

/**
 * Given a random delay event, initialize the event struct's variables.
 */
event_t Person::createRandomDelayEvent (vector <string> parts){
	event_t randomDelayEvent;

	randomDelayEvent.parts = parts;
	randomDelayEvent.eventName = parts[0];
	randomDelayEvent.type = parts[1];
	randomDelayEvent.nextEvent = parts[2];

	randomDelayEvent.randomDelayEvent.minimum = atoi (parts[3].c_str());
	randomDelayEvent.randomDelayEvent.maximum = atoi (parts[4].c_str());
	randomDelayEvent.randomDelayEvent.timeSinceEventStart = 0;
	randomDelayEvent.randomDelayEvent.decision = -1;

	return randomDelayEvent;
}

/**
 * Given a dishes event, initialize the event struct's variables.
 */
event_t Person::createDishesEvent (vector<string> parts){
	event_t dishesEvent;

	dishesEvent.parts = parts;
	dishesEvent.eventName = parts[0];
	dishesEvent.type = parts[1];
	dishesEvent.nextEvent = parts[2];

	return dishesEvent;
}

/**
 *  Resets event to start state so it can be reused (next day, etc.)
 */
string Person::resetEvent (event_t event){
	string name = event.eventName;
	string nextEvent = event.nextEvent;

	if (event.type == "RandomTime")
		today_[name] = createRandomTimeEvent(event.parts);

	if (event.type == "Delay")
		today_[name] = createDelayEvent(event.parts);

	if (event.type == "RandomDelay")
		today_[name] = createRandomDelayEvent(event.parts);

	LOG4CXX_INFO(personLog, string("Person now entering state ") << nextEvent);
	return nextEvent;

}


/**
 * Chooses the correct event creator depending on the given line.
 */

event_t Person::parseLine (string line){

	vector <string> parts = vectorize (line, " ");

	string type = parts[1];

	if (type == "RandomTime")
		return createRandomTimeEvent (parts);

	if (type == "Delay")
		return createDelayEvent (parts);

	if (type == "Choice")
		return createChoiceEvent (parts);

	if (type == "Activate")
		return createActivateEvent (parts);

	if (type == "RandomDelay")
		return createRandomDelayEvent (parts);

	if (type == "Dishes")
		return createDishesEvent (parts);

}

void Person::applyConfiguration(map<string, string> configuration){

	/* Take care of the valid map labels. Remove them when finished. */

	if(configuration.find("label") != configuration.end()){
		label_ = configuration["label"];
		configuration.erase("label");
	}

	if(configuration.find("startState") != configuration.end()){
		state_ = configuration["startState"];
		configuration.erase("startState");
	} else {
		LOG4CXX_INFO(personLog, "Person was not given a valid start state"
				" the first state listed will be its start state.");
	}


	if(configuration.find("filename") != configuration.end()){
		filename_ = configuration["filename"];
		configuration.erase("filename");

		// Make sure that the file specified exists and can be opened
		fstream file;
		file.open(filename_.c_str(),ios::in);
		if( !file.is_open() ){
			LOG4CXX_ERROR(personLog, "File " << filename_ << " was not found" <<
					"Person MUST be given a valid script to follow." <<
					"Please do so in the model-spec file. Simulation will " <<
					"attempt to continue without person.");
			invalid_ = true;
		}
		file.close();

	} else {
		fprintf(stderr, "Person given invalid script\n");

		LOG4CXX_ERROR(personLog, "Person MUST be given a script to follow." <<
				"Please do so in the model-spec file Simulation will " <<
				"attempt to continue without person.");

		// Program cannot continue without a valid script to follow.
		invalid_ = true;
	}

	/* Run through the rest of the map and alert user that they weren't used */
	map<string, string>::iterator i;
	for (i = configuration.begin(); i != configuration.end(); i++){
		LOG4CXX_ERROR(personLog, "Configuration entry: " << (*i).first <<
				" " << (*i).second << " is not valid for this model.");
	}
}

/*
 * Getters
 */

int Person::getTime()
{
	return todaysCounter_;
}

string Person::getState(){
	return state_;
}



