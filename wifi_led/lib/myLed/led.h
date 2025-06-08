#ifndef __MY_LED_H__
#define __MY_LED_H__

//声明一个LED灯的类
class led{
  public:
    led(uint8_t PIN);
    uint8_t PIN;
    bool ledStuatus = 0;
    void On();
    void Off();
  private:
    void Init();
  };

#endif