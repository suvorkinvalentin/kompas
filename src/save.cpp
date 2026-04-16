#include "save.h"
void Prefs::save(double lat, double lon) {
    prefs.begin("nav_data", false);

    prefs.putDouble("target_lat", lat);
    prefs.putDouble("target_lon", lon);
    
    prefs.end();
}
void Prefs::load(double &lat, double &lon){
    prefs.begin("nav_data", true);

    lat = prefs.getDouble("target_lat", 0.0);
    lon = prefs.getDouble("target_lon", 0.0);

    prefs.end();
}
