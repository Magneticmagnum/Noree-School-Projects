/**
 * @class Person
 * @brief A person who goes about his/her day following a script
 *
 * This is a person model that simulates a typical person that will go about
 * the day according to a script supplied by the user.
 *
 * <p>Person can understand the following variables in its model-spec file: </p>
 * <ul><li><b>label</b> (default: Microwave) -- This changes the name
 * of the person. Example: <i>label John</i></li>
 *
 * <li> <b>startState</b> (default: <i> first specified state </i>) --
 * This changes the state in which the person will start. If none is specified,
 * it will be set to the first state specified in the script.
 * Example: <i>startState reading</i></li>
 *
 * <li> <b>fileName</b> (default: <i>none</i>) -- This changes the location of
 * the script which the person will follow. Example: <i>fileName Bart-Script</i></li><ul>
 *
 * <h2> About The Script </h2>

<p>There are 6 different states that can be specified in a person's script:</p>
<ol><li> <h2><p>Delay</p></h2>

<b><p>Pattern:</b> name Delay nextState timeToDelayBy</p>

<p><b>Info:</b>While in this state, the person will wait for the specified number of ticks
from when he first entered the state.</p>

<p><b>Example:</b>   waitForWaterToBoil Delay putInPasta 10</p></li>

<li><h2><p>RandomTime</p></h2>

<b><p>Pattern:</b> name RandomTime nextState minimum maximum</p>

<p><b>Info:</b>In this state, the person will wait for a random time between 'minimum' and
'maximum' before moving onto the next state. Use military-style time input.</p>

<p><b>Example:</b> waitForBus RandomTime rideBus 17:30 17:40<p></li>


<li><h2><p>Choice</p></h2>

<b><p>Pattern:</b> name Choice firstChoice secondChoice probabilityOfFirstChoice</p>

<p><b>Info:</b>At this state, the person will "choose" between two possible states to enter
into next.  The probability of the first choice being taken should be stated
in the fifth space "probabilityOfFirstChoice." It should be stated in the form
of a decimal number between 0 (no chance of the first choice being chosen) and
1 ( a 100% chance of first choice being taken.  If the first choice is not
taken, then the second choice will be.</p>

<p><b>Example:</b>  pickADoor doorNumberOne doorNumberTwo 0.5</p></li>



<li><h2><p>Activate</p></h2>

<b><p>Pattern:</b> name nextState modelName numberOfArguments Arguments ...</p>

<p><b>Info</b>Here the person will activate the model of the name modelName with a certain
number of arguments.  If the modelName is incorrect then the program will
continue to run and a log will be made indicating the problem.</p>

<p>The following is a list of all models and their activation arguments.</p>

<h3>Zero-Argument:</h3><ul>
    <li> Refrigerator -- Will toggle between cooking and not cooking modes.</li>
     <li> Shower       -- Will turn the shower on, if off and off, if on.</li>
     <li> Dishwasher   -- Will turn the dishwasher on.</li>
     <li> Microwave    -- if zero arguments are given to the microwave then it will
                     run for a random amount of time between 40 and 120 seconds.</li>
     <li> Computer     -- Will turn on or off a computer.</li>
     <li> Television   -- Will turn on or off a television</li>
     </li> Toaster      -- Will turn on the toaster.</li></ul>

<h3>One-Argument:</h3><ul>
     <li>Oven         -- Will run the oven for the specified amount of time plus an
                     additional ten minutes for pre-heating.</li>
     <li>Microwave    -- The microwave can also be given a time for which to run.
                     The time can take the form a decimal number (e.g. 2.5
                     corresponds to 2 minutes and 30 seconds (0.5 of a minute).</li></ul>

<h3>Two-Argument:</h3><ul>
    <li>Lights (numLights, state)   -- Will turn apply the specified state (ON, OFF)
                                    to the specified number of lights.</li>
     <li>Stove  (powerLevel, length) -- Will turn on a burner (if available) at the
                                    given powerLevel (LOW, MEDIUM, HIGH) for the
                                    specified amount of time.</li></ul>

<p>NOTE: do NOT attempt to activate a model with an incorrect number of arguments.
      If you do so, no warning will be given and nothing will be activated.</p>

<p><b>Examples:</b>  powerOnComputer Activate checkEmail ThinkPad 0</p>
<p>         bakeCookies Activate maybeLetCool KenmoreOven 1 20</p>
<p>        cookEggs Activate eatEggs ElectricRange 2 HIGH 5</p></li>


<li><h2><p>RandomDelay</p></h2>

<p><b>Pattern:</b> name RandomTime nextState minimum maximum</p>

<p><b>Info</b>In this state, the person will wait for a random number of ticks between
'minimum' and 'maximum' before moving onto the next state.</p>

<p><b>Example:</b>    getReadyForBed RandomTime sleep rideBus 10 15</p></li>


<li><h2><p>Dishes</p></h2>

<p><b>Pattern:</b> name Dishes nextState</p>

<p><b>Info</b>In this state, if  it has been three days since the dishwasher has been last
run, then the dishwasher will be run by the person in this state.</p>

<p><b>Example:</b>  checkDishwasher Dishes watchTV</p></li>
</ol>

<h2>Special Considerations:</h2>
<ol>
<li>Each state's name must be unique.</li>
<li>You cannot use tabs in either the person script file or the model spec file</li>
<li>There cannot be spaces in the path of the model spec or person spec files.</li>
<li>If multiple people are living within the same house, it is assumed that they
are friendly to each other and will plan their days around the other person
(e.g. they won't try and use the same device the other one is using).  Having
such a resource conflict situation in this simulation, as in real-life, may
cause unusual or unintended behavior and is not optimal.</li>
</ol>
 */

