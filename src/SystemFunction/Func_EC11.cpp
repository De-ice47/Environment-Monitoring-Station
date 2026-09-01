#include <Func_EC11.hpp>

ESP32Encoder encoder;

void Trigger_Rot();
void Trigger_Key();
void Int_Rot();
Callback cb_CC_Rot;
Callback cb_C_Rot;
Callback cb_Key;

unsigned long lastSwitchPress;
unsigned long lastSignal;

volatile bool callRot;

void SetupEC11()
{
    ESP32Encoder::useInternalWeakPullResistors = puType::up;

    encoder = ESP32Encoder();
    // use pin 19 and 18 for the encoder
    encoder.attachHalfQuad(10, 11);
    // set starting count value after attaching
    encoder.setCount(0);

    SP_PinConfig(12, SP_DIRECTION_INPUT, SP_PULLUP);
    attachInterrupt(digitalPinToInterrupt(12), Trigger_Key, RISING);
    SP_PinConfig(11, SP_DIRECTION_INPUT, SP_PULLUP);
    attachInterrupt(digitalPinToInterrupt(11), Int_Rot, RISING);
    SP_PinConfig(10, SP_DIRECTION_INPUT, SP_PULLUP);
    attachInterrupt(digitalPinToInterrupt(10), Int_Rot, RISING);
    // set the lastToggle
    LL::Println("[EC11] Encoder Ready");
}
void LoopEC11()
{
    if(callRot){
        callRot = false;
        Trigger_Rot();
    }
}

void Set_CC_Rotation_Callback(Callback cb)
{
    cb_CC_Rot = cb;
}
void Set_C_Rotation_Callback(Callback cb)
{
    cb_C_Rot = cb;
}
void Set_Key_Callback(Callback cb)
{
    cb_Key = cb;
}

void Int_Rot(){
    callRot = true;
}

void Trigger_Rot()
{
    if(millis() - lastSignal < 200) return;
    int8_t count = encoder.getCount();
    lastSignal = millis();
    if (count > 0)
    {
        if (cb_C_Rot)
            cb_C_Rot();
        Serial.println("C");
        
    }
    else if(count < 0)
    {
         if (cb_CC_Rot)
            cb_CC_Rot();
        Serial.println("CC");
    }
    encoder.clearCount();
}
void Trigger_Key()
{
    if (millis() - lastSwitchPress < 200)
        return;
    lastSwitchPress = millis();
    if (cb_Key)
        cb_Key();
    Serial.println("Switch Pressed");
}