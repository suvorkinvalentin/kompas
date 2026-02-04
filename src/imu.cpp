#include "imu.h"
#include "display.h"
#include "gps.h"
bool Compass::begin() {
     delay(100); //  Wait for BNO to boot
 // Start i2c and BNO080
    //Wire.flush();   // Reset I2C
    //sensor.begin(BNO080_DEFAULT_ADDRESS, Wire);
    Wire.begin(4, 5);
    Wire.setClock(400000);
    if (sensor.begin(BNO080_DEFAULT_ADDRESS, Wire)) {
        Serial.println("BNO080 found and initialized!");
        
        sensor.enableRotationVector(50);
        sensor.enableMagnetometer(50);
        delay(1500);
            Serial.println(sensor.getYaw());
            Serial.println(sensor.getGravityAccuracy());
            Serial.println(sensor.getMagX());
        
        yaw = 0;
        yawr = 0;
        return true;
    }
    else {
        Serial.println("BNO080 NOT detected! Check wiring.");
        return false;
    }

}
void Compass::update() {
    if (sensor.dataAvailable()) {
        yawr=((sensor.getYaw()));
        yaw=yawr * 180.0 / PI;
    }
}

double Compass::getYaw() {
    return yaw;

}
float Compass::getYawr() {
    return yawr;
}
void Compass::calibrateIMU() {
    Serial.println("Запуск калибровки магнитометра...");


    sensor.calibrateAll();  
    int m=sensor.getMagAccuracy();

    unsigned long start = millis();
    int n = 0;

    display.dprint(String(sensor.getMagAccuracy()),140,5);
    while (n < 100 && millis() - start < 90000) {
        if (sensor.dataAvailable()) {  // читаем новые отчёты с сенсора
            float d = sensor.getMagX();
            Serial.print("Magnetometer Accuracy: ");
            Serial.println(sensor.getMagAccuracy());
            Serial.print(sensor.getMagX()); Serial.print(", ");
            Serial.print(sensor.getMagY()); Serial.print(", ");
            Serial.println(sensor.getMagZ());
            if (m!=sensor.getMagAccuracy()){
                display.dprint(String(sensor.getMagAccuracy()),140,5);
                m=sensor.getMagAccuracy();
            }
            if (sensor.getMagAccuracy()==3){n++;}
            else{n=0;}
        }
        delay(20);
    }

    // Сохраняем калибровку во Flash
    if (n>=40){sensor.saveCalibration();
    Serial.println("Калибровка завершена и сохранена!");}
    else{Serial.println("Калибровка НЕзавершена и НЕсохранена!");}
    sensor.endCalibration();

}
double Compass::getAngleTo(double lat1, double lng1, double lat2, double lng2){
  double lat1r = lat1 * DEG_TO_RAD;
  double lat2r = lat2 * DEG_TO_RAD;
  double dLambda = (lng2 - lng1) * DEG_TO_RAD;

  double y = sin(dLambda) * cos(lat2r);
  double x = (cos(lat1r) * sin(lat2r)) - (sin(lat1r) * cos(lat2r) * cos(dLambda));

  double theta = atan2(y, x);
 //return theta
  double angle = theta + yawr - magnetDecl;

  if (angle >  PI) angle -= 2 * PI;
  if (angle < -PI) angle += 2 * PI;
  return angle;
}
int Compass::getCal(){
    return sensor.getMagAccuracy();
}