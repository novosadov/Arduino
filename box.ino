#include <SoftwareSerial.h>
SoftwareSerial BTserial(8, 9); // Устанавливаем програмно  RX | TX
 
const long baudRate = 9600; 
char c=' ';
boolean NL = true;

char incomingByte;  // входящие данные
int  Right = 6;      // подключение право реле
int Left = 7;        // подключение левого реле

int Led = 10;
int Led1 = 11;       // подключение питания к светодиоду
int Led2 = 12;
int Parktronik = 3;  //подключение парктроника

int trigPin = 4;    //Триггер парктроника
int echoPin = 5;    //Эхо парктроника

long duration, cm, inches;
 
void setup() 
{
    Serial.begin(9600);
    Serial.print("Sketch:   ");   Serial.println(__FILE__);
    Serial.print("Uploaded: ");   Serial.println(__DATE__);
    Serial.println(" ");
 
    BTserial.begin(baudRate);  // устанавливаем скорость передачи по блютуз
    Serial.print("BTserial started at ");//выводим сообщение в монитор порта о подкл. по блютуз
    Serial.println(baudRate); ////выводим сообщение в монитор порта скорость подключения блютуз
    Serial.println(" ");

    
    //устанавливаем контакты в нужное положение
    pinMode(Right, OUTPUT);
    pinMode(Left, OUTPUT);    
    pinMode(Led, OUTPUT);
    pinMode(Led1, OUTPUT);
    pinMode(Led2, OUTPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(Parktronik, OUTPUT);
  
}

 
void loop()
{
  
  
  if(c == 'p') {
      //Если пришёл символ включаем парктроник         
       digitalWrite(Parktronik, HIGH); 
       digitalWrite(Parktronik, HIGH);
       digitalWrite(Parktronik, HIGH);
       Serial.println(" ");  
    }
     else{
      //Иначе отключаем парктроник
      digitalWrite(Parktronik, LOW);
      digitalWrite(Parktronik, LOW);
      digitalWrite(Parktronik, LOW);      
      Serial.println(" "); 
     }

    // отключаем парктроник кнопкой в приложении
    if(c == 'n') {
     digitalWrite(Parktronik, LOW);
      digitalWrite(Parktronik, LOW);
      digitalWrite(Parktronik, LOW);       
       Serial.println(" ");
    }
  
  
  // работа парктроника
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  cm = (duration/2) / 29.1;
  inches = (duration/2) / 74;
  BTserial.print(cm); //отправляем полученные данные по блютуз
  
  // выводим данные в монитор порта
  Serial.print(cm); 
  Serial.print("cm");
  Serial.println();
  
  delay(500);
  

    // Считываем данные с блютуз и отправляем в Serial monitor
    if (BTserial.available())
    {
        c = BTserial.read();
        Serial.write(c);               
    }

    

     if(c == 'e') {
      //Если пришёл символ "е" включаем светодиоды         
       digitalWrite(Led, HIGH); 
       digitalWrite(Led1, HIGH);
       digitalWrite(Led2, HIGH);
       
    }
     else{
      //Иначе отключаем светодиоды
      digitalWrite(Led, LOW);
      digitalWrite(Led1, LOW);
      digitalWrite(Led2, LOW);      
       
     }

    
    // выключаем светодиоды кнопкой в приложении
    if(c == 'd') {
     digitalWrite(Led, LOW);
      digitalWrite(Led1, LOW);
      digitalWrite(Led2, LOW);       
      
    }
   


    
    //если приходит "с" - отключаем реле
    if(c == 'c') {           
       digitalWrite(Right, LOW);  
       digitalWrite(Left, LOW);       
           
    }
    
    //если приходит "о" - включаем реле
    if(c == 'o') {                  
       digitalWrite(Right, HIGH);
       digitalWrite(Left, HIGH);      
        
          
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
