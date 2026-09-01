#pragma once
#include <stdint.h>
#include <NGUI.hpp>
#include <Func_Display.hpp>
#include <ScreenHandlers.hpp>
#include <Func_Sensor.hpp>

// GUI Location
extern uint8_t CurrentBranchChildIndex;

void SwitchPushed();
void RotaryC();
void RotaryCC();
void DrawGUI();