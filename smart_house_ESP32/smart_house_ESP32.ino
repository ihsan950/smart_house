#define BLYNK_TEMPLATE_ID "TMPLzgMN4Fz8"
#define BLYNK_DEVICE_NAME "Data Monitoring with Automation"
#define BLYNK_AUTH_TOKEN "0QTeAYVnZ1TMjyclvL9k1gODK88Ya2-m"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
//***************servo*******************************
#include <Servo.h>
Servo myservo;
Servo tankservo;
int pos = 0;
int pos2 = 0;
//**********************************************
#include "DHT.h"
//***************//led//************************
#include <FastLED.h> 
#define COLOR_ORDER GRB
#define LED_TYPE    WS2812
#define NUM_LEDS1 4
CRGB leds1[NUM_LEDS1];
#define PIN1 14
int data=255;
int r,g,b;
//**************GAS*************************
int gas = 12;
//***************Rain***************************
int analogRainPin = 27;
//********************************************
char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "";
char pass[] = "";

BlynkTimer timer;
//***********temp*******************
#define DHT11PIN 26
#define DHTTYP DHT11
DHT dht;
//*************solar***********************
int solarpin = 13;
//************waterLvL*********************
#define WATER_PIN 15
int watervalue = 0;
//******************************************
#define sensor_w 25
int val = 0;


BLYNK_WRITE(V3)//LED Yellow
{
  int pinvalue=param.asInt();
  digitalWrite(19,pinvalue);
}

BLYNK_WRITE(V4)//Window Servo
{
  int pos=param.asInt();
   myservo.write(pos);
   
}

BLYNK_WRITE(V5)//LED 5050 2x2
{
  r = param[0].asInt();
  g = param[1].asInt();
  b = param[2].asInt();
 
  static1(r, g, b,data);
}

BLYNK_WRITE(V6)//Brightness
{
data = param.asInt(); 
static1(r, g, b,data);
}

BLYNK_WRITE(V8)//Window Servo
{
  int pos=param.asInt();
   tankservo.write(pos);
   
}

void getSensorData()
{

}



void sendSensor() //store data from sensors to vertiual pins
{
  float h = dht.getHumidity();
  float t = dht.getTemperature();
  float BatteryValue = analogRead(solarpin);
  float potvalue= (float(BatteryValue)*5)/4095*2;
  float rain = analogRead(analogRainPin);
  float watervalue = analogRead(WATER_PIN);
   if (isnan(h)|| isnan(t)){
   Serial.println("Faild to read from DHT sensor");


   
   return;
 }
  


  Blynk.virtualWrite(V0,t);
  Blynk.virtualWrite(V1,h);
  Blynk.virtualWrite(V2, potvalue);
  Blynk.virtualWrite(V7,rain);
  Serial.print("Temperature : ");
  Serial.print(t);
  Serial.print("Humidity : ");
  Serial.println(h);

}

void static1(int r, int g, int b,int brightness)
{
  FastLED.setBrightness(brightness);
  for (int i = 0; i < NUM_LEDS1; i++ )
  {
    leds1[i] = CRGB(r, g, b);
  }
  FastLED.show();
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(15,OUTPUT);
  pinMode(sensor_w, INPUT);
  myservo.attach(18);
  tankservo.attach(5);

  pinMode(gas,INPUT);
  


  
  Blynk.begin(auth,ssid,pass);
  dht.setup(DHT11PIN);
  FastLED.addLeds<LED_TYPE, PIN1, COLOR_ORDER>(leds1, NUM_LEDS1).setCorrection( TypicalLEDStrip );
  timer.setInterval(100L,sendSensor);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = analogRead(sensor_w);
  getSensorData();
  Blynk.virtualWrite(V10, val);
  Blynk.run();
  timer.run();
  int potvalue=analogRead(gas);
  Blynk.virtualWrite(V9,potvalue);
  

  
  
  

}
