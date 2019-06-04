

/** Define Variables 
 *  Константы переменных для функций
 * 
 *  param: SERIAL
 *  param: RADIO
 *  param: STATUSES
 */

#define SERIAL_BAUDRATE 9600

#define RADIO_CHANNEL                   5
#define RADIO_PIPE_GREEN                1
#define RADIO_PIPE_RED                  2
#define RADIO_ADDRESS_GREEN             0xAABBCCDD11LL
#define RADIO_ADDRESS_RED               0xAABBCCDD22LL
#define RADIO_ADDRESS_REMOTE_CONTROL    0xAABBCCDD22LL
#define RADIO_DATARATE                  RF24_1MBPS
#define RADIO_PALEVEL                   RF24_PA_HIGH

// const uint8_t STATUS_BUTTON_START               = 0;
// const uint8_t STATUS_BUTTON_SCORE_GREEN_UP      = 3;
// const uint8_t STATUS_BUTTON_SCORE_GREEN_DOWN    = 4;
// const uint8_t STATUS_BUTTON_SCORE_RED_UP        = 5;
// const uint8_t STATUS_BUTTON_SCORE_RED_DPWN      = 6;
// const uint8_t STATUS_BUTTON_UPDATE_SCORE        = 7;
// const uint8_t STATUS_BUTTON_UPDATE_TIMER        = 8;
// const uint8_t STATUS_BUTTON_STOP                = 9;