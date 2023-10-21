#include "nrf.h"

void setup(){
  Serial.begin(9600);
  init_radio();
}

void loop(){
   radio_recv_data();
   run_radio();
   delay(25);
}
