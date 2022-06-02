#include <Arduino.h>
#include "DHT.h"
#include <WiFi.h>
#include <ESP_Mail_Client.h>

DHT dht;
#define WIFI_SSID "REVI AUTOS CLARO"
#define WIFI_PASSWORD "Reviautos"

//************************
//** F U N C I O N E S ***
//************************
void setup_wifi();

void setup(){
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Conectando a internet");
  setup_wifi();

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

void setup_wifi(){
  delay(5000);
  // Nos conectamos a nuestra red Wifi
  Serial.println();
  Serial.print("Conectando a ssid: ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  int cont=10;
  while (WiFi.status() != WL_CONNECTED) {
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

