//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"
#include <Streaming.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

const int ledPin =  5;
// Handle received and sent messages
String message = "";
char incomingChar;

// Timer: auxiliar variables
unsigned long previousMillis = 0;    // Stores last time temperature was published
const long interval = 10000;         // interval at which to publish sensor readings


void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name

  setupDHT();
  Serial.println(F("DHT22 test ok!"));
  
  //scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  setupScale();
   Serial.println(F("HX711 scale set up."));
  
  pinMode(ledPin, OUTPUT);
  //dht.begin();
  
  //Serial.println("Mortonki! The device started, now you can pair it with bluetooth!");
  Serial << "Mortonki! The device started, now you can pair it with bluetooth!\n";
  
}

void loop() {

  unsigned long currentMillis = millis();
  // Send temperature readings
  if (currentMillis - previousMillis >= interval){
    previousMillis = currentMillis;
  
    String temp_hum = readDHT22_measurements();
    if(!temp_hum.equals("")) {
      SerialBT.println(temp_hum);
      Serial << "DHT22: " << temp_hum << endl;
    }
    else Serial << "failed to read DHT\n";
    
    long scale = readScale();
    SerialBT.println(scale);
    Serial << "HX711 Scale read: " << scale << endl;
   
  }
   // check if characters available in serialBT 
   if (SerialBT.available()){
    char incomingChar = SerialBT.read();
    if (incomingChar != '\n'){
      message += String(incomingChar);
    }
    else{
      message = "";
    }
    // write the incoming message to console
    //Serial.write(incomingChar);  
    Serial << "Got via BT: " << message << endl;
  }
  
  if (message =="led_on"){
    digitalWrite(ledPin, HIGH);
  }
  else if (message =="led_off"){
    digitalWrite(ledPin, LOW);
  }
  else if (message=="calibrate"){
    Serial << "calibrating the scale" << endl;
  }

  
   delay(20);
}
