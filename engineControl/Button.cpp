#include "Button.h"

#include <Arduino.h>


Button::Button(const int pin) : m_pin(pin)
{

}

void Button::init()
{

  pinMode(m_pin, INPUT);
  //начальное состояние
  m_lastButtonState = read();
}


//считываем
bool Button::read()
{
  return digitalRead(m_pin);

}



//Если кнопка не нажата - функция вернёт LOW (0).
//если нажата вернёт - HIGH (1).

bool Button::onChange()
{
  //состояние кнопки, 1 - нажат, 0 - не нажат
  bool reading = read();
  // если изменилось состояние из за шума, то есть дребезг контактов
  if (reading != m_lastButtonState) {
    m_lastDebounceTime = millis();
    m_pressFlag = 1;
  }

  if ((millis() - m_lastDebounceTime) > m_debounceDelay) {
    //если задержка больше то учитывать это нажатие
    if (m_pressFlag) { 
      m_pressFlag = 0;

      
      m_lastButtonState = reading;
      return 1;

    }
  }
  m_lastButtonState = reading;

  return 0;

}

bool Button::onPress()
{
  //состояние кнопки, 1 - нажат, 0 - не нажат
  bool reading = read();
  // если дребезг
  if (reading == HIGH && m_lastButtonState == LOW) {    
    m_lastDebounceTime = millis();
    m_pressFlag = 1;
  }

  if ((millis() - m_lastDebounceTime) > m_debounceDelay) {
     //если не дребезг   
    if (m_pressFlag) {      
      m_pressFlag = 0;
      m_lastButtonState = reading;
      return 1;

    }
  }
  m_lastButtonState = reading;

  return 0;

}

bool Button::onRelease()
{
  //состояние кнопки, 1 - нажат, 0 - не нажат
  bool reading = read();  
  if (reading == LOW && m_lastButtonState == HIGH) {    
    m_lastDebounceTime = millis();
    m_pressFlag = 1;
  }

  if ((millis() - m_lastDebounceTime) > m_debounceDelay) {
    
    if ( m_pressFlag) { //reading == LOW && m_lastButtonState == HIGH) {
      m_lastButtonState = reading;
      m_pressFlag = 0;
      return 1;

    }
  }
  m_lastButtonState = reading;

  return 0;

}
