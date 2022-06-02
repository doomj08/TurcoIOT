#include <Arduino.h>
#include "DHT.h"
#include <WiFi.h>
#include <ESP_Mail_Client.h>
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
#include <PirSensor.h>



DHT dht;
#define WIFI_SSID "ALFRED 2G"
#define WIFI_PASSWORD "1125658411"

#define SECRET_CH_ID 1756985			// replace 0000000 with your channel number
#define SECRET_WRITE_APIKEY "FIK2XHGA6W4WPYSN" 

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

WiFiClient espClient;

//************************
//** P I N S   U T I L I Z A D O S ***
//************************
#define PINWIFI 2
#define DHTPIN 4
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


PirSensor motion1 = PirSensor(PIR1, 2, false, false);
PirSensor motion2 = PirSensor(PIR2, 2, false, false);
//#define DHTTYPE DHT11

//************************
//** F U N C I O N E S ***
//************************
void setup_wifi();
void reconnect();
void publicador_think(int,int);
void personas_pir();

int cont2=0;
int cont_think=0;
void setup(){
  motion1.begin();
  motion2.begin();
  
  // put your setup code here, to run once:
  pinMode(PINWIFI,OUTPUT);
  Serial.begin(9600);
  Serial.println("Conectando a internet");
  setup_wifi();
    ThingSpeak.begin(espClient);  // Initialize ThingSpeak

  dht.setup(DHTPIN); // data pin 2
}

void loop(){
  digitalWrite(PINWIFI,(WiFi.status() == WL_CONNECTED));
  if (WiFi.status() != WL_CONNECTED) {
    setup_wifi();
  }
  personas_pir();
  // put your main code here, to run repeatedly:
  delay(dht.getMinimumSamplingPeriod());
  Serial.print("Humedad relativa");
  Serial.print(dht.getHumidity());
  Serial.println("Temperatura");
  Serial.print(dht.getTemperature());
  Serial.print("\t");
  cont2++;
  publicador_think(1,cont2);
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

void publicador_think(int input,int val){
  if(cont_think>9){
    cont_think=0;
  int x = ThingSpeak.writeField(myChannelNumber, input, val, myWriteAPIKey);
    if(x == 200){
      Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
  }else{
    cont_think++;
  }  
}

void personas_pir(){
    
        int motionStateChange1 = motion1.sampleValue();
        int motionStateChange2 = motion2.sampleValue();
        if (motionStateChange1 >= 0) {
            Serial.println(motionStateChange1);
        }
        if (motionStateChange2 >= 0) {
            Serial.println(motionStateChange2);
        }
}