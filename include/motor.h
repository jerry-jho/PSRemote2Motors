#include <arduino.h>

class Motor {
public:
  Motor(int PWM_RES=8, int PWM_FREQ=5000) {
    _PWM_RES = PWM_RES;
    _PWM_FREQ = PWM_FREQ;
    _PWM_A = 0;
    _PWM_B = 1;
  }
  void begin(int PIN_STDBY, int PIN_PWMA, int PIN_AIN1, int PIN_AIN2, int PIN_PWMB, int PIN_BIN1, int PIN_BIN2) {
    _PIN_STDBY = PIN_STDBY;
    _PIN_PWMA = PIN_PWMA;
    _PIN_AIN1 = PIN_AIN1;
    _PIN_AIN2 = PIN_AIN2;
    _PIN_PWMB = PIN_PWMB;
    _PIN_BIN1 = PIN_BIN1;
    _PIN_BIN2 = PIN_BIN2;
    if (_PIN_STDBY!=0) pinMode(_PIN_STDBY, OUTPUT);
    
    pinMode(_PIN_PWMA, OUTPUT);
    pinMode(_PIN_AIN1, OUTPUT);
    pinMode(_PIN_AIN2, OUTPUT);

    pinMode(_PIN_PWMB, OUTPUT);   
    pinMode(_PIN_BIN1, OUTPUT);
    pinMode(_PIN_BIN2, OUTPUT);  

    ledcSetup(_PWM_A, _PWM_FREQ, _PWM_RES);
    ledcAttachPin(_PIN_PWMA, _PWM_A);
    ledcSetup(_PWM_B, _PWM_FREQ, _PWM_RES);
    ledcAttachPin(_PIN_PWMB, _PWM_B);

  }

  void set_stdby(int s) {
    if (_PIN_STDBY!=0) digitalWrite(_PIN_STDBY, s);
  }

  void set_pin_a(int a1, int a2) {
    if (_PIN_AIN1 != _PIN_PWMA) digitalWrite(_PIN_AIN1, a1);
    if (_PIN_AIN2 != _PIN_PWMA) digitalWrite(_PIN_AIN2, a2); 
  }

  void set_pin_b(int b1, int b2) {    
    if (_PIN_BIN1 != _PIN_PWMB) digitalWrite(_PIN_BIN1, b1);
    if (_PIN_BIN2 != _PIN_PWMB) digitalWrite(_PIN_BIN2, b2);  
  }

  void stop() {
    set_stdby(0);
    ledcWrite(_PWM_A, 0);
    ledcWrite(_PWM_B, 0);
    set_pin_a(0, 0);
    set_pin_b(0, 0);
  }

  int fp2i(float fp) {
    if (fp < 0) return 0;
    int r = (1<<_PWM_RES) * fp;
    if (r > ((1<<_PWM_RES) - 1)) r = ((1<<_PWM_RES) - 1);
    return r;
  }

  void a_forward(float speed = 1.0) {
    set_stdby(1);
    ledcWrite(_PWM_A, fp2i(speed));
    set_pin_a(1, 0);
  }

  void a_stop() {
    ledcWrite(_PWM_A, 0);
    set_pin_a(0, 0);
  }

  void a_backward(float speed = 1.0) {
    set_stdby(1);
    ledcWrite(_PWM_A, fp2i(speed));
    set_pin_a(0, 1);
  }

  void b_forward(float speed = 1.0) {
    set_stdby(1);
    ledcWrite(_PWM_B, fp2i(speed));
    set_pin_b(1, 0);
  }

  void b_stop() {
    ledcWrite(_PWM_B, 0);
    set_pin_b(0, 0);
  }

  void b_backward(float speed = 1.0) {
    set_stdby(1);
    ledcWrite(_PWM_B, fp2i(speed));
    set_pin_b(0, 1);
  }

private:
  int _PWM_RES;
  int _PWM_FREQ;
  int _PWM_A;
  int _PWM_B;

  int _PIN_STDBY;
  int _PIN_PWMA;
  int _PIN_AIN1;
  int _PIN_AIN2;
  int _PIN_PWMB;
  int _PIN_BIN1;
  int _PIN_BIN2;
};