#pragma once
#include <Arduino.h>
#include <Preferences.h>
class Prefs {
private:
    Preferences prefs;
public:
    void save(double lat, double lon); // сохранение координат в память 
    void load(double &lat, double &lon); // загрузка координат из памяти, если данных нет - возвращает 0,0
};