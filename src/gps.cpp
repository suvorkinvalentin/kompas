#include "gps.h"
HardwareSerial GPSSerial(1);
void GPS::start(){
    Serial.println("GPS started");
    GPSSerial.begin(9600, SERIAL_8N1, 12, 1);
}
void GPS::work(){
    while (GPSSerial.available()) {
    gps.encode(GPSSerial.read());
  }
}
void GPS::print(){
  if (gps.location.isValid()) {
    Serial.println("------------------------");
    Serial.print("Latitude:  ");
    Serial.println(gps.location.lat(), 6);

    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);

    Serial.print("Satellites: ");
    Serial.println(gps.satellites.value());

    Serial.print("HDOP: ");
    Serial.println(gps.hdop.hdop());

    Serial.println("------------------------");
  }
  else{
    Serial.print("Sats: ");
    Serial.println(gps.satellites.value());
  }

  if (gps.charsProcessed() < 30) {
    Serial.print("Нет данных от GPS.");
    Serial.println(gps.charsProcessed());

  }
}
double GPS::getlat(){if (gps.location.isValid()) {return gps.location.lat();}}
double GPS::getlng(){if (gps.location.isValid()) {return gps.location.lng();}}