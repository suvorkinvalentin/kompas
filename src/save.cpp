#include "save.h"
void Prefs::save(double lat, double lon) {
    // 1. Открываем пространство имен "nav_data". 
    // false = режим чтения и записи (Read/Write)
    prefs.begin("nav_data", false);

    // 2. Записываем данные по ключам
    prefs.putDouble("target_lat", lat);
    prefs.putDouble("target_lon", lon);
    
    // 3. Закрываем
    prefs.end();
    
    Serial.println("Coordinates Saved!");
}
void Prefs::load(double &lat, double &lon){
    // 1. Открываем в режиме "Только чтение" (true)
    prefs.begin("nav_data", true);

    // 2. Читаем. Второй аргумент — значение по умолчанию, если память пуста
    lat = prefs.getDouble("target_lat", 0.0);
    lon = prefs.getDouble("target_lon", 0.0);

    // 3. Закрываем
    prefs.end();
    
    Serial.print("Loaded Lat: "); Serial.println(lat, 6);
    Serial.print("Loaded Lon: "); Serial.println(lon, 6);
}
