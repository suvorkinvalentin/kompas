#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_BNO080_Arduino_Library.h>

class Compass {
private:
    BNO080 sensor;
    double yawr;
    float yaw;

public:
    bool begin();       // инициализация датчика
    void update();      // обновление yaw
    double getYaw();     // получение текущего yaw
    float getYawr();    // получение текущего yaw в радианах
    void calibrateIMU();    // калибровка датчика
    double getAngleTo(double lat1, double lng1, double lat2, double lng2);  // расчет угла до цели
    int getCal();      // получение статуса калибровки
    double magnetDecl=0.21; // магнитное склонение для Москвы, в радианах
};
