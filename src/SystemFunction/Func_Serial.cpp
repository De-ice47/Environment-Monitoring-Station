#include <Func_Serial.hpp>

void SetupSerial(){
    Serial.begin(9600);
  LL::Initialize(SerialOutput);
  LL::Println("[ROOT] Starting Serial Output");
}

void SerialOutput(const char *message)
{
  Serial.println(message);
}