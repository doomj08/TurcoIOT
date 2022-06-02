#include <Arduino.h>
#include "DHT.h"
#include <WiFi.h>
#include <ESP_Mail_Client.h>
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros


DHT dht;
#define WIFI_SSID "TecniRTM_CDAReviautos"
#define WIFI_PASSWORD "T3cn1RTM"

//************************
//** P I N S   U T I L I Z A D O S ***
//************************
#define PINWIFI 2
#define DHTPIN 12
#define PIR1 14
#define PIR2 27
#define ESTRACTOR1 26
#define ESTRACTOR2 0
#define VENTILADOR1 25
#define VENTILADOR2 27
#define FLUJO1 33
#define FLUJO2 32
#define FOGON1 35
#define FOGON2 34
//#define DHTTYPE DHT11




//************************
//** F U N C I O N E S ***
//************************
void setup_wifi();

void setup(){
  // put your setup code here, to run once:
  pinMode(PINWIFI,OUTPUT);
  Serial.begin(9600);
  Serial.println("Conectando a internet");
  setup_wifi();

  dht.setup(DHTPIN); // data pin 2
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

void led_wifi(){
  
}

void setup_wifi(){
  delay(5000);
  bool con =false;
  // Nos conectamos a nuestra red Wifi
  Serial.println();
  Serial.print("Conectando a ssid: ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  int cont=10;
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(PINWIFI,con);
    con=!con;
    if (cont>10)
    {
      cont=0;
      Serial.println("");  
      Serial.print("Conectando");
    }
    cont++;
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conectado a red WiFi!");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

