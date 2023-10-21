#include <RH_NRF24.h>

#define DEFAULT_BUFFER_LEN RH_NRF24_MAX_MESSAGE_LEN

#define DATA_RATE_OPTION_1 0x00
#define DATA_RATE_OPTION_2 0x01
#define DATA_RATE_OPTION_3 0x02

#define TX_POWER_OPTION_1 0x00
#define TX_POWER_OPTION_2 0x01
#define TX_POWER_OPTION_3 0x02
#define TX_POWER_OPTION_4 0x03

RH_NRF24::DataRate dataRateOptions [3] = {
  RH_NRF24::DataRate1Mbps, 
  RH_NRF24::DataRate2Mbps, 
  RH_NRF24::DataRate250kbps
};
  
RH_NRF24::TransmitPower txPowerOptions [4] = {
    RH_NRF24::TransmitPowerm18dBm, 
    RH_NRF24::TransmitPowerm12dBm, 
    RH_NRF24::TransmitPowerm6dBm,
    RH_NRF24::TransmitPower0dBm,
};

char * dataRateDisplayOptions[3] = { "1Mbps", "2Mbps", "250Kbps"};
char * txPowerDisplayOptions[4] = { "18dBm", "12dBm", "6dBm", "0dBm"};
