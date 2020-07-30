
/*
Model Rocket Engine Tester
This example code uses bogde's excellent library: https://github.com/bogde/HX711
bogde's library is released under a GNU GENERAL PUBLIC LICENSE
Arduino pin 2 - HX711 Clock
pin 3 - Serial Data In
5V - VCC
GND - GND
*/
#include "HX711.h"
#define DOUT 3
#define CLK 2
#define IGNITE 4
#define CONTINUITY A0

int cont = 0;
float analogMax = 1023;

HX711 scale(DOUT, CLK);
void setup() {
    pinMode(IGNITE, OUTPUT);
    digitalWrite(IGNITE, LOW);

    Serial.begin(9600);
    scale.set_scale();
    Serial.println("Ready to zero scale, hit any key when ready");
    waitForSerial();
    scale.tare(); //Reset the scale to 0
    Serial.println("Scale zeroed, add calibration weight and enter weight in grams when ready:");
    while (Serial.available()==0)  {
    }
    float calibration_weight = Serial.parseFloat(); 
    float calibration_measure = scale.get_units(10); //Get a baseline reading
    float scalefactor = calibration_measure/calibration_weight;
    Serial.print("cal_weight: ");
    Serial.println(calibration_weight);
    Serial.print("cal_measure: ");
    Serial.println(calibration_measure);
    Serial.print("scale factor: ");
    Serial.println(scalefactor);
    scale.set_scale(scalefactor);
}

void loop() {
    cont = analogRead(CONTINUITY);
    Serial.print("Continuity reading is ");
    Serial.println(cont);
    Serial.print("Continuity is ");
    if (cont > analogMax/2) {
      Serial.println("GOOD");
    } 
    else {
      Serial.println("BAD");  
  }   
// If cont is bad, don't continue!  TBD
  
    Serial.println("Ready for motor test.  Remove calibration weight and replace with motor.  Hit any key to begin data collection.");
    Serial.println("Data collection will continue for ~10 seconds.");
    waitForSerial();
    for (int i=0; i <= 100; i++){ 
      //Serial.println(scale.get_value(1));
      Serial.print(millis());
      Serial.print("\t"); 
      Serial.println(scale.get_units());
    }
    digitalWrite(IGNITE, HIGH);
     for (int i=0; i <= 100; i++){ 
      //Serial.println(scale.get_value(1));
      Serial.print(millis());
      Serial.print("\t"); 
      Serial.println(scale.get_units());
    }
  digitalWrite(IGNITE, LOW);  
    for (int i=0; i <= 800; i++){ 
      //Serial.println(scale.get_value(1));
      Serial.print(millis());
      Serial.print("\t"); 
      Serial.println(scale.get_units());
    }
   
    Serial.println("Data collection complete.");
    while(1);      
}

void waitForSerial(){
  // First flush the buffer
  while(Serial.available()){Serial.read();}
//Wait for activity
  while (!Serial.available()) { }
//read what's there, print it, and exit
  Serial.println(Serial.read());
}
