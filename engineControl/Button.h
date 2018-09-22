#ifndef _BUTTON_H_
#define _BUTTON_H_


//класс кнопка
class Button {
  public:
    Button(const int pin);   
    void init();    
    bool read();
    bool onChange();    
    bool onPress();
    bool onRelease();
    

  private:
    const int m_pin;
    bool m_lastButtonState; 
    long m_lastDebounceTime = 0;  
    const int m_debounceDelay = 50;
    bool m_pressFlag = 0;

};

#endif 
