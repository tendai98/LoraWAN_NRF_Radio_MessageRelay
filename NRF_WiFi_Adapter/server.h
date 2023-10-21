#include <ESP8266WebServer.h>

#define PORT 80
#define HTTP_OK_CODE 200
#define DEFAULT_MIME "text/html"

ESP8266WebServer server(PORT);

int radioChannel = 0;
int dataRateId = 0;
int txPowerId = 0;
String data;

void set_radio_channel(){
  radioChannel = server.arg(0).toInt();
  
  if(radio.setChannel(radioChannel)){
      server.send(HTTP_OK_CODE, DEFAULT_MIME, "RF CHANNEL: SET");
  }else{
      server.send(HTTP_OK_CODE, DEFAULT_MIME, "RF CHANNEL: FAIL");
  }
}

void set_radio_options(){
    dataRateId = server.arg(0).toInt();
    txPowerId = server.arg(1).toInt();
  
    if(radio.setRF(dataRateOptions[dataRateId], txPowerOptions[txPowerId])){
        server.send(HTTP_OK_CODE, DEFAULT_MIME, "RF OPTIONS: SET");
    }else{
        server.send(HTTP_OK_CODE, DEFAULT_MIME, "RF OPTIONS: FAIL");
    }    
}


void get_recv_data(){
   if(isNewIngressDataAvailable){
      isNewIngressDataAvailable = false;
      server.send(HTTP_OK_CODE, DEFAULT_MIME, ingressBuffer);
   }else{
      server.send(HTTP_OK_CODE, DEFAULT_MIME, "RADIO: NO RX DATA");
   }

   for(int i = 0; i < RH_NRF24_MAX_MESSAGE_LEN; i++){
    ingressBuffer[i] = 0;
   }
}

void run_load_data(){
    data = server.arg(0);
    data.toCharArray(egressBuffer, data.length()+1);
    isNewEgressDataAvailable = true; 
    server.send(HTTP_OK_CODE, DEFAULT_MIME, "RADIO: EXEC PACKET TX");
}

void clear_buffers(){
  
  for(int i = 0; i < RH_NRF24_MAX_MESSAGE_LEN; i++){
    egressBuffer[i] = 0;
    ingressBuffer[i] = 0;
  }
  server.send(HTTP_OK_CODE, DEFAULT_MIME, "RADIO: CLEAR BUFFERS");
}

void reset_radio(){
  server.send(HTTP_OK_CODE, DEFAULT_MIME, "RADIO RESET");
  delay(2000);
  ESP.reset();
}


void init_server(){
  
  radioChannel = DEFAULT_CHANNEL;
  dataRateId = DEFAULT_DATA_RATE_OPTION;
  txPowerId = DEFAULT_TX_POWER_OPTION;

  server.on("/channel", set_radio_channel);
  server.on("/tx_packet", run_load_data);
  server.on("/rx_packet", get_recv_data);
  server.on("/rf_options", set_radio_options);
  server.on("/clear", clear_buffers);
  server.on("/reset", reset_radio);
  
  server.begin();  
}

void server_loop(){
  server.handleClient();
}
