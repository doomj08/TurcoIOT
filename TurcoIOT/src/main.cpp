#include <Arduino.h>
#include "DHT.h"
#include <WiFi.h>
#include <ESP_Mail_Client.h>
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros


DHT dht;
#define WIFI_SSID "TecniRTM_CDAReviautos"
#define WIFI_PASSWORD "T3cn1RTM"

#define SECRET_CH_ID 1756985			// replace 0000000 with your channel number
#define SECRET_WRITE_APIKEY "55U8BRZPPFWJC5WC" 

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

WiFiClient espClient;

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
void reconnect();
void publicador_think(int,int);

int cont2=0;
void setup(){
  
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
    int x = ThingSpeak.writeField(myChannelNumber, input, val, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}