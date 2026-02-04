#include "save.h"
void Prefs::save(double lat, double lon) {
    prefs.begin("nav_data", false);

    prefs.putDouble("target_lat", lat);
    prefs.putDouble("target_lon", lon);
    
    prefs.end();
    
    Serial.println("Coordinates Saved!");
}
void Prefs::load(double &lat, double &lon){
    prefs.begin("nav_data", true);

    lat = prefs.getDouble("target_lat", 0.0);
    lon = prefs.getDouble("target_lon", 0.0);

    prefs.end();
    
    Serial.print("Loaded Lat: "); Serial.println(lat, 6);
    Serial.print("Loaded Lon: "); Serial.println(lon, 6);
}
