#include "ble.h"

/* ===== Callbacks Classes ===== */

// Класс для отслеживания подключения
class ServerCallbacks : public BLEServerCallbacks {
    Ble* _ble; 
public:
    ServerCallbacks(Ble* ble) : _ble(ble) {}

    void onConnect(BLEServer* pServer) override {
        _ble->clientConnected = true; 
        Serial.println("BLE connected");
    }

    void onDisconnect(BLEServer* pServer) override {
        _ble->clientConnected = false;
        Serial.println("BLE disconnected");

    }
};

// Класс для приема данных
class RXCallback : public BLECharacteristicCallbacks {
    Ble* _ble; 
public:
    RXCallback(Ble* ble) : _ble(ble) {}

    void onWrite(BLECharacteristic *pCharacteristic) override {

        std::string rxValue = pCharacteristic->getValue();


        if (rxValue.length() == 0) return;


        String value = String(rxValue.c_str()); 

        Serial.print("RAW BLE: ");
        Serial.println(value);

        int comma = value.indexOf(',');
        if (comma == -1) return;

        // Теперь обновляем данные в классе
        _ble->targetLat = value.substring(0, comma).toFloat();
        _ble->targetLon = value.substring(comma + 1).toFloat();
        _ble->coordsReceived = true;

        Serial.print("TARGET LAT: ");
        Serial.println(_ble->targetLat, 6);
        Serial.print("TARGET LON: ");
        Serial.println(_ble->targetLon, 6);
    }
};

/* ===== CLASS METHODS ===== */

void Ble::waitForTargetCoords() {
    coordsReceived = false;
    clientConnected = false;

    targetLat = 0;
    targetLon = 0;

    BLEDevice::init("ESP32-NAV");

    pServer = BLEDevice::createServer();

    pServer->setCallbacks(new ServerCallbacks(this));

    BLEService *service = pServer->createService(SERVICE_UUID);

    BLECharacteristic *rxChar = service->createCharacteristic(
        CHARACTERISTIC_RX,
        BLECharacteristic::PROPERTY_WRITE
    );

    rxChar->setCallbacks(new RXCallback(this));

    service->start();

    BLEAdvertising *adv = BLEDevice::getAdvertising();
    adv->addServiceUUID(SERVICE_UUID);
    adv->setScanResponse(true);
    adv->setMinPreferred(0x06);  
    adv->setMinPreferred(0x12);
    BLEDevice::startAdvertising();

    Serial.println("Waiting for site connection...");

    while (!clientConnected) {
        delay(50);

    }

    Serial.println("Waiting for coordinates...");

    while (!coordsReceived) {
        delay(50);
    }

    // Когда получили координаты:
    Serial.println("Coordinates received. Stopping BLE...");
    

    adv->stop(); 
    pServer->disconnect(0); 
}

float Ble::getTargetLat() {
    return targetLat;
}

float Ble::getTargetLon() {
    return targetLon;
}