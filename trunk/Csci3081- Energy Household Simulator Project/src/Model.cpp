#include "../libs/Model.h"
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
using namespace std;

///	Default constructor for the Model class.
Model::Model() {
}

///	Destructor
Model::~Model() {

}

///	A virtual method that should be overriden by a child class.
///	It describes the behavior of a device within one minute.
void Model::tick() {

}

///	A virtual method that should be overriden by a child class.
///	It sets the state of the model to activated.
void Model::activate() {

}

/// The virtual activation model used by the WaterHeater to extract water.
void Model::activate(double, double){

}

// The virtual activation model used by the stove to light burners.
void Model::activate(int, int){

}

// The virtual activation model used by microwave testing.
void Model::activate(double){

}

///	Returns the instantaneous power usage of this Model.
int Model::getPower()
{
	return power_;
}

///	Returns the total energy usage of this Model since it was created.
int Model::getEnergy()
{
	return totalCost_;
}

string Model::getLabel()
{
	return label_;
}
