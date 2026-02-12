#pragma once
#include <Arduino.h>

class Stick{
    private:
    const int PIN_X = 0;
    const int PIN_B = 13;
    const int Modes = 5; // количество режимов
    int currentMode = 0;
    bool stickReady = true;

    const int THRESHOLD_LOW = 1000;
    const int THRESHOLD_HIGH = 3900;
    const int CENTER_MIN = 1500;
    const int CENTER_MAX = 3600;

public:
int check(); // проверка положения стика и переключение режимов
bool Bcheck(); // проверка состояния кнопки
};