

#include "Arduino.h"
#include "BTHC05.h"
#include "DCMDriverL298.h"
#include "Encoder.h"
#include "Button.h"


// Пины на ардуино
#define BTHC05_PIN_RXD	10
#define BTHC05_PIN_TXD	11
#define DCMOTORDRIVERL298_PIN_ENA	5
#define DCMOTORDRIVERL298_PIN_INT1	4
#define DCMOTORDRIVERL298_PIN_INT2	7
#define DCMOTORDRIVERL298_PIN_ENB	6
#define DCMOTORDRIVERL298_PIN_INT3	8
#define DCMOTORDRIVERL298_PIN_INT4	9
#define ROTARYENCI_PIN_D	3
#define ROTARYENCI_PIN_CLK	2
#define ROTARYENCI_PIN_S1	12




long rotaryEncIOldPosition  = 0;

Encoder rotaryEncI(ROTARYENCI_PIN_D,ROTARYENCI_PIN_CLK);
Button rotaryEncIButton(ROTARYENCI_PIN_S1);
BTHC05 bthc05(BTHC05_PIN_RXD,BTHC05_PIN_TXD);
DCMDriverL298 dcMotorDriverL298(DCMOTORDRIVERL298_PIN_ENA,DCMOTORDRIVERL298_PIN_INT1,DCMOTORDRIVERL298_PIN_INT2,DCMOTORDRIVERL298_PIN_ENB,DCMOTORDRIVERL298_PIN_INT3,DCMOTORDRIVERL298_PIN_INT4);



// объявляем переменные
const int timeout = 10000;       //таймаут 10 сек
char menuOption = 0;
long time0;


void setup() 
{

    Serial.begin(9600);
    while (!Serial) ; 
    Serial.println("start");
    
    bthc05.begin(9600);
    //для теста в терминал должно придти сообщение
    bthc05.println("Bluetooth On....");
    rotaryEncIButton.init();
    pinMode(ROTARYENCI_PIN_S1, INPUT_PULLUP);
    menuOption = menu();
    
}


void loop() 
{   
    
    if(menuOption == '1') {
    // HC - 05 Test Code
    String bthc05Str = "";
    //получаем строку с блютуз
    if (bthc05.available())
    {
        //Считываем строку введённую в терминале
        bthc05Str = bthc05.readStringUntil('\n');
        // печатаем в serial monitor
        Serial.print("BT Raw Data: ");
        Serial.println(bthc05Str);
    }
    //отправляем данные с датчиков по блютуз
    bthc05.println("PUT YOUR SENSOR DATA HERE");
    }
    else if(menuOption == '2') {
    // L298N Motor Driver - Test Code

    // запускаем оба двигателя, скорость от 0-255, вращение вправо влево(1, 0)
    dcMotorDriverL298.setMotorA(200,1);
    dcMotorDriverL298.setMotorB(200,0);
    delay(2000);
    //останавливаем двигатель
    dcMotorDriverL298.stopMotors();
    delay(2000);

    }
    else if(menuOption == '3') {
    // Енкодер - Test Code
    //Считываем позицию енкодера
    long rotaryEncINewPosition = rotaryEncI.read();
    bool rotaryEncIButtonVal = rotaryEncIButton.onPress();
    if (rotaryEncINewPosition != rotaryEncIOldPosition || rotaryEncIButtonVal) {
        rotaryEncIOldPosition = rotaryEncINewPosition;
        Serial.print(F("Pos: "));
        Serial.print(rotaryEncINewPosition); // печатаем позицию в монитор порта
        Serial.print(F("\tButton: "));
        Serial.println(rotaryEncIButtonVal);
      }

    }
    
    
    
    if (millis() - time0 > timeout)
    {
        menuOption = menu();
    }
    
}



// Меню для выбора компанентов для тестирования
char menu()
{

    Serial.println(F("\nWhich component would you like to test?"));
    Serial.println(F("(1) HC - 05 Bluetooth"));
    Serial.println(F("(2) L298N Motor Driver"));
    Serial.println(F("(3) Encoder"));
    Serial.println(F("(menu) send anything else or press on board reset button\n"));
    while (!Serial.available());

    
    while (Serial.available()) 
    {
        char c = Serial.read();
        if (isAlphaNumeric(c)) 
        {
            if(c == '1') 
    			Serial.println(F("Now Testing HC - 05 Bluetooth "));
    		else if(c == '2') 
    			Serial.println(F("Now Testing L298N Motor Driver "));
    		else if(c == '3') 
    			Serial.println(F("Now Testing Encoder"));
            else
            {
                Serial.println(F("illegal input!"));
                return 0;
            }
            time0 = millis();
            return c;
            }
        }
    }
