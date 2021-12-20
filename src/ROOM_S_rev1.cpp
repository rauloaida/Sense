/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/ro/OneDrive/Documents/Firmware/Sense/src/ROOM_S_rev1.ino"
/*  
    Title: Fan Override function
    Description: TBD
    Author: Raul Oaida
    Date: Nov 22th 2021
    Last Checked: Dec 20th 2021
*/


void setup();
void loop();
#line 10 "c:/Users/ro/OneDrive/Documents/Firmware/Sense/src/ROOM_S_rev1.ino"
SYSTEM_THREAD(ENABLED);

//Includes
//#include "Particle.h"
#include "TSYS01.h" //for temperature sensor

//Defining Fan Override Variables
String fanValHigh = "025";
String fanValMed = "015";
String fanValLow = "010";
String setFanSpeed;
uint8_t value;
uint8_t value1;
bool fanOverride;

//PIR Stuff
const int PIN_TO_SENSOR = D22;   // the pin that OUTPUT pin of sensor is connected to
int pinStateCurrent   = LOW; // current state of pin
int pinStatePrevious  = LOW; // previous state of pin
const unsigned long DELAY_TIME_MS = 8900000; //set to 890000 for 14:50 min override timer, main room function.
bool delayEnabled = true; //PIR Delay
unsigned long delayStartTime;
bool lightsCondition1;
bool lightsCondition2;

//Variables for motion reporting to cloud
const unsigned long Motion_DELAY_TIME_MS = 180000; // 
bool Motion_delayEnabled = true;
bool Motion_setDelayExpired = false;
unsigned long Motion_delayStartTime;

//Particle Function Callouts
int enableOverride(String command);

//Temperature Sensor
TSYS01 sensor;
int reportTemp(String command);

void setup() {
  Serial.begin(9600);
  Particle.function("Fan Override", enableOverride);
  Particle.function("Report Temperature", reportTemp); 
  Serial1.begin(9600);// initialize UART to MR Logic PCBA
  pinMode(PIN_TO_SENSOR, INPUT_PULLDOWN); // set pinmode
  Wire.begin();
  sensor.init();
}

void loop()
{
  //EEPROM Read of fan enable and fan speed byes.
  uint8_t value = EEPROM.read(0);
  uint8_t value1 = EEPROM.read(1); 

//Checks the fan speed condition once and sets it to one of 3 pre-set FW values.
  if (value1 == 1) {
    setFanSpeed = fanValLow;
  } else if (value1 == 2){
    setFanSpeed = fanValMed;
  } else if (value1 == 3) {
    setFanSpeed = fanValHigh;
  }
  Serial.print(setFanSpeed);

  pinStatePrevious = pinStateCurrent; // store state
  pinStateCurrent =! digitalRead(PIN_TO_SENSOR);  // read new state

  if (pinStatePrevious == LOW && pinStateCurrent == HIGH && Motion_setDelayExpired == true) {   // pin state change: LOW -> HIGH
    Serial.println("Motion detected!");
    Particle.publish("M", "1", PRIVATE);
    Motion_delayEnabled = false; // disable delay
    Motion_setDelayExpired = false;

  }
  else if (pinStatePrevious == HIGH && pinStateCurrent == LOW) {   // pin state change: HIGH -> LOW
    Serial.println("Motion stopped!");
    Motion_delayEnabled = true; // enable delay
    Motion_delayStartTime = millis(); // set start time
    
  }
   else if (Motion_delayEnabled == true && (millis() - Motion_delayStartTime) >= Motion_DELAY_TIME_MS) {
    Serial.println("No motion for 180 seconds!");
    Particle.publish("M", "0", PRIVATE);
    Motion_delayEnabled = false; // disable delay
    Motion_setDelayExpired = true;
  }


//Override IF Statements
if (pinStatePrevious == LOW && pinStateCurrent == HIGH && value == 1) {   // pin state change: LOW -> HIGH
    Serial1.print("F"); 
    Serial1.print(setFanSpeed); 
    delayEnabled = false; // disable delay
    lightsCondition1 = true; 
  }
  else if (pinStatePrevious == HIGH && pinStateCurrent == LOW && value == 1) {   // pin state change: HIGH -> LOW
    delayEnabled = true; // enable delay
    lightsCondition1 = false;
    delayStartTime = millis(); // set start time
    
  }
  if (lightsCondition1 == true && lightsCondition2 == true && value == 1) {
    Serial1.print("M");
    lightsCondition1 = false;
    lightsCondition2 = false;
  };
  if (delayEnabled == true && (millis() - delayStartTime) >= DELAY_TIME_MS && value == 1) {
    Serial.println("No motion for 20 seconds! turning stuff off"); 
    Serial1.print("O");
    Serial1.print("F");
    Serial1.print("000");
    delayEnabled = false; // disable delay
    lightsCondition2 = true; 
  }
}

//--------------------------------------TEMPERATURE REPORTING CLOUD FUNCTION----------------------------------------//
int reportTemp(String command) {
  if(command == "report") {
  float temperatureReading;
  sensor.read();
  temperatureReading = sensor.temperature();
  Particle.publish("Temperature", String(temperatureReading), PRIVATE);
  return 1;
  } else return -1;
}


//--------------------------------------FAN OVERRIDE CLOUD FUNCTION----------------------------------------//
int enableOverride(String command) {
  if(command == "true")   //if cmd from cloud is true, permanently sets EEPROM byte on address 0 to 1;
  {
    Serial.print("fan override true");  
    int addr = 0;
    uint16_t value = 1;
    EEPROM.put(addr, value);
    Particle.publish("Fan Override", "true");

    return 1;
  } else if (command == "false") {  //if cmd from cloud is false, permanently sets EEPROM byte on address 0 to 0;
    Serial.print("fan override false");
    int addr = 0;
    uint16_t value = 0;
    EEPROM.put(addr, value);
    Particle.publish("Fan Override", "false");

    return 1;
  }

    else if(command == "low") { //if cmd from cloud is true, permanently sets EEPROM byte on address 0 to 1;
    Serial.print("fan speed low");  
    int addr = 1;
    uint16_t value = 1;
    EEPROM.put(addr, value);
    Particle.publish("Fan Speed", "Low");
    return 1;

  } else if (command == "medium") {  //if cmd from cloud is false, permanently sets EEPROM byte on address 0 to 0;
    Serial.print("fan speed medium");
    int addr = 1;
    uint16_t value = 2;
    EEPROM.put(addr, value);
    Particle.publish("Fan Speed", "Medium");
    return 1;

  } else if (command == "high") {  //if cmd from cloud is false, permanently sets EEPROM byte on address 0 to 0;
    Serial.print("fan speed high");
    int addr = 1;
    uint16_t value = 3;
    EEPROM.put(addr, value);
    Particle.publish("Fan Speed", "High");
    return 1;

  }

  
  else return -1;
}
