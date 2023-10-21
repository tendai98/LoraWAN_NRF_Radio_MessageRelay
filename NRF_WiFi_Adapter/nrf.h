#include "nrf_cmd.h"

#define DEFAULT_CHANNEL 2
#define DEFAULT_DATA_RATE_OPTION DATA_RATE_OPTION_2
#define DEFAULT_TX_POWER_OPTION TX_POWER_OPTION_4

RH_NRF24 radio(2, 4);

bool isNewEgressDataAvailable = false;
bool isNewIngressDataAvailable = false;

char egressBuffer[RH_NRF24_MAX_MESSAGE_LEN];
char ingressBuffer[RH_NRF24_MAX_MESSAGE_LEN];

void radio_recv_data(){
  uint8_t len = sizeof(ingressBuffer);
  
  if(radio.waitAvailableTimeout(1000)){
    if(radio.recv((uint8_t*) ingressBuffer, &len)){
      isNewIngressDataAvailable = true;
    }
  }
}

void radio_send_data(){
  if(isNewEgressDataAvailable){
    isNewEgressDataAvailable = false;
    uint8_t len = sizeof(egressBuffer);
    radio.send((uint8_t *) egressBuffer,  len);
    radio.waitPacketSent();
    
    for(int i=0; i<DEFAULT_BUFFER_LEN; i++){
      egressBuffer[i]=0;
    }
  }
}


void init_radio(){

  if(radio.init()){
     Serial.println("RADIO INIT");
  }else{
     Serial.println("INIT FAILED");    
  }

  if(radio.setChannel(DEFAULT_CHANNEL)){
    Serial.printf("SET RADIO CHANNEL=%d\n", DEFAULT_CHANNEL);
  }else{
    Serial.printf("SET RADIO CHANNEL=ERROR\n");
  }

  if(radio.setRF(dataRateOptions[DEFAULT_DATA_RATE_OPTION], txPowerOptions[DEFAULT_TX_POWER_OPTION])){
    Serial.printf("SET RF-OPTIONS: %s : %s\n", dataRateDisplayOptions[DEFAULT_DATA_RATE_OPTION], txPowerDisplayOptions[DEFAULT_TX_POWER_OPTION]);
  }else{
    Serial.printf("SET RF-OPTIONS: ERROR:ERROR\n");    
  }  
}
