#pragma once
#include <Arduino.h>
#include <Preferences.h>
class Prefs {
private:
    Preferences prefs;
public:
    void save(double lat, double lon);      
    void load(double &lat, double &lon);
};