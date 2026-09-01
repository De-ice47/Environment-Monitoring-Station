#include <stdint.h>
#include <stdlib.h>
#include <driver/gpio.h>
#include <Arduino.h>

class EC11
{
public:
    typedef void (*Callback)();

    EC11();
    EC11(uint8_t pinS1, uint8_t pinS2, uint8_t pinKey);

    void Loop();

    void Set_CC_Rotation_Callback(Callback cb);
    void Set_C_Rotation_Callback(Callback cb);
    void Set_Key_Callback(Callback cb);
    private:
    gpio_num_t pinS1;
    gpio_num_t pinS2;
    gpio_num_t pinKey;

    int lastState;

    Callback cb_CC_Rot;
    Callback cb_C_Rot;
    Callback cb_Key;

    void Trigger_CC_Rot();
    void Trigger_C_Rot();
    void Trigger_Key();
};