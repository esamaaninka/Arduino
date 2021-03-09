#include "DHT.h"

#define DHTPIN 27
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

String temperatureString = "";

void setupDHT() {
  dht.begin();
}

String readDHT22_measurements() {
     // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) ) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return "";
    }

    temperatureString = String(dht.readTemperature()) + "C  " +  String(dht.readHumidity()) + "%";
    //sensors.requestTemperatures(); 
    //temperatureString = String(sensors.getTempCByIndex(0)) + "C  " +  String(sensors.getTempFByIndex(0)) + "F";
    return temperatureString;
}
