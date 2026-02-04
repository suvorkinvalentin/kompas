#pragma once
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>


class ServerCallbacks;
class RXCallback;

class Ble {
private:

    const char* SERVICE_UUID = "6e400001-b5a3-f393-e0a9-e50e24dcca9e";
    const char* CHARACTERISTIC_RX = "6e400002-b5a3-f393-e0a9-e50e24dcca9e";

    float targetLat = 0;
    float targetLon = 0;

    bool coordsReceived = false;
    bool clientConnected = false;

    BLEServer* pServer = nullptr;


    friend class ServerCallbacks;
    friend class RXCallback;

public:
    void waitForTargetCoords(); // блокирует выполнение, пока не будут получены координаты

    double getTargetLat(); // возвращает полученную широту
    double getTargetLon(); // возвращает полученную долготу
};