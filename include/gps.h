#pragma once

#include <Arduino.h>
#include <TinyGPSPlus.h>
class GPS {
private:
    TinyGPSPlus gps;
public:
    void start(); // инициализация GPS
    void work(); // обработка данных GPS
    void print(); // вывод данных GPS в Serial
    double getlat(); // получение широты
    double getlng(); // получение долготы
};