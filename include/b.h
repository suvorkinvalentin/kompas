#pragma once
#include <Arduino.h>

class Stick{
    private:
    const int PIN_X = 0;
    const int PIN_B = 9;
    const int Modes = 5; // количество режимов
    int currentMode = 0;
    bool stickReady = true;

    const int THRESHOLD_LOW = 1000;
    const int THRESHOLD_HIGH = 3800;
    const int CENTER_MIN = 3200;
    const int CENTER_MAX = 3400;

public:
int check();
bool Bcheck();
};