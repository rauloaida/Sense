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
  // register the cloud function
  Serial.begin(9600);
  Particle.function("Fan Override", Test);
}

void loop()
{
  uint8_t value = EEPROM.read(0);
  uint8_t value1 = EEPROM.read(1); //reads the EEPROM byte on address 0 and publishes it to cloud. 
  Particle.publish("Fan Override State", String(value));
  Particle.publish("Fan Speed", String(value1));
  delay(2000);
}




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

