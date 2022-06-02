#include <Arduino.h>
#include "DHT.h"

DHT dht;

void setup(){
  // put your setup code here, to run once:
  Serial.begin(9600);

  dht.setup(2); // data pin 2
}

void loop(){
  // put your main code here, to run repeatedly:
  delay(dht.getMinimumSamplingPeriod());
  Serial.print("Humedad relativa");
  Serial.print(dht.getHumidity());
  Serial.println("Temperatura");
  Serial.print(dht.getTemperature());
  Serial.print("\t");
}

