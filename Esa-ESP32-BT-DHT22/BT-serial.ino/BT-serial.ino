//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"

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
  
  pinMode(ledPin, OUTPUT);
  //dht.begin();
  
  Serial.println("Moron! The device started, now you can pair it with bluetooth!");

  
}

void loop() {

  unsigned long currentMillis = millis();
  // Send temperature readings
  if (currentMillis - previousMillis >= interval){
    previousMillis = currentMillis;
  
    String tmp = readDHT22_measurements();
    if(!tmp.equals("")) {
      SerialBT.println(tmp);
    }
    else Serial.println("failed to read DHT");
    
    readScale();
   
  }
  
   if (SerialBT.available()){
    char incomingChar = SerialBT.read();
    if (incomingChar != '\n'){
      message += String(incomingChar);
    }
    else{
      message = "";
    }
    Serial.write(incomingChar);  
  }
  
  if (message =="led_on"){
    digitalWrite(ledPin, HIGH);
  }
  else if (message =="led_off"){
    digitalWrite(ledPin, LOW);
  }

  
   delay(20);
}
