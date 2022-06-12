#include <Arduino.h>
#include "SoftwareSerial.h"
#include "PMS.h"
#include <DHT.h>
#include <MQ135.h>
#include <WiFi.h>
#include <ThingsBoard.h>

//Access Point Details
#define WIFI_MODE_AP "HUAWEI-B253" 
#define WIFI_PASSWORD "pokemon1245"

//ThingsBoard Token
#define TOKEN "a45HaNR6Ya4AjQxF516m"

//Pin Assignment
#define PMS7003_TX 27
#define PMS7003_RX 26
#define DHTTYPE DHT22
#define DHTPIN 25
#define PIN_MQ135 32
#define PMS7003_BAUD 9600
char thingsboardServer[] = "thingsboard.cloud"; //Change according to your set-upp

//Sensor Initiation
WiFiClient wifiClient;
ThingsBoard tb(wifiClient);
DHT dht(DHTPIN, DHTTYPE);
MQ135 mq135_sensor(PIN_MQ135);
SoftwareSerial pms_ss(PMS7003_TX, PMS7003_RX);
PMS pms(pms_ss);
PMS::DATA pms_data;

int status = WL_IDLE_STATUS;
unsigned long lastSend;
int pm1;
int pm25;
int pm10;

void setup() {
  Serial.begin(115200);
  pms_ss.begin(PMS7003_BAUD);
  dht.begin();
  InitWiFi();
}


void loop() {
  
  if ( !tb.connected() ) {
    reconnect();
  }
  
  delay(2000);

  //Temperature and Humidity Section
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.println(F("%"));
  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));

  // Carbon Dioxide Section
  float correctedPPM = mq135_sensor.getCorrectedPPM(t, h); //Temperature and Humidity data taken from DHT22 to adjust the CO2 ppm as they are correlated. 
  Serial.print(correctedPPM);
  Serial.println("ppm");


  // PM Section
  while (pms_ss.available()) { 
    pms_ss.read(); 
  }
  pms.requestRead();
  if (pms.readUntil(pms_data)) {
    pm1 = pms_data.PM_AE_UG_1_0;
    pm25 = pms_data.PM_AE_UG_2_5;
    pm10 = pms_data.PM_AE_UG_10_0;
    Serial.print("PM 1.0 (ug/m3): "); 
    Serial.println(pm1);
    Serial.print("PM 2.5 (ug/m3): "); 
    Serial.println(pm25);
    Serial.print("PM 10.0 (ug/m3): "); 
    Serial.println(pm10);
  }

  //Calibration formula and ThingsBoard Transmission
  if ( millis() - lastSend > 500 ) { // Update and send only after 1 seconds
    float calib_t = (0.82665*t + 4.85075);
    float calib_h = (1.33625*h - 21.02294);
    float calib_co2 = (((correctedPPM - 320.7772)*160.8)/204.0475)+425.8;
    float calib_pm = (((pm25 - 7.530973)*32)/15.71227)+15.4;
    tb.sendTelemetryFloat("Temperature", calib_t);
    tb.sendTelemetryFloat("Humidity", calib_h);
    tb.sendTelemetryFloat("CO2", calib_co2);
    tb.sendTelemetryFloat("PM1", pm1);
    tb.sendTelemetryFloat("PM2.5", calib_pm);
    tb.sendTelemetryFloat("PM10", pm10);
  }
}


// Wi-Fi setup
void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_MODE_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}


void reconnect() {
  // Loop until we're reconnected
  while (!tb.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(WIFI_MODE_AP, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
    Serial.print("Connecting to ThingsBoard node ...");
    if ( tb.connect(thingsboardServer, TOKEN) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED]" );
      Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}
