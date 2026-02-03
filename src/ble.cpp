#include "ble.h"

/* ===== Callbacks Classes ===== */

// Класс для отслеживания подключения
class ServerCallbacks : public BLEServerCallbacks {
    Ble* _ble; // Указатель на наш основной класс
public:
    ServerCallbacks(Ble* ble) : _ble(ble) {}

    void onConnect(BLEServer* pServer) override {
        _ble->clientConnected = true; // Доступно, так как класс friend
        Serial.println("BLE connected");
    }

    void onDisconnect(BLEServer* pServer) override {
        _ble->clientConnected = false;
        Serial.println("BLE disconnected");
        // Чтобы реклама (advertising) возобновилась при разрыве, можно добавить:
        // BLEDevice::startAdvertising(); 
    }
};

// Класс для приема данных
class RXCallback : public BLECharacteristicCallbacks {
    Ble* _ble; // Указатель на наш основной класс
public:
    RXCallback(Ble* ble) : _ble(ble) {}

    void onWrite(BLECharacteristic *pCharacteristic) override {
        // === ИСПРАВЛЕНИЕ ОШИБКИ ===
        // 1. Получаем std::string (как дает библиотека)
        std::string rxValue = pCharacteristic->getValue();

        // 2. Если пусто - выходим
        if (rxValue.length() == 0) return;

        // 3. Преобразуем std::string в Arduino String ПРАВИЛЬНО
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
    // Сброс координат, если нужно
    targetLat = 0;
    targetLon = 0;

    BLEDevice::init("ESP32-NAV");

    pServer = BLEDevice::createServer();
    // Передаем 'this' (текущий объект Ble) в конструктор колбэка
    pServer->setCallbacks(new ServerCallbacks(this));

    BLEService *service = pServer->createService(SERVICE_UUID);

    BLECharacteristic *rxChar = service->createCharacteristic(
        CHARACTERISTIC_RX,
        BLECharacteristic::PROPERTY_WRITE
    );
    // Передаем 'this' в конструктор колбэка
    rxChar->setCallbacks(new RXCallback(this));

    service->start();

    BLEAdvertising *adv = BLEDevice::getAdvertising();
    adv->addServiceUUID(SERVICE_UUID);
    adv->setScanResponse(true);
    adv->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    adv->setMinPreferred(0x12);
    BLEDevice::startAdvertising();

    Serial.println("Waiting for site connection...");

    while (!clientConnected) {
        delay(50);
        // Можно добавить таймаут или yield()
    }

    Serial.println("Waiting for coordinates...");

    while (!coordsReceived) {
        delay(50);
    }

    // Когда получили координаты:
    Serial.println("Coordinates received. Stopping BLE...");
    
    // Останавливаем рекламу и сервис, чтобы не мешать дальнейшей работе
    // (опционально, зависит от задачи)
    adv->stop(); 
    pServer->disconnect(0); 
}

float Ble::getTargetLat() {
    return targetLat;
}

float Ble::getTargetLon() {
    return targetLon;
}