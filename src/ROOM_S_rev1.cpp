/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/rauloaida/Documents/GitHub/Sense/src/ROOM_S_rev1.ino"
/*  
    Title: Fan Override function
    Description: TBD
    Author: Raul Oaida
    Date: Nov 19th 2021
*/


void setup();
void loop();
#line 9 "/Users/rauloaida/Documents/GitHub/Sense/src/ROOM_S_rev1.ino"
SYSTEM_THREAD(ENABLED);

const int PIN_TO_SENSOR = D22;   // the pin that OUTPUT pin of sensor is connected to
int pinStateCurrent   = LOW; // current state of pin
int pinStatePrevious  = LOW; // previous state of pin
const unsigned long DELAY_TIME_MS = 10000; // 890000...30000 miliseconds ~ 30 seconds was 890000
bool delayEnabled = true;
unsigned long delayStartTime;
bool lightsCondition1;
bool lightsCondition2;


//Defining Fan Override Variables
String fanValHigh = "028";
String fanValMed = "015";
String fanValLow = "010";
uint8_t value;
uint8_t value1;

//Particle Function Callouts
int Test(String command);



void setup()
{
  Serial.begin(9600);
  Particle.function("Fan Override", Test);
  Serial1.begin(9600);// initialize UART to MR Logic PCBA
  pinMode(PIN_TO_SENSOR, INPUT_PULLDOWN); // set pinmode
}

void loop()
{
  uint8_t value = EEPROM.read(0);
  uint8_t value1 = EEPROM.read(1); //reads the EEPROM byte on address 0 and publishes it to cloud. 
  //Particle.publish("Fan Override State", String(value));
  //Particle.publish("Fan Speed", String(value1));

if (value == 0){
  Particle.publish("Testing stuff!", "running first loop bc fan override is 0");
  pinStateCurrent =! digitalRead(PIN_TO_SENSOR);   // read new state
  Serial.print(pinStateCurrent);

}

if (value == 1){
  Particle.publish("Testing stuff!", "running 2nd loop, value is 1");
  pinStatePrevious = pinStateCurrent; // store state
  pinStateCurrent =! digitalRead(PIN_TO_SENSOR);   // read new state
  Serial.print(pinStateCurrent);

  if (pinStatePrevious == LOW && pinStateCurrent == HIGH) {   // pin state change: LOW -> HIGH
    Serial.println("Motion detected!");
    Serial1.print("F");
    Serial1.print("030");
    Serial.println("Turned fans to 30%");
    delayEnabled = false; // disable delay
    lightsCondition1 = true;
    Serial.println("Motion detected! lightsCondition_1 set to true");
    // TODO: turn on alarm, light or activate a device ... here
  }
  else
  if (pinStatePrevious == HIGH && pinStateCurrent == LOW) {   // pin state change: HIGH -> LOW
    Serial.println("Motion stopped! lightCondition_1 set to false");
    delayEnabled = true; // enable delay
    lightsCondition1 = false;
    delayStartTime = millis(); // set start time
    
  }
  if (lightsCondition1 == true && lightsCondition2 == true) {
    Serial1.print("M");
    Serial.println("both light conditions true, set lights to M mode");
    lightsCondition1 = false;
    lightsCondition2 = false;
    Serial.println("lightsCondition_1 reset to false");
    Serial.println("lightsCondition_2 reset to false");
  };
  if (delayEnabled == true && (millis() - delayStartTime) >= DELAY_TIME_MS) {
    Serial.println("No motion for 15 min! turning stuff off");
    Serial1.print("O");
    Serial.println("Set lights to OFF");
    Serial1.print("F");
    Serial1.print("000");
    Serial.println("Fans set to 0%");
    delayEnabled = false; // disable delay
    lightsCondition2 = true;
    Serial.println("lightsCondition_2 set to true");
    // TODO: turn off alarm, light or deactivate a device ... here
  }


}


}





//--------------------------------------FAN OVERRIDE SECTION----------------------------------------//
int Test(String command)
{
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

