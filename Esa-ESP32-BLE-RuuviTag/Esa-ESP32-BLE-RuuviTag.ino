//https://www.hackster.io/amir-pournasserian/ble-weather-station-with-esp32-and-ruuvi-e8a68d

/* tiedosto liian iso, jotta saa latautumaan pitää valita partition 
 "Partition Scheme -> Huge APP (3MB No OTA/1MB SPIFFS). Tätä ei löydy Tools alta, pitää valita Board
 ESP32DevModule:ksi
 */

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEAddress.h>
#include <WiFi.h>
#include <HTTPClient.h>

int scanTime = 5; //In seconds
BLEScan* pBLEScan;
int temp, hum, pressure, ax, ay, az, voltage_power, voltage, power, rssi_ruuvi, movement, measurement;
String payload;
//String MAC_add = "MAC ADDRESS HERE"; //All the identified MAC addresses will go in one String
String MAC_add = "ca:f7:7e:04:0e:15"; //,D3:0D:8C:08:21:90";

const char* ssid     = "Pajaranta";              //Main Router      
const char* password = "Perttulintie7C";            //Main Router Password
const char* url = "http://esasiot.hub.ubeac.io/esaubeacgw2"; 

//Converts hexadecimal values to decimal values
int hexadecimalToDecimal(String hexVal)
{
    int len = hexVal.length();
    int base = 1;

    int dec_val = 0;

    for (int i = len - 1; i >= 0; i--)
    {
        if (hexVal[i] >= '0' && hexVal[i] <= '9')
        {
            dec_val += (hexVal[i] - 48) * base;

            base = base * 16;
        }
        else if (hexVal[i] >= 'A' && hexVal[i] <= 'F')
        {
            dec_val += (hexVal[i] - 55) * base;

            base = base * 16;
        }
    }
    return dec_val;
}

//Decodes RUUVI raw data and arranges it in an array
void decodeRuuvi(String hex_data, int rssi){
    Serial.printf("decodeRuuvi got: %s %d", hex_data, rssi);
    if(hex_data.substring(4, 6) == "05"){
        temp = hexadecimalToDecimal(hex_data.substring(6, 10))*0.005;
        hum = hexadecimalToDecimal(hex_data.substring(10, 14))*0.0025;
        pressure = hexadecimalToDecimal(hex_data.substring(14, 18))*1+50000;

        ax = hexadecimalToDecimal(hex_data.substring(18, 22));
        ay = hexadecimalToDecimal(hex_data.substring(22, 26));
        az = hexadecimalToDecimal(hex_data.substring(26, 30));     

        if(ax > 0xF000){
          ax = ax - (1 << 16);
        }
        if(ay > 0xF000){
          ay = ay - (1 << 16);
        }
        if (az > 0xF000){
          az = az - (1 << 16);
        }

        voltage_power = hexadecimalToDecimal(hex_data.substring(30, 34));
        voltage = (int)((voltage_power & 0x0b1111111111100000) >> 5) + 1600;
        power = (int)(voltage_power & 0x0b0000000000011111) - 40;

        rssi_ruuvi = rssi;

        movement = hexadecimalToDecimal(hex_data.substring(34, 36));
        measurement = hexadecimalToDecimal(hex_data.substring(36, 40));
    }
}

//Converts decoded RUUVI data into uBeac JSON format
void uBeacRuuvi(){
  payload = "[{\"id\": \"MyRUUVI\", \"sensors\": [{\"id\": \"Temperature\", \"value\": $temperature$}, {\"id\": \"Humidity\", \"value\": $humidity$}, "
                   "{\"id\": \"Pressure\", \"value\": $pressure$}, {\"id\": \"Acceleration\", \"value\": {\"ax\": $ax$,\"ay\": $ay$,\"az\": $az$,}}, "
                   "{\"id\": \"Voltage Power\", \"value\": $voltage_power$}, {\"id\": \"Voltage\", \"value\": $voltage$}, {\"id\": \"Power\", \"value\": $power$}, "
                   "{\"id\": \"RSSI\", \"value\": $rssi$}, {\"id\": \"Movement Counter\", \"value\": $movement$}, {\"id\": \"Measurement Sequence\", \"value\": $measurement$}]}]";

  payload.replace("$temperature$",String(temp));
  payload.replace("$humidity$",String(hum));  
  payload.replace("$pressure$",String(pressure/100));
  payload.replace("$ax$",String(ax*9.81/1000));
  payload.replace("$ay$",String(ay*9.81/1000));
  payload.replace("$az$",String(az*9.81/1000));
  payload.replace("$voltage_power$",String(voltage_power));
  payload.replace("$voltage$",String(voltage));
  payload.replace("$power$",String(power));
  payload.replace("$rssi$",String(rssi_ruuvi));
  payload.replace("$movement$",String(movement));
  payload.replace("$measurement$",String(measurement));
}

//Class that scans for BLE devices
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    //Serial.println("MyAdvertisedDeviceCallbacks");
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      //Scans for specific BLE MAC addresses 
      if(MAC_add.indexOf(advertisedDevice.getAddress().toString().c_str()) >= 0){ //If the scanned MAC address is in the identified MAC address String
        String raw_data = String(BLEUtils::buildHexData(nullptr, (uint8_t*)advertisedDevice.getManufacturerData().data(), advertisedDevice.getManufacturerData().length()));
        raw_data.toUpperCase();
        Serial.println("MyAdvertisedDeviceCallbacks");
        Serial.println(raw_data);
        decodeRuuvi(raw_data, advertisedDevice.getRSSI());
        uBeacRuuvi();
      }  

      //  Sends RuuviTag JSON data to IoT clod platform
      if(WiFi.status()== WL_CONNECTED){ 
 
        HTTPClient http;   
  
        http.begin(url);  
        int httpResponseCode = http.POST(payload); 
 
        if(httpResponseCode>0){
          String response = http.getString(); 
          Serial.println(httpResponseCode);
        }
        http.end();
 
      }else{
        Serial.println("Error in WiFi connection");    
      } 
    }
};

void setup() {
  Serial.begin(19200);
  
  //Connect to Local WiFi
  delay(4000);   //Delay needed before calling the WiFi.begin
 
  WiFi.begin(ssid, password); 
 
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.printf("Connected to the WiFi network %s\n",ssid);

  //BLE scanning
  Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

void loop() {
  Serial.println("Starting loop:");
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.println("found devices");

  int deviceCount = foundDevices.getCount();
  Serial.println(deviceCount);

  for (uint32_t i = 0; i < deviceCount; i++)
  {
    BLEAdvertisedDevice device = foundDevices.getDevice(i);
    Serial.println("attempt to print device names");
    Serial.print(device.getName().c_str());
    }

  //Serial.println(foundDevices.dump());
  //  Serial.println(BLEScanResults->getCount());
  
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(3000);
}
