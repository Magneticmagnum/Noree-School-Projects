#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <map>

#include "../libs/Scheduler.h"

using namespace std;
using log4cxx::Logger;
using log4cxx::LoggerPtr;
using log4cxx::PropertyConfigurator;
using log4cxx::File;

LoggerPtr schedulerLog(Logger::getLogger("Scheduler"));

int Scheduler::totalTime_;

///	Constructor, set the number of registered model as 0
///	Set the number of ticks passed as 0
Scheduler::Scheduler() {
	PropertyConfigurator::configure(File("log.config"));
	LOG4CXX_INFO(schedulerLog, string("Scheduler has been created."));
	totalTime_ = 0;
}

///	Destructor
Scheduler::~Scheduler() {
	LOG4CXX_INFO(schedulerLog, string("Scheduler has been deconstructed."));
}

///	Put the registered model into a vector array by reference
bool Scheduler::registerHouse(char* modelSpecFile) {
	house_ = new House(modelSpecFile);
	return true;
}

///	Run the loop for limit minutes, during the loop, call
///	the tick() method of the registered house  every minute.
///	Write tick info into the log file
void Scheduler::run(int limit) {

	int day = -1, hours, minutes;

	// For each tick, notify the house and print appropriate log heading.
	for (int i = 0; i < limit; i++) {
		char* formattingHours = "";
		char* formattingMinutes = "";

		hours = (i % 1440) / 60;
		minutes = (i % 1440) - (hours * 60);

		if(hours < 10) formattingHours = "0";
		if(minutes < 10) formattingMinutes = "0";

		// Upon a new day, reset values to zero.
		if((i % 1440) == 0){
			hours = 0;
			minutes = 0;
			day++;
		}

		LOG4CXX_INFO(schedulerLog, string("          ------- Day ") << day
				<< " - " << formattingHours << hours
				<< ":" << formattingMinutes << minutes << " --------");

		house_->tick();

		LOG4CXX_INFO (schedulerLog, "Total amount of energy used since start of simulation: "
				<< house_->getTotalEnergy() / (double)1000 << "kW");

		LOG4CXX_INFO (schedulerLog, "Current instantaneous power usage: "
				<< house_->getTotalPower() / (double)1000 << "kW\n\n");

		totalTime_++;
	}

	///Cleanup resources.

	/* Final log postings */
	LOG4CXX_INFO (schedulerLog, "\n\n");

	LOG4CXX_INFO (schedulerLog, "Simulation ended.  Total amount of energy used: "
			<< house_->getTotalEnergy() / (double)1000 << "kW");

	LOG4CXX_DEBUG (schedulerLog, "Total energy used throughout simulation: "
			<< house_->getTotalEnergy());

	house_->cleanup();
	delete(house_);

	LOG4CXX_INFO (schedulerLog, "\n\n");
}

///	Get the number of ticks have passed
int Scheduler::getTime() {

	return totalTime_;
}

/// Set the number of ticks have passed
void Scheduler::setTime(int time) {
	totalTime_ = time;
}
