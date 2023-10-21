#include "net.h"
#include "nrf.h"
#include "server.h"

void setup(){
  Serial.begin(9600);
  init_network();
  init_radio();
  init_server();
}

void loop(){
   radio_recv_data();
   radio_send_data();
   server_loop();
   delay(25);
}
