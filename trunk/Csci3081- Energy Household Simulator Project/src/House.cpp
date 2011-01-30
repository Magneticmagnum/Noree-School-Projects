#include "../libs/House.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <map>

using namespace std;
using log4cxx::Logger;
using log4cxx::LoggerPtr;
using log4cxx::PropertyConfigurator;
using log4cxx::File;

int House::totalTime_;

LoggerPtr houseLog(Logger::getLogger("House"));

House::House() {
	numDevices_ = 0;
	daysSinceDishwasherRan_ = 0;
}

///	Constructor, set the number of registered model as 0
///	Set the number of ticks passed as 0
House::House(char* filePath) {

	LOG4CXX_INFO(houseLog, "House has been created.");
	numDevices_ = 0;
	totalTime_ = 0;
	daysSinceDishwasherRan_ = 0;
	vector < map<string, string> > configurations;
	int i;

	string line;
	ifstream file(filePath);

	// Open file and read it line by line
	if (file.is_open()) {

		while (file.good()) {
			string args;

			getline(file, line);

			if(line != "" && line != " " && line != "\n"){
				// Make sure the line isn't a comment
				if (line[0] != '#') {
					vector < string > vectored = vectorize(line, " ");

					// Simple check that ratio of variable to value is 1:1
					if (vectored.size() % 2 == 0) {
						createModel ( generateConfig(vectored));
					}
				}
			}
		}
	}
	file.close();
}

// Builds a config based on a given line.
map<string, string> House::generateConfig(vector<string> vectored) {
	map < string, string > configuration;

	vector<string>::iterator variableIterator = vectored.begin();
	vector<string>::iterator valueIterator = vectored.begin();
	valueIterator++;

	/// For all of the variables in the vector, match them to their values.
	while (variableIterator != vectored.end()) {
		configuration.insert(pair<string, string> (*(variableIterator),
				*(valueIterator)));
		variableIterator += 2;
		valueIterator += 2;
	}

	return configuration;
}

// Depending on type, create the model and register it.
void House::createModel(map<string, string> configuration) {
	string type = configuration["type"];
	configuration.erase("type");

	if (type == "Dishwasher") {
		Dishwasher* dishwasher = new Dishwasher(configuration, *(this));
		registerModel(dishwasher);

	} else if (type == "Toaster") {
		Toaster* toaster = new Toaster(configuration);
		registerModel(toaster);

	} else if (type == "WaterHeater" || type == "Waterheater") {
		WaterHeater* waterheater = new WaterHeater(configuration);
		registerModel(waterheater);

	} else if (type == "Computer") {
		Computer* computer = new Computer(configuration);
		registerModel(computer);

	} else if (type == "Lights") {
		Lights* lights = new Lights(configuration);
		registerModel(lights);

	} else if (type == "Microwave") {
		Microwave* microwave = new Microwave(configuration);
		registerModel(microwave);

	} else if (type == "Oven") {
		Oven* oven = new Oven(configuration);
		registerModel(oven);

	} else if (type == "Refrigerator") {
		Refrigerator* refrigerator = new Refrigerator(configuration);
		registerModel(refrigerator);

	} else if (type == "Shower") {
		Shower* shower = new Shower(configuration, *(this));
		registerModel(shower);

	} else if (type == "Stove") {
		Stove* stove = new Stove(configuration);
		registerModel(stove);

	} else if (type == "Television") {
		Television* television = new Television(configuration);
		registerModel(television);

	} else if (type == "Person") {
		Person* person = new Person(*(this), configuration);
		registerModel(person);

	} else {
		LOG4CXX_ERROR(houseLog, string("The type: ") << type << " is not a"
				"valid kind of model.  Consult the documentation");
	}
}

///	Destructor
House::~House() {
	LOG4CXX_INFO(houseLog, string("House has been deconstructed."));
}

/// Inform models that a tick has passed
void House::tick() {

	map<string, Model*>::iterator currentModel = models_.begin();

	// When there's a new day, increase the days since dishwasher was run.
	if (totalTime_ % 1440 == 0) {
		daysSinceDishwasherRan_++;
	}

	// Tick all of the models.
	while (currentModel != models_.end()) {
		currentModel->second->tick();
		currentModel++;
	}

	totalTime_++;
}

///	Put the registered model into a vector array by reference
bool House::registerModel(Model* m) {
	models_[m->getLabel()] = m;

	numDevices_++;
	return true;
}

void House::activateModel(string modelName, double arg1, double arg2) {
	Model* model = models_[modelName];

	/// Make sure the model exists before activating it.
	if (model != NULL) {
		model -> activate(arg1, arg2);
	} else {
		LOG4CXX_ERROR(houseLog, "Model " << modelName << " was not found:"
				<< " and, therefore, will not be executed");
	}
}

void House::activateModel(string modelName, double argument1) {
	Model* model = models_[modelName];

	/// Make sure the model exists before activating it.
	if (model != NULL) {
		models_[modelName] ->activate(argument1);
	} else {
		LOG4CXX_ERROR(houseLog, "Model " << modelName << " was not found:"
				<< " and, therefore, will not be executed");
	}
}

void House::activateModel(string modelName) {
	Model* model = models_[modelName];

	/// Make sure the model exists before activating it.
	if (model != NULL) {
		models_[modelName]->activate();
	} else {
		LOG4CXX_ERROR(houseLog, "Model " << modelName << " was not found:"
				<< " and, therefore, will not be executed");
	}
}

int House::getNumberDevices() {
	return numDevices_;
}

/// Returns the current power used by all models during simulation.
int House::getTotalPower() {
	int total_power = 0;

	map<string, Model*>::iterator currentModel = models_.begin();

	while (currentModel != models_.end()) {
		total_power += currentModel->second->getPower();
		currentModel++;
	}

	LOG4CXX_DEBUG(houseLog, string("The current power is ") << total_power);
	return total_power;
}

/// Returns the total energy used by all models during simulation.
int House::getTotalEnergy() {
	int total_energy = 0;

	map<string, Model*>::iterator currentModel = models_.begin();

	while (currentModel != models_.end()) {
		total_energy += currentModel->second->getEnergy();
		currentModel++;
	}

	LOG4CXX_DEBUG(houseLog, string("The total energy use so far is ")
			<< total_energy);
	return total_energy;
}

/// Used for debugging and testing only.
void House::run(int times) {
	for (int i = 0; i < times; i++)
		tick();
}

void House::cleanup(){
	map<string, Model*>::iterator currentModel = models_.begin();

	while (currentModel != models_.end()) {
		delete(currentModel->second);
		currentModel++;
	}
}

void House::dishwasherRan() {
	daysSinceDishwasherRan_ = 0;
}

int House::getDaysSinceDishwasherRan() {
	return daysSinceDishwasherRan_;
}
