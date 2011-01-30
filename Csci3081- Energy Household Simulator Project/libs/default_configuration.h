#ifndef DEFAULT_CONFIGURATION_H_
#define DEFAULT_CONFIGURATION_H_

/******************************************************************************
 *                        Computer default values
 *****************************************************************************/

const int COMPUTER_ON_POWER = 500;
const int COMPUTER_OFF_POWER = 100;
const int COMPUTER_STATE = OFF;
const string COMPUTER_LABEL = "Computer";

/******************************************************************************
 *                       Dishwasher values
 ******************************************************************************/

const int MAX_TEMP = 51;
const int DISHWASHER_OFF_POWER = 4;
const int DISHWASHER_ON_POWER = 2000;
const int DISHWASHER_ON_PERIOD = 45;
const int DISHWASHER_FIRST_CYCLE_LENGTH = 20;
const int DISHWASHER_WATER_DRAW = 22;
const int DISHWASHER_STATE = OFF;
const string DISHWASHER_LABEL = "Dishwasher";
const string DISHWASHER_ASSOCIATED_WATERHEATER = "Waterheater";

/*******************************************************************************
 *                         Lights values
 ******************************************************************************/

const int LIGHTS_ON_POWER = 1;
const int LIGHTS_OFF_POWER = 0;
const int LIGHTS_STATE = OFF;
const int LIGHTS_ACTIVATED = 0;
const int LIGHTS_NUMBER_OF = 45;
const string LIGHTS_LABEL = "Lights";

/******************************************************************************
 *                        Microwave values
 ******************************************************************************/

const int MICROWAVE_POWER = 0;
const int MICROWAVE_OFF_POWER = 0;
const int MICROWAVE_ON_POWER = 1500;
const int MICROWAVE_STATE = OFF;
const string MICROWAVE_LABEL = "Microwave";

/******************************************************************************
 *                          Oven values
 ******************************************************************************/

const int OVEN_PRE_HEAT_TIME = 10;
const int OVEN_PRE_HEAT_POWER = 7500;
const int OVEN_COOK_POWER = 2500;
const int OVEN_OFF_POWER = 0;
const int OVEN_STATE = OFF;
const string OVEN_LABEL = "Oven";

/******************************************************************************
 *                          Refrigerator values
 ******************************************************************************/

const double REFRIGERATOR_ACTIVE_DUTY_CYCLE = 0.5;
const double REFRIGERATOR_INACTIVE_DUTY_CYCLE = 0.3;
const int REFRIGERATOR_ON_PERIOD = 3;
const int REFRIGERATOR_ACTIVE_ON_PERIOD = 3;
const int REFRIGERATOR_ON_POWER = 300;
const int REFRIGERATOR_OFF_POWER = 0;
const string REFRIGERATOR_LABEL = "Refrigerator";


/******************************************************************************
 *                          Person values
 ******************************************************************************/

const int PERSON_DAYS_SINCE_DISHWASHER_RAN = 0;

/******************************************************************************
 *                         Shower values
 ******************************************************************************/

const string SHOWER_LABEL = "Shower";
const int SHOWER_WATER_DRAW = 8;
const int SHOWER_WATER_TEMPERATURE = 40;

/******************************************************************************
 *                         Stove values
 ******************************************************************************/

const int STOVE_NUMBER_OF_BURNERS = 4;
const int STOVE_OFF_POWER_COST = 0;
const int STOVE_LOW_POWER_COST = 350;
const int STOVE_MEDIUM_POWER_COST = 700;
const int STOVE_HIGH_POWER_COST = 1400;
const string STOVE_LABEL = "Stove";

/******************************************************************************
 *                        Television Values
 ******************************************************************************/

const int TELEVISION_OFF_POWER = 25;
const int TELEVISION_ON_POWER = 200;
const int TELEVISION_SIZE = 27;
const string TELEVISION_LABEL = "Television";

/******************************************************************************
 *                           Toaster Values
 ******************************************************************************/

const string TOASTER_LABEL = "Toaster";
const int TOASTER_ON_POWER = 450;
const int TOASTER_OFF_POWER = 0;
const int TOASTER_COOK_TIME = 2;

/******************************************************************************
 *                          WaterHeater Values
 *****************************************************************************/

const string WATERHEATER_LABEL = "Waterheater";
const int WATERHEATER_ON_POWER = 4500;
const int WATERHEATER_OFF_POWER = 0;
const int WATERHEATER_MAX_LITERS = 200;
const int WATERHEATER_HIGH_TEMP_CUTOFF = 50;
const int WATERHEATER_MIN_TEMP_CUTOFF = 49;
const int WATERHEATER_INCOMING_TEMPERATURE = 7;
const int WATERHEATER_SPECIFIC_HEAT = 4132;
const double WATERHEATER_HEAT_LOSS_PER_TICK =  0.0029;
#endif /* DEFAULT_CONFIGURATION_H_ */
