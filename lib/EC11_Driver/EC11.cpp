#include <EC11.hpp>

gpio_num_t pinS1;
gpio_num_t pinS2;
gpio_num_t pinKey;

EC11::Callback cb_CC_Rot;
EC11::Callback cb_C_Rot;
EC11::Callback cb_Key;

int lastState;
uint8_t lastKeyState;

EC11::EC11(uint8_t pinS1, uint8_t pinS2, uint8_t pinKey)
{
    this->pinS1 = static_cast<gpio_num_t>(pinS1);
    this->pinS2 = static_cast<gpio_num_t>(pinS2);
    this->pinKey = static_cast<gpio_num_t>(pinKey);
    gpio_set_direction(this->pinS1, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(this->pinS1, GPIO_PULLUP_ONLY);
    gpio_set_direction(this->pinS2, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(this->pinS2, GPIO_PULLUP_ONLY);
    gpio_set_direction(this->pinKey, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(this->pinKey, GPIO_PULLUP_ONLY);

    lastState = (gpio_get_level(this->pinS1) << 1) | gpio_get_level(this->pinS2);
}
EC11::EC11() : EC11(0, 0, 0) {}

void EC11::Loop()
{
    int state = (gpio_get_level(this->pinS1) << 1) | gpio_get_level(this->pinS2);
    Serial.println("Looping");
    if(state != lastState){
        Serial.println("Got here");
        if ((lastState == 0b00 && state == 0b01) ||
            (lastState == 0b01 && state == 0b11) ||
            (lastState == 0b11 && state == 0b10) ||
            (lastState == 0b10 && state == 0b00))
        {
            Trigger_C_Rot();
            Serial.println("C");
        }

        if ((lastState == 0b00 && state == 0b10) ||
            (lastState == 0b10 && state == 0b11) ||
            (lastState == 0b11 && state == 0b01) ||
            (lastState == 0b01 && state == 0b00))
        {
            Trigger_CC_Rot();
            Serial.println("CC");
        }

        lastState = state;
    }
    if(gpio_get_level(pinKey) == 0 && lastKeyState == 0){
        Trigger_Key();
        lastKeyState = 1;
        Serial.println("Key");
    }
    if(gpio_get_level(pinKey) == 1){
        lastKeyState = 0;
    }
}
void EC11::Set_CC_Rotation_Callback(Callback cb){
    cb_CC_Rot = cb;
}
void EC11::Set_C_Rotation_Callback(Callback cb){
    cb_C_Rot = cb;
}
void EC11::Set_Key_Callback(Callback cb){
    cb_Key = cb;
}
void EC11::Trigger_CC_Rot()
{
    if (cb_CC_Rot)
        cb_C_Rot();
}
void EC11::Trigger_C_Rot()
{
    if (cb_C_Rot)
        cb_C_Rot();
}
void EC11::Trigger_Key()
{
    if (cb_Key)
        cb_Key();
}