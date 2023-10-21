bool isNewEgressDataAvailable = false;
bool isNewIngressDataAvailable = false;

#define MAX_BUFFER_LEN 200
#define BLINK_LED_GPIO 4

char egressBuffer[MAX_BUFFER_LEN];
char ingressBuffer[MAX_BUFFER_LEN];

void blink_led(int timeout){
  digitalWrite(BLINK_LED_GPIO, HIGH);
  delay(timeout);
  digitalWrite(BLINK_LED_GPIO, LOW);
  delay(timeout);
}

void radio_recv_data(){
  int index = 0;
  
  while(Serial.available()){
    ingressBuffer[index] = Serial.read();
    index++;
    delay(1);
  }

  if(index > 0){
    isNewIngressDataAvailable = true;
    blink_led(150);
  }
}

void radio_send_data(){
  if(isNewEgressDataAvailable){
    isNewEgressDataAvailable = false;

    Serial.print(egressBuffer);
    
    for(int i=0; i<MAX_BUFFER_LEN; i++){
      egressBuffer[i]=0;
    }
  }
}

void init_radio(){
  pinMode(BLINK_LED_GPIO, OUTPUT);
  digitalWrite(BLINK_LED_GPIO, LOW);
  blink_led(500);
}
