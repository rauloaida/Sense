/*  
    Title: Test Firmware for ROOM S PCBA QA function
    Description: Base Firmware + current measuring QA feature
    Author: Raul Oaida
    Date: Sept 19th 2021
*/


//Just adding a comment as another test! 

SYSTEM_THREAD(ENABLED); //uncomment to run all FW at startup without waiting for LTE connection to be estabished

const int PIN_TO_SENSOR = D2;   // the pin that OUTPUT pin of sensor is connected to
int pinStateCurrent   = LOW; // current state of pin
int pinStatePrevious  = LOW; // previous state of pin
const unsigned long DELAY_TIME_MS = 180000; // 
bool delayEnabled = true;
bool setDelayExpired = false;
unsigned long delayStartTime;
int report(String command);

void setup() {
  Serial.begin(9600);  
  pinMode(PIN_TO_SENSOR, INPUT_PULLUP); // set arduino pin to input mode to read value from OUTPUT pin of sensor
  setDelayExpired = true;
  Particle.function("report", report);
}

void loop() {
  pinStatePrevious = pinStateCurrent; // store state
  pinStateCurrent =! digitalRead(PIN_TO_SENSOR);   // read new state

  if (pinStatePrevious == LOW && pinStateCurrent == HIGH && setDelayExpired == true) {   // pin state change: LOW -> HIGH
    Serial.println("Motion detected!");
    Particle.publish("M", "1", PRIVATE);
    delayEnabled = false; // disable delay
    setDelayExpired = false;
  }
  else if (pinStatePrevious == HIGH && pinStateCurrent == LOW) {   // pin state change: HIGH -> LOW
    Serial.println("Motion stopped!");
    delayEnabled = true; // enable delay
    delayStartTime = millis(); // set start time
    
  }
   else if (delayEnabled == true && (millis() - delayStartTime) >= DELAY_TIME_MS) {
    Serial.println("No motion for 180 seconds!");
    Particle.publish("M", "0", PRIVATE);
    delayEnabled = false; // disable delay
    setDelayExpired = true;
  }   
}


//The chunk below deals with reporting current measurements to Particle Cloud for QA purpouses. 
int report(String command)
{
  if(command == "report")
  {
      Serial1.begin(9600);
      Serial1.write("P");
      delay(10000);
      unsigned int val1 = analogRead(A3);
      unsigned int val2 = analogRead(A4);
      unsigned int val3 = analogRead(A5);
      unsigned int val4 = analogRead(A0);
      unsigned int val5 = analogRead(A1);
      Particle.publish("Ambient_1", String(val1), PRIVATE); //Ambient1 current
      delay(500);
      Particle.publish("Ambient_2", String(val2), PRIVATE); //Ambient2 current
      delay(500);
      Particle.publish("Spotlight_1", String(val3), PRIVATE); //Spot1 current
      delay(500);
      Particle.publish("Spotlight_2", String(val4), PRIVATE); //Spot 2 current -- change to A6 for BSOM
      delay(500);
      Particle.publish("Fan", String(val5), PRIVATE); //Fan current -- change to A7 for BSOM
      delay(500);
      return 1;
  }
  else return -1;
} 