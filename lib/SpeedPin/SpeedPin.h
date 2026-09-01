#pragma once
#include <driver/gpio.h>

#define INVALID_PIN 0xFF


typedef enum {
    SP_DIRECTION_INPUT,
    SP_DIRECTION_OUTPUT,
    SP_DIRECTION_INPUT_OUTPUT
} PinDirection;
typedef enum {
    SP_PULL_NONE,
    SP_PULLUP,
    SP_PULLDOWN
} PinPullConfig;


#ifdef __cplusplus
extern "C" {
#endif
void SP_PinConfig(uint8_t pin, PinDirection direction, PinPullConfig pull);
#ifdef __cplusplus
}
#endif