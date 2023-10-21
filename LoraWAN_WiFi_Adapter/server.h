#include <WebServer.h>

#define PORT 80
#define HTTP_OK_CODE 200
#define DEFAULT_MIME "text/html"

WebServer server(PORT);

String data;

void get_recv_data(){
   if(isNewIngressDataAvailable){
      isNewIngressDataAvailable = false;
      server.send(HTTP_OK_CODE, DEFAULT_MIME, ingressBuffer);
   }else{
      server.send(HTTP_OK_CODE, DEFAULT_MIME, "RADIO: NO RX DATA");
   }

   for(int i = 0; i < MAX_BUFFER_LEN; i++){
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
  
  for(int i = 0; i < MAX_BUFFER_LEN; i++){
    egressBuffer[i] = 0;
    ingressBuffer[i] = 0;
  }
  server.send(HTTP_OK_CODE, DEFAULT_MIME, "RADIO: CLEAR BUFFERS");
}

void reset_radio(){
  server.send(HTTP_OK_CODE, DEFAULT_MIME, "RADIO RESET");
  delay(2000);
  ESP.restart();
}


void init_server(){
 
  server.on("/tx_packet", run_load_data);
  server.on("/rx_packet", get_recv_data);
  server.on("/clear", clear_buffers);
  server.on("/reset", reset_radio);
  
  server.begin();  
}

void server_loop(){
  server.handleClient();
}
