#pragma once
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

// Предварительное объявление классов колбэков,
// чтобы мы могли объявить их "друзьями" (friend) класса Ble
class ServerCallbacks;
class RXCallback;

class Ble {
private:
    // UUID лучше держать константами здесь или в cpp
    const char* SERVICE_UUID = "6e400001-b5a3-f393-e0a9-e50e24dcca9e";
    const char* CHARACTERISTIC_RX = "6e400002-b5a3-f393-e0a9-e50e24dcca9e";

    float targetLat = 0;
    float targetLon = 0;

    bool coordsReceived = false;
    bool clientConnected = false;

    BLEServer* pServer = nullptr;

    // Объявляем эти классы друзьями, чтобы они могли менять приватные переменные (targetLat и т.д.)
    friend class ServerCallbacks;
    friend class RXCallback;

public:
    void waitForTargetCoords();
    
    // Эти функции должны возвращать float, а не void
    float getTargetLat();
    float getTargetLon();
};