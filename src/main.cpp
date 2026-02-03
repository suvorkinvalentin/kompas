#define ENABLE_GxEPD2_GFX 0
#define GxEPD2_DEBUG 0
#include <Arduino.h>
#include "imu.h"
#include "display.h"
#include "gps.h"
#include "ble.h"
#include "b.h"
#include "save.h"
Compass compass;
GPS gps;
Ble ble;
Stick stick;
Prefs prefs;
unsigned long lastDataSend  = 0;
const unsigned long dataInterval      = 1000;
int prevMode=0;
double lat2;
double lng2;

void setup(){
unsigned long start_time = millis();
while (millis() - start_time < 1000) {}
Serial.begin(115200);
delay(500);
Serial.println("Serial ready!");
gps.start();
Serial.println("GPS ready!");
compass.begin();
Serial.println("IMU ready!");
delay(500);
display.wakeup();
Serial.println("DISPLAY ready!");
delay(500);
//display.dprint(String(42));
display.cleanup();
pinMode(0, INPUT);
pinMode(9, INPUT_PULLUP);

prefs.load(lat2,lng2);
}

void loop(){
unsigned long now = millis();
compass.update();
gps.work();

int nowMode=stick.check();
bool ButtonState=stick.Bcheck(); // 0 = не нажата, 1 = нажата
if (!(prevMode==nowMode)){
    prevMode=nowMode;
    display.cleanup();
}
if (now - lastDataSend >= dataInterval) {
    lastDataSend = now;

    gps.print();

    float yaw=compass.getYaw();
    float yawr=compass.getYawr();
    double lat1=55.972643; //gps.getlat();
    double lng1=37.174057; //gps.getlng();


    Serial.println(yaw);
    Serial.println(yawr);
    //display.dprint(String(yaw),5,5);
    //display.arrow(yawr);
    //display.update(yawr,String(yaw),140,5);
    if(nowMode==0){
        if (ButtonState==true){
            display.cleanup();
            display.dprint(String("Started"),120,64);
            compass.calibrateIMU();
            display.cleanup();
        }
        else{

            display.dprint(String(compass.getCal()),100,10);
            display.dprint(String("Press button to start calibrating"),40,57);
        }
    }
    if(nowMode==1){
        if (ButtonState==true){
        display.cleanup();
        display.dprint(String("Started"),120,57);
        ble.waitForTargetCoords();
        lat2=ble.getTargetLat();
        lng2=ble.getTargetLon();
        prefs.save(lat2,lng2);

        }
        else{
            display.dprint(String("Press button to start receiving"),40,57);
        }
    }
    double angle=compass.getAngleTo(lat1,lng1,lat2,lng2);
    if(nowMode==2){display.update(angle,String("Saved"),140,0, String(angle*RAD_TO_DEG),140,8);} // режим запомненных координат
    if(nowMode==3){display.update(yawr,String("Magnetic North"),98,0, String(yawr*RAD_TO_DEG),140,8);} // режим магнитного севера
    if(nowMode==4){display.update(yawr-compass.magnetDecl,String("True North"),120,0, String((yawr-compass.magnetDecl)*RAD_TO_DEG),140,8);} // режим географического севера
}
}