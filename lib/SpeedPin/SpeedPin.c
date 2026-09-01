#include <SpeedPin.h>

volatile bool INTERRUPT_TRIGGERED = false;
volatile uint8_t INTERRUPT_PIN_NUMBER = INVALID_PIN;

void IRAM_ATTR encoder_isr_handler(void *arg);
void SP_Interrupt_Handle(uint8_t pinInterrupted);

void SP_PinConfig(uint8_t pin, PinDirection direction, PinPullConfig pullConfig)
{
    gpio_config_t io_conf = {};
    // Pin Number
    gpio_num_t pinCasted = (gpio_num_t)(pin);
    io_conf.pin_bit_mask = (1ULL << pinCasted);
    // Direction
    switch (direction)
    {
    case SP_DIRECTION_INPUT:
        io_conf.mode = GPIO_MODE_INPUT;
        break;
    case SP_DIRECTION_OUTPUT:
        io_conf.mode = GPIO_MODE_OUTPUT;
        break;
    case SP_DIRECTION_INPUT_OUTPUT:
        io_conf.mode = GPIO_MODE_INPUT_OUTPUT;
        break;
    default:
        break;
    }
    // Pull Configuration
    switch (pullConfig)
    {
    case SP_PULL_NONE:
        io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
        io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
        break;
    case SP_PULLUP:
        io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
        io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
        break;
    case SP_PULLDOWN:
        io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
        io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
        break;
    default:
        break;
    }

    // setting configuration
    gpio_config(&io_conf);
}