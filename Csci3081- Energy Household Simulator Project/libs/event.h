#include <string>

/// Contains additional information for randomDelayEvent
typedef struct randomDelayEvent {
	int minimum; /// The minimum number of ticks to delay.
	int maximum; /// The maximum number of ticks to delay.
	int decision; /// Stores the randomly generated tick it will wait for.
	int timeSinceEventStart; /// Ticks passed since the event began running.

} randomDelayEvent_t;

/// Contains additional information for the activateEvent
typedef struct activateEvent {

	int numberOfArguments; /// The number of arguments to be passed to activate.
	string modelName; /// The name of the model to activate.
	double argument1; /// The first argument.
	double argument2; /// The second argument.

} activateEvent_t;


/// Contains additional information for the Choice event.
typedef struct choiceEvent {
	double probabilityOfFirstChoice;
	string alternateEvent;

} choiceEvent_t;

/// Contains additional information for the delayEvent.
typedef struct delayEvent {
	int delayLength;
	int timeSinceEventStart; /// Ticks passed since the event began running.

} delayEvent_t;

/// Contains additional information for the random time event.
typedef struct randomTimeEvent{
	int minimumTime; /// The minimum time to wait until (in ticks).
	int maximumTime; /// The maximum time tow ait until (in ticks).
	int decision; /// Stores the randomly generated tick it will wait for.

} randomTimeEvent_t;

/// Defines information about an event (one line of the script.)
typedef struct event{
	vector<string> parts; /// The parts from which this event was created.
	string eventName; /// Name of the current event.
	string nextEvent; /// Name of the next event to run once completed.
	string type; /// The current event's type.

	randomTimeEvent_t randomTimeEvent;
	delayEvent_t delayEvent;
	choiceEvent_t choiceEvent;
	activateEvent_t activateEvent;
	randomDelayEvent_t randomDelayEvent;

} event_t;
