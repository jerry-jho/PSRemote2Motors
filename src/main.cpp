#include <Ps3Controller.h>
#include <Wire.h>
#include "motor.h"

int player = 0;
int battery = 0;

#define SINGLE_KEY_L 0 // left
#define SINGLE_KEY_R 1 // right
#define SINGLE_KEY_U 2 // up
#define SINGLE_KEY_D 3 // down
#define SINGLE_KEY_Q 4 // square
#define SINGLE_KEY_C 5 // circle
#define SINGLE_KEY_T 6 // triangle
#define SINGLE_KEY_X 7 // cross
#define SINGLE_KEY_S 8 // select
#define SINGLE_KEY_B 9 // start

void onConnect(){
    Serial.println("Connected.");
}

unsigned long tmr = 0L;

Motor motor;

void setup() {
    Serial.begin(115200);
    // //Ps3.attach(notify);
    Ps3.attachOnConnect(onConnect);
    Ps3.begin();
    Serial.println(Ps3.getAddress());
    Serial.println("Ready.");
    motor.begin(
        17, // all-switch
        25, // a-pwm
        27, // a1
        26, // a2
        19, // b-pwm
        5,  // b1
        18  // b2
    );
    tmr = millis();
    motor.stop();

}

int last_key = 0;



void loop() {
    delay(1);
    bool timeout = false;
    if (millis() - tmr > 2000) {
        tmr = millis();
        timeout = true;
    }
    if(!Ps3.isConnected()) {
        return;
    }
    int single_key = -1;
    if( abs(Ps3.data.button.left) ){
        motor.b_stop();
        motor.a_forward(1.0);
        return;
    }
    if( abs(Ps3.data.button.right) ){
        motor.a_stop();
        motor.b_forward(1.0);
        return;
    }
    if( abs(Ps3.data.button.up) ){
        int data_srx = Ps3.data.analog.stick.rx;
        if (abs(data_srx) < 20) {
            motor.a_forward(1.0);
            motor.b_forward(1.0);
        } else if (data_srx < 0) {
            motor.b_stop();
            data_srx = -2 * data_srx;
            motor.a_forward(data_srx * 1.0 / 256.0);
        } else {
            motor.a_stop();
            data_srx = 2 * data_srx;
            motor.b_forward(data_srx * 1.0 / 256.0);
        }

        
        return;
    }
    if( abs(Ps3.data.button.down) ){
        motor.a_backward(1.0);
        motor.b_backward(1.0);
        return;
    }

    int l2 = Ps3.data.analog.button.l2;
    int r2 = Ps3.data.analog.button.r2;
    if (l2 + r2 > 0) {
        if (l2 > 0) {
            float fl2 = l2 * 1.0 / 256.0;
            motor.a_forward(fl2);
        }
        if (r2 > 0) {
            float fr2 = r2 * 1.0 / 256.0;
            motor.b_forward(fr2);
        }
        return;
    }


    // float fr2 = Ps3.data.analog.button.r2 * 1.0 / 256.0;
    // Serial.printf("Key %d %d\n", l2, r2);
    // vTaskDelay(500);
    motor.stop();
    return;
    #if 0

    if( abs(Ps3.event.button_down.down) ){
        single_key = SINGLE_KEY_D;
    }
    if( Ps3.event.button_down.square) {
        single_key = SINGLE_KEY_Q;
    }
    if( Ps3.event.button_down.circle) {
        single_key = SINGLE_KEY_C;
    }
    if( Ps3.event.button_down.triangle) {
        single_key = SINGLE_KEY_T;
    }
    if( Ps3.event.button_down.cross) {
        single_key = SINGLE_KEY_X;
    }
    if( Ps3.event.button_down.select )
        single_key = SINGLE_KEY_S;
    if( Ps3.event.button_down.start )
        single_key = SINGLE_KEY_B;    

    if( abs(Ps3.event.analog_changed.button.l1)){
       single_key = SINGLE_KEY_L;
    }

    if( abs(Ps3.event.analog_changed.button.r1) ){
        single_key = SINGLE_KEY_R;
    }

    if( abs(Ps3.event.analog_changed.button.l2) ){
        single_key = SINGLE_KEY_L;
    }

    if( abs(Ps3.event.analog_changed.button.r2) ){
        single_key = SINGLE_KEY_R;
    }


    int data_srx = Ps3.data.analog.stick.rx;
    int data_sry = Ps3.data.analog.stick.ry;

    if( (abs(data_srx) > 10) || (abs(data_sry)) > 10 ){
        if (abs(data_srx) > abs(data_sry)) {
            if (data_srx > 0) {
                single_key = SINGLE_KEY_R;
            } else {
                single_key = SINGLE_KEY_L;
            }
        } else {
            if (data_sry > 0) {
                single_key = SINGLE_KEY_D;
            } else {
                single_key = SINGLE_KEY_U;
            }            
        }
    }

    int data_slx = Ps3.data.analog.stick.lx;
    int data_sly = Ps3.data.analog.stick.ly;

    if( (abs(data_slx) > 10) || (abs(data_sly)) > 10 ){
        if (abs(data_slx) > abs(data_sly)) {
            if (data_slx > 0) {
                single_key = SINGLE_KEY_R;
            } else {
                single_key = SINGLE_KEY_L;
            }
        } else {
            if (data_sly > 0) {
                single_key = SINGLE_KEY_D;
            } else {
                single_key = SINGLE_KEY_U;
            }            
        }
    }

    if (single_key != -1) {
        if (single_key == SINGLE_KEY_U) {
            motor.forward();
        } else if (single_key == SINGLE_KEY_D) {
            motor.backward();
        } else if (single_key == SINGLE_KEY_R) {
            motor.right();
        } else if (single_key == SINGLE_KEY_L) {
            motor.left();
        } else {
            motor.stop();
        }
    }

    static int single_key_last = -2;
    if (single_key != single_key_last) {
        single_key_last = single_key;
        if (single_key != -1) {
            Serial.printf("Key %d\n", single_key);
        }
    }

    #endif
    
}