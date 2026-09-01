#include <Func_Serial.hpp>

void SetupSerial(){
    Serial.begin(115200);
  LL::Initialize(SerialOutput);
  LL::Println("[ROOT] Starting Serial Output");
}

void SerialOutput(const char *message)
{
  Serial.println(message);
}