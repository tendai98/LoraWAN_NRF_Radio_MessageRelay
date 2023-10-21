#include "nrf_cmd.h"

#define DEFAULT_CHANNEL 2
#define DEFAULT_DATA_RATE_OPTION DATA_RATE_OPTION_2
#define DEFAULT_TX_POWER_OPTION TX_POWER_OPTION_4

RH_NRF24 radio(2, 4);

bool isNewDataFromSerial = false;
bool isNewEgressDataAvailable = false;
  

char egressBuffer[DEFAULT_BUFFER_LEN];
char ingressBuffer[DEFAULT_BUFFER_LEN];
void (*commands[6])();

void config_radio_channel(){
  
  int radioChannel = (int) dataBuffer[1];
  
  radio.setChannel(radioChannel);
  /*if(radio.setChannel(radioChannel)){
    Serial.printf("RADIO: CHANNEL-> 0x%x\n", radioChannel);
  }else{
    Serial.printf("RADIO: CHANNEL-> FAIL\n", radioChannel);    
  }*/

}

void config_radio_options(){
    int dataRateId = (int) dataBuffer[1];
    int txPowerId = (int) dataBuffer[2];
    
    radio.setRF(dataRateOptions[dataRateId], txPowerOptions[txPowerId]);
    
    /*if(radio.setRF(dataRateOptions[dataRateId], txPowerOptions[txPowerId])){
      Serial.printf("RADIO: OPTIONS: %s : %s\n", dataRateDisplayOptions[dataRateId], txPowerDisplayOptions[txPowerId]);
    }else{
      Serial.printf("RADIO: OPTIONS: ERROR:ERROR\n");    
    }*/  
}

void radio_recv_data(){
  uint8_t len = sizeof(ingressBuffer);
  
  if(radio.waitAvailableTimeout(1000)){
    radio.recv((uint8_t*) ingressBuffer, &len);
    Serial.print(ingressBuffer);

    for(int i=0; i<DEFAULT_BUFFER_LEN; i++){
      ingressBuffer[i]=0;
    }
  }
}

void radio_send_data(){
  if(isNewEgressDataAvailable){
    isNewEgressDataAvailable = false;
    uint8_t len = sizeof(egressBuffer);
    radio.send((uint8_t *) egressBuffer+1,  len);
    radio.waitPacketSent();
    //Serial.println("RADIO: EXEC PACKET TX");
    
    for(int i=0; i<DEFAULT_BUFFER_LEN; i++){
      egressBuffer[i]=0;
    }
  }
}


void load_data_buffer(){
  for(int i=1; i<DEFAULT_BUFFER_LEN; i++){
    egressBuffer[i] = dataBuffer[i];
  }
  isNewEgressDataAvailable = true;
  //Serial.println("RADIO: LOAD TX BUFFER");
}


void clear_buffers(){
  
  for(int i = 0; i < DEFAULT_BUFFER_LEN; i++){
    egressBuffer[i] = 0;
    ingressBuffer[i] = 0;
  }
  
  //Serial.println("RADIO: CLEAR TX:RX BUFFERS");
}

void reset_radio(){
  //Serial.println("RADIO: EXEC SOFT-RESET");
  delay(2000);
  ESP.reset();
}

void init_radio(){

  commands[0] = config_radio_channel;
  commands[1] = radio_recv_data;
  commands[2] = load_data_buffer;
  commands[3] = clear_buffers;
  commands[4] = reset_radio;
  commands[5] = config_radio_options;

  radio.init();
  
  /*if(radio.init()){
     Serial.println("RADIO: INIT");
  }else{
     Serial.println("RADIO: FAILED");    
  }*/

  radio.setChannel(DEFAULT_CHANNEL);
  /*if(radio.setChannel(DEFAULT_CHANNEL)){
    Serial.printf("RADIO: CHANNEL=%d\n", DEFAULT_CHANNEL);
  }else{
    Serial.printf("RADIO: RADIO CHANNEL=ERROR\n");
  }*/

  radio.setRF(dataRateOptions[DEFAULT_DATA_RATE_OPTION], txPowerOptions[DEFAULT_TX_POWER_OPTION]);
  /*if(radio.setRF(dataRateOptions[DEFAULT_DATA_RATE_OPTION], txPowerOptions[DEFAULT_TX_POWER_OPTION])){
    Serial.printf("RADIO: OPTIONS: %s : %s\n", dataRateDisplayOptions[DEFAULT_DATA_RATE_OPTION], txPowerDisplayOptions[DEFAULT_TX_POWER_OPTION]);
  }else{
    Serial.printf("RADIO: OPTIONS: ERROR:ERROR\n");    
  }*/  
}


void clear_data_buffer(){
  for(int i = 0; i < DEFAULT_BUFFER_LEN; i++){
    dataBuffer[i] = (char) 0;
  }
}

void execute_cmd(){
   if(isNewDataFromSerial){
      
      int command_index = dataBuffer[0] - 0x40;
      
      if(command_index >= 0 && command_index <= 7){
        commands[command_index]();
        clear_data_buffer();
      }
      
      isNewDataFromSerial = false;
   }
}

void run_radio(){
  int index = 0;
    
  while(Serial.available()){
    dataBuffer[index] = Serial.read();
    index++;
    delay(1);
  }

  
  if(index > 0){
    isNewDataFromSerial = true;
    execute_cmd();
    radio_send_data();
  }
}
