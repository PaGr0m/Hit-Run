#ifndef __SETTINGS_H__
#define __SETTINGS_H__


/** Define Variables 
 *  Константы переменных
 * 
 *  param: SERIAL
 *  param: RADIO
 */
#define SERIAL_BAUDRATE 9600
#define SERIAL_DELAY_CONSOLE_ACTIVATE 3000

#define RADIO_CHANNEL                   5
#define RADIO_PIPE_GREEN                1
#define RADIO_PIPE_RED                  2
#define RADIO_ADDRESS_GREEN             0xAABBCCDD11LL
#define RADIO_ADDRESS_RED               0xAABBCCDD22LL
#define RADIO_ADDRESS_REMOTE_CONTROL    0xAABBCCDD33LL
#define RADIO_DATARATE                  RF24_1MBPS
#define RADIO_PALEVEL                   RF24_PA_HIGH


/** Define Variables 
 *  Константы переменных
 * 
 *  param: STATUSES
 */
const unsigned char STATUS_BUTTON_START               = 0;
const unsigned char STATUS_SPORTSMEN_GREEN            = 1;
const unsigned char STATUS_SPORTSMEN_RED              = 2;
const unsigned char STATUS_BUTTON_SCORE_GREEN_UP      = 3;
const unsigned char STATUS_BUTTON_SCORE_GREEN_DOWN    = 4;
const unsigned char STATUS_BUTTON_SCORE_RED_UP        = 5;
const unsigned char STATUS_BUTTON_SCORE_RED_DOWN      = 6;
const unsigned char STATUS_BUTTON_UPDATE_SCORE        = 7;
const unsigned char STATUS_BUTTON_UPDATE_TIMER        = 8;
const unsigned char STATUS_BUTTON_STOP                = 9;


#endif