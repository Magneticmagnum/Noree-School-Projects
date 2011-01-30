#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/Scheduler.h"
#include "../libs/Refrigerator.h"
#include "../libs/Dishwasher.h"
#include "../libs/Microwave.h"
#include "../libs/Oven.h"
#include "../libs/Stove.h"
#include "../libs/Television.h"
#include "../libs/Toaster.h"
#include "../libs/WaterHeater.h"
#include "../libs/Person.h"
#include "../libs/utilities.h"

using namespace std;
using log4cxx::Logger;
using log4cxx::LoggerPtr;
using log4cxx::PropertyConfigurator;
using log4cxx::File;

using namespace std;

#define DEFAULT_DAYS_TO_RUN 10				//Number of ticks in a given day

int main(int argc, char* const argv[]) {
	LoggerPtr log(Logger::getLogger("simulator"));
	PropertyConfigurator::configure(File("log.config"));

	LOG4CXX_INFO(log, "          -----Starting Simulation-----");
	Scheduler scheduler;

	int positionInArray = 1;
	int daysToRun = 10;
	int modelFileLocation;
	int personCount = 1;

	string logfile;

	// Ensure valid number of arguments.
	if (argc < 2) {
		printf("Invalid entry to simulator. The form must be: \n"
				"./simulator [-d ndays] model-spec person-spec ...\n");
		exit(0);
	}

	// Handle case where days specified.
	if(strcmp(argv[1], "-d") == 0){

		// Ensure valid number of arguments.
		if(argc < 3){
			printf("Invalid entry to simulator. The form must be: \n"
					"./simulator [-d ndays] model-spec person-spec ...\n");
			exit(0);
		}

		daysToRun = atoi(argv[2]);
		positionInArray = 4;
		modelFileLocation = 3;

		// Ensure valid number of daysToRun.
		if (daysToRun <= 0) {
			LOG4CXX_ERROR(log, string("An invalid number of days was entered!"));
			daysToRun = DEFAULT_DAYS_TO_RUN;
		}

	// Case where days are not specified.
	} else {
		modelFileLocation = 1;
		positionInArray = 2;
	}


	/* Append person line to model-spec file for proper creation*/

	// Copy file so as not to mess up original.
	copyFile(argv[modelFileLocation], "copiedFile");

	fstream file;
	file.open("copiedFile", fstream::in | fstream::out | fstream::app);

	// Append person line to model-spec file.
	if (file.is_open())	{
		for(int i = positionInArray; i < argc; i++){
			file << "type Person label Person" << personCount << " filename " << argv[i] << endl;
			personCount++;
		}
	} else{
		LOG4CXX_ERROR(log, "File " << argv[modelFileLocation] << " was not found" <<
						"Simulator MUST be given a valid script to follow." <<
						"Please do so and try again.");
		exit(-1);
	}
	file.close();

	// Create house based on model-spec file.
	scheduler.registerHouse("copiedFile");

	LOG4CXX_INFO(log, "Will run simulaton for " << daysToRun << " days.\n\n");

	scheduler.run(daysToRun * 1440);

	LOG4CXX_INFO(log, "Simulation has successfully completed");
	return 0;
}

