// Libraries
#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <printf.h>


// Define Pins
#define PIN_NRF_CE 7
#define PIN_NRF_CSN 8


// Initialize Variables
const uint64_t pipe01 = 0xF0F1F2F3F4LL;
const uint8_t TIME_DUAL_HIT = 100;
volatile boolean flag_tx = false;
volatile boolean flag_fail = false;
volatile boolean flag_rx = false;
int i;
uint8_t pipe;


// Initialize Objects
RF24 radio(PIN_NRF_CE, PIN_NRF_CSN);

void radioSettings() 
{
    radio.begin();                              // Инициируем работу nRF24L01+
    radio.setChannel(5);                        // Указываем канал приёма данных (от 0 до 127), 5 - значит приём данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
    radio.setDataRate(RF24_1MBPS);              // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
    radio.setPALevel(RF24_PA_HIGH);             // Указываем мощность передатчика RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
    radio.openReadingPipe(1, 0xAABBCCDD11LL);   // Открываем 1 трубу с идентификатором 1 передатчика 0xAABBCCDD11, для приема данных
    radio.openReadingPipe(2, 0xAABBCCDD22LL);   // Открываем 2 трубу с идентификатором 2 передатчика 0xAABBCCDD22, для приема данных
    // radio.printDetails();
    radio.startListening();
}

/**
 * TODO: 
 * так как переделано на прерывания то я не получаю номер pipe
 * тем самым стоит посылать номер спортсмена
 */
void receiveData() 
{
    /*
    Если в буфере имеются принятые данные, то получаем номер трубы,
    по которой они пришли, по ссылке на переменную pipe
    */
    uint8_t reveiveData;


    radio.read(&reveiveData, sizeof(reveiveData) );
    printf("%d", reveiveData);
    delay(2000);

    // Serial.write(pipe);
    // printf("%s\n", pipe);

    // TODO: добавить возможность обоюдного удара
    // дать время на считывание второго спортсмена

}

void setup()
{
    /**
     * При "падающем" прерывании на pin 3 
     * вызывается функция check_radio
     */
    attachInterrupt(1, check_radio, FALLING);

    Serial.begin(9600);
    printf_begin();
    printf("\n\r >>>>> RECEIVER-CONTROLLER <<<<< \n\r");

    // Radio settings
    radioSettings();

    // Wait for console opening
    delay(3000);
}

// обработчик прерывания
void checkRadioData() 
{
    bool tx, fail, rx;
    radio.whatHappened(tx, fail, rx); // читаем регистр
    flag_tx = tx;                     // 1 если успешно отправили данные
    flag_fail = fail;                 // 1 если данные не отправленны
    flag_rx = rx;                     // 1 если пришли данные для чтения
}


void scoreCounter(uint8_t status) 
{
    if (status == 1)
    {
        // TODO: спортсмен слева ++
    }
    else if (status == 2)
    {
        // TODO: спортсмен справа ++
    }
    else if (status == 3)
    {
        // TODO: спортсмен слева и справа ++
    }

    // updateScore
}

void loop() 
{
    // если пришли данные для чтения
    // FIXME: попытка проверки обоюдных ударов
    if (flag_rx) 
    {
        flag_rx = false;

        uint8_t sportsmen = 0;
        radio.read(&sportsmen, sizeof(sportsmen));

        uint8_t beginTime = millis();
        while (millis() < beginTime + 40)
        {
            if (flag_rx)
            {
                flag_rx = false;
                sportsmen = 3;   
            } 
        }
    }

    // если данные не отправленны
    if (flag_fail)
    {
        printf("Flag_fail : данные не отправлены");
        flag_fail = 0;
    }

    // если данные отправленны
    if (flag_tx)
    {
        printf("Flag_tx : данные отправлены");
        flag_tx = 0;
    }
}