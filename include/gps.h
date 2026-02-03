#pragma once

#include <Arduino.h>
#include <TinyGPSPlus.h>
class GPS {
private:
    TinyGPSPlus gps;
public:
    void start();      
    void work();     
    void print();
    double getlat();
    double getlng();
};