#ifndef PERSON_H_
#define PERSON_H_

class House; /// Required to resolve declaration issues.

#include "../libs/Person.h"
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <fstream>

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
#include "../libs/event.h"

#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <ostream>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <ostream>
#include <map>

using namespace std;

class Person: public Model {

	friend class PersonTest; /// Required to properly test person.

public:

	Person(House&, map<string, string> configuration);

	/**
	 * Constructor that is dependent on House and particular
	 * events that happened that day
	 * to interact with the models in the simulator.
	 */
	Person(House&, char*, string);

	/**
	 * Constructor that is dependent on House to interact
	 * with the models in the simulator.
	 */
	Person(House&);

	/**
	 * Destructor
	 */
	~Person();

	/**
	 * Informs the person that one tick has passed. The person will then
	 * perform different acts based on its current state.
	 */
	virtual void tick();

	/// Returns the current time.
	int getTime();



	/// Returns the state that the person is currently in.
	string getState();

protected:

	/// Given a filename, will convert the contents into a map of Events.
	map <string, event> planDay(string fileName);

	/// The name of the state the person is currently in.
	string state_;

	/// The number of ticks that have passed in the current day.
	int todaysCounter_;

	/// The number of ticks that have passed since the beginning of the simulation.
	int totalCounter_;

	/// The house under which the person is registered.
	House *house_;

	/// Signals that even though the person is invalid, attempt to continue.
	bool invalid_;

	/// The filename of the script
	string filename_;

	/**
	 * A map between state names and their respective events. Together they
	 * make up the person's script.
	 */
	map <string, event>  today_;

	/// Based on information contained on a line, generates an event.
	event_t parseLine(string line);

	/// Using the given vector of strings, will create a randomTimeEvent.
	event createRandomTimeEvent(vector<string> parts);

	/// Using the given vector of strings, will create a delayEvent.
	event createDelayEvent(vector<string> parts);

	/// Using the given vector of strings, will create a choiceEvent.
	event createChoiceEvent(vector<string> parts);

	/// Using the given vector of strings, will create an activateEvent.
	event createActivateEvent(vector<string> parts);

	/// Using the given vector of strings, will create a randomDelayEvent.
	event createRandomDelayEvent(vector<string> parts);

	/// Using the given vector of strings, will create a dishwasher event.
	event createDishesEvent(vector<string> parts);

	/// Resets the given event back to its start state. Returns the next event.
	string resetEvent(event_t event);

	/**
	 * Will change the Person's actions depending on the state it is currently
	 * in.  It will return the name of the state the person should be in for
	 * the next tick
	 */
	string handleState(event_t&);

	/**
	 * Attempts to apply the user specified values for this model.  If a
	 * particular variable is not given by the user or is invalid, then the
	 * default value will be used. In addition, if certain specified values
	 * cannot be applied, the user will be notified.
	 */
	virtual void applyConfiguration(map<string, string> configuration);

private:
	Person();
};

#endif
