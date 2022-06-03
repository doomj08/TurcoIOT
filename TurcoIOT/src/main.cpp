#include <Arduino.h>
#include "DHT.h"
#include <WiFi.h>
#include <ESP_Mail_Client.h>
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros





DHT dht;
#define WIFI_SSID "UNISANGIL YOPAL"
#define WIFI_PASSWORD ""

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

//#define DHTTYPE DHT11

//************************
//** F U N C I O N E S ***
//************************
void setup_wifi();
void reconnect();
void publicador_think(int,int);
void personas_pir();
void control_flujo(int);
void control_fogones(int);
void controlar_temperatura(bool);
void controlar_humedad(bool);

int cant_personas=0;
int cont_think=0;
//************************************************
//** C O N D I C I O N E S   I N I C I A L E S ***
//************************************************
float temp_inicial=40;
float hum_inicial=70;

float temp_min=52;
float temp_max=55;

float hum_min=89;
float hum_max=95;
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
  personas_pir();
  // put your main code here, to run repeatedly:
  
  publicador_think(1,dht.getHumidity());
  publicador_think(2,dht.getTemperature());
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
  if(cont_think>20){
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
    /*
        int motionStateChange1 = motion1.sampleValue();
        int motionStateChange2 = motion2.sampleValue();
        if (motionStateChange1 >= 0) {
            Serial.println(motionStateChange1);
        }
        if (motionStateChange2 >= 0) {
            Serial.println(motionStateChange2);
        }
        */
        controlar_temperatura(cant_personas=0);
        controlar_humedad(cant_personas=0);
}

void controlar_temperatura(bool en_uso){
  float check_temp;
  int temp_leida=dht.getTemperature();

  if(en_uso)
    check_temp=temp_min;
  else
    check_temp=temp_inicial;

    if(temp_leida<check_temp){
      control_fogones(2);
    }
    else{
      control_fogones(1);
    }  
}

void controlar_humedad(bool en_uso){
  float check_hum;
  int hum_leida=dht.getHumidity();

  if(en_uso)
    check_hum=temp_min;
  else
    check_hum=temp_inicial;

    if(hum_leida<check_hum){
      control_flujo(2);
      control_fogones(1);
    }
    else{
      control_flujo(1);
    }  
}

void control_fogones(int cantidad){
  if(cantidad=0){
    digitalWrite(FOGON1,LOW);
    digitalWrite(FOGON2,LOW);
  }
  else if(cantidad=1){
    digitalWrite(FOGON1,HIGH);
    digitalWrite(FOGON2,OUTPUT);
  }
  else{
    digitalWrite(FOGON1,HIGH);
    digitalWrite(FOGON2,HIGH);
  }    
}

  void control_flujo(int cantidad){
  if(cantidad=0){
    digitalWrite(FLUJO1,LOW);
    digitalWrite(FLUJO1,LOW);
  }
  else if(cantidad=1){
    digitalWrite(FLUJO1,HIGH);
    digitalWrite(FLUJO1,OUTPUT);
  }
  else{
    digitalWrite(FLUJO1,HIGH);
    digitalWrite(FLUJO1,HIGH);
  }
}

void old(){
      delay(dht.getMinimumSamplingPeriod());
    Serial.println("Humedad relativa");
    Serial.print(dht.getHumidity());
    Serial.println("Temperatura");
    Serial.print(dht.getTemperature());
    Serial.print("\t");
}