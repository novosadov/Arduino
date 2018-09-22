#include <Arduino.h>
#include "DCMDriverL298.h"

//Драйвер для двух двигателей
// enA, enB - PWM от Ардуино. 
DCMDriverL298::DCMDriverL298(const int enA, const int pinA1, const int pinA2, const int enB, const int pinB1, const int pinB2) : m_enA(enA), m_enB(enB), m_pinA1(pinA1), m_pinA2(pinA2), m_pinB1(pinB1), m_pinB2(pinB2)
{
	pinMode(m_pinA1,OUTPUT);
    pinMode(m_pinA2,OUTPUT);
    pinMode(m_pinB1,OUTPUT);
    pinMode(m_pinB2,OUTPUT);
    pinMode(m_enA,OUTPUT);
    pinMode(m_enB,OUTPUT);
    
 	stopMotors();
}

//Управление скоростью и направлением 1 двигателя
void DCMDriverL298::setMotorA(int speed, bool dir)
{
	setMotor(m_enA, m_pinA1, m_pinA2, speed, dir);
}

//Управление скоростью и направлением 2 двигателя
void DCMDriverL298::setMotorB(int speed, bool dir)
{
	setMotor(m_enB, m_pinB1, m_pinB2, speed, dir);
}

//остановка первого двигателя
void DCMDriverL298::stopMotorA()
{
	off(m_enA, m_pinA1, m_pinA2);
}

//остановка второго двигателя
void DCMDriverL298::stopMotorB()
{
	off(m_enB, m_pinB1, m_pinB2);
}

//остановка двух двигателей
void DCMDriverL298::stopMotors()
{
	stopMotorA();
	stopMotorB();
}

//Скорость двигателей и направление
void DCMDriverL298::setMotor(int pinPWM, int pinDir1, int pinDir2, int speed, bool dir)
{
	analogWrite(pinPWM, speed);
	digitalWrite(pinDir1, !dir);
	digitalWrite(pinDir2, dir);
}


//выключение движков
void DCMDriverL298::off(int pinPWM, int pinDir1, int pinDir2)
{
	analogWrite(pinPWM, 0);
	digitalWrite(pinDir1, LOW);
	digitalWrite(pinDir2, LOW);
}
