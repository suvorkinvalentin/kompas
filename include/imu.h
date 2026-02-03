#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_BNO080_Arduino_Library.h>

class Compass {
private:
    BNO080 sensor;
    double yawr;
    float yaw;          // внутреннее состояние

public:
    bool begin();       // инициализация датчика
    void update();      // обновление yaw
    double getYaw();     // получение текущего yaw
    float getYawr();
    void calibrateIMU();
    double getAngleTo(double lat1, double lng1, double lat2, double lng2);
    int getCal();
    double magnetDecl=0.21;
};
