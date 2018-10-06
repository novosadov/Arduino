#include "DCMDriverL298.h"

#include <SoftwareSerial.h>
SoftwareSerial BTserial(10, 11); // RX | TX


#include <ACE128.h>  // Absolute Contact Encoder
// #include <ACE128map12345678.h> // mapping for pin order 12345678
#include <ACE128map87654321.h> // mapping for pin order 87654321 (backpack module)
#include <Wire.h> // I2C bus communication library

#define ACE_ADDR 0x38
ACE128 myACE((uint8_t)ACE_ADDR, (uint8_t*)encoderMap_87654321); // backpack module


#define DCMOTORDRIVERL298_PIN_ENA  5
#define DCMOTORDRIVERL298_PIN_INT1  4
#define DCMOTORDRIVERL298_PIN_INT2  7
#define DCMOTORDRIVERL298_PIN_ENB 6
#define DCMOTORDRIVERL298_PIN_INT3  8
#define DCMOTORDRIVERL298_PIN_INT4  9

DCMDriverL298 dcMotorDriverL298(DCMOTORDRIVERL298_PIN_ENA,DCMOTORDRIVERL298_PIN_INT1,DCMOTORDRIVERL298_PIN_INT2,DCMOTORDRIVERL298_PIN_ENB,DCMOTORDRIVERL298_PIN_INT3,DCMOTORDRIVERL298_PIN_INT4);


// set-zero button on pin 13
// - button to ground e.g. MakerShield button
const int ZERO = 13;
uint8_t pinPos = 0; // pin values from MCP23008 or PCF8574
uint8_t rawPos = 0;
uint8_t upos = 0;
uint8_t oldPos = 255;
int8_t pos;
int16_t mpos;
uint8_t seen = 0;


const long baudRate = 9600; 
char c=' ';
boolean NL = true;

char incomingByte;  // входящие данные

int rotation; //направление вращение двигателя
int speedmotor;



void setup() 
{
  myACE.begin();    // this is required for each instance, initializes the IO expander
  // myACE.reverse(true);  // uncomment this for counter-clockwise operation
  pinMode(ZERO, INPUT_PULLUP);    // configure set-zero button
  pinPos = myACE.acePins();          // get IO expander pins
  oldPos = pinPos;                 // remember where we are
  Serial.begin(9600);
  Serial.println(myACE.acePins());

  BTserial.begin(baudRate);

}

void loop() 
{
   // Считываем данные с блютуз и отправляем в Serial monitor
    if (BTserial.available())
    {
        c = BTserial.read();
        Serial.write(c);               
    }




  
   if (digitalRead(ZERO) == 0) 
   {      
    // check set-zero button

    // myACE.setMpos(6723);  // set current position to 6723
    myACE.setMpos(0);    // set logical multiturn zero to current position
    // myACE.setZero();     // set logical zero to current position
    // myACE.reverse(true);  // set reverse
    oldPos = 255;                   // force display update
  }
  pinPos = myACE.acePins();          // get IO expander pins
  rawPos = myACE.rawPos();           // get raw mechanical position
  pos = myACE.pos();                 // get logical position - signed
  upos = myACE.upos();               // get logical position - unsigned
  mpos = myACE.mpos();               // get multiturn position - signed

  if (pinPos != oldPos) {            // did we move?
    seen |= pinPos ^ oldPos;         // what changed?
    oldPos = pinPos;                 // remember where we are
    if (seen < 255) {
      
      for (uint8_t i = 0; i <= 7; i++) {
        if (! (seen & 1 << i)) {
           Serial.println(i, DEC);
        }
      }
    } else {      
       /*Serial.println("pin ");
       Serial.println(pinPos);  
       BTserial.println(pinPos);            
       */
           
       Serial.println("raw ");
       Serial.println(rawPos);   
       BTserial.println(rawPos); /*
       отправляем исходное механическое положение 
       */
   
       Serial.println(pos, DEC);  
       BTserial.println(pos, DEC); 
       /*
       отправляем позицию -64 -> +63
       */
           
       Serial.println(upos, DEC);   
       BTserial.println(upos, DEC);
       /*
       отправляем позицию 0 -> 127
       */
          
       Serial.println(mpos, DEC);
       BTserial.println(mpos, DEC);
       /*
       отправляем позицию -32768 -> +32768
       */
    }
  }



//если приходит символ 'r' - устанавливаем вращение вправо
  if (c=='r')
  {
    rotation = 1;
    }
    

//если приходит символ 'l' - устанавливаем вращение влево
    if (c=='l')
  {
    rotation = 0;
    }



// скорость вращения двигателя
    if (c >= 0 && c <= 255)
    {
      speedmotor=c;
    }
    else
    {
      speedmotor=100;
      }



    dcMotorDriverL298.setMotorA(speedmotor,rotation);/*устанавливаем скорость и
     и в какую сторону будем вращать двигатель*/ 



//Останавливаем двигатель
    if(c=='s')
    {
      dcMotorDriverL298.stopMotors();
    }

    



  // Считываем данные из Serial Monitor и отправляем на Bluetooth
    if (Serial.available())
    {
        c = Serial.read();
        BTserial.write(c);   
 
        
        if (NL) { Serial.print(">");  NL = false; }
        Serial.write(c);
        if (c==10) { NL = true; }
    }
}
