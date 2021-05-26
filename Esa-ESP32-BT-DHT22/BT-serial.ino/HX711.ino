#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 21;
const int LOADCELL_SCK_PIN = 19;

HX711 scale;

void setupScale() {
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
}
long readScale() {
 //scale.power_up();
  
  if (scale.is_ready()) {
    long reading = scale.read();
    //Serial.print("HX711 reading: ");
    //Serial.println(reading);
    return reading;
  } else {
    //Serial.println("HX711 not found.");
    return 0;
  }
  //scale.power_down();  // put ADC in sleep mode - power saving EI TOIMI EI HERÄÄ 
}
