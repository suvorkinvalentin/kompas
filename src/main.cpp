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
Prefs prefs;
unsigned long lastDataSend  = 0;
const unsigned long dataInterval      = 1000; //частота обновления экрана
int prevMode=0;
double lat2;
double lng2;
double lat0;
double lng0;
double angle0;
bool DisplayFlag=true;
double prevAngle=0;
double angle;

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
display.cleanup();
pinMode(0, INPUT);
pinMode(13, INPUT_PULLUP);

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
    DisplayFlag=true;
    display.cleanup();
}
if (now - lastDataSend >= dataInterval) {
    lastDataSend = now;

    //gps.print();

    float yaw=compass.getYaw();
    float yawr=compass.getYawr();
    //double lat1=55.972643;
    //double lng1=37.174057;
    double lat1=gps.getlat();
    double lng1=gps.getlng();
    switch(nowMode){
    case 0: // режим калибровки
        if (ButtonState==true){
            display.cleanup();
            display.dprint(("Started"),120,64);
            compass.calibrateIMU();
            display.cleanup();
            DisplayFlag=true;
        }
        else{
            if (DisplayFlag){
                DisplayFlag=false;
                display.dprint(String(compass.getCal()),145,10);
                display.dprint(("Press button to start calibrating"),50,57);
        }}
        break;

    case 1: // режим получения координат цели
        if (ButtonState==true){
        display.cleanup();
        display.dprint(("Started"),120,57);
        ble.waitForTargetCoords();
        lat2=ble.getTargetLat();
        lng2=ble.getTargetLon();
        prefs.save(lat2,lng2);
        DisplayFlag=true;
        }
        else{
            if (DisplayFlag){
                DisplayFlag=false;
                display.dprint(("Press button to start receiving target coordinates"),0,57);
        }}
        break;
    
    case 2: // режим получения начальных координат
        if (ButtonState==true){
        display.cleanup();
        display.dprint(("Started"),120,57);
        ble.waitForTargetCoords();
        lat0=ble.getTargetLat();
        lng0=ble.getTargetLon();
        DisplayFlag=true;
        }
        else{
            if (DisplayFlag){
                DisplayFlag=false;
                display.dprint(("Press button to start receiving start coordinates"),0,57);
        }}
        break;
    
    case 3: // режим сохранения текущих координат
        if (ButtonState==true){
        display.cleanup();
        display.dprint(("Saved"),120,57);
        lat2=ble.getTargetLat();
        lng2=ble.getTargetLon();
        prefs.save(lat2,lng2);
        }
        else{
            if (DisplayFlag){
                DisplayFlag=false;
                display.dprint(("Press button to save current coordinates"),30,57); 
        }}
        break;
    
    case 4: // режим отображения информации
        int nowBMode=stick.BModecheck();
        //Serial.print("BMode: ");Serial.println(nowBMode);
        switch(nowBMode){
        case 0:
            angle=compass.getAngleTo(lat1,lng1,lat2,lng2);
            if (abs(angle-prevAngle)>0.02){
                    prevAngle=angle;
                    display.update(angle,("Saved gps"),140,0, String(angle*RAD_TO_DEG),140,8);} // режим запомненных координат
            break;
        case 3:
            if (abs(yawr-prevAngle)>0.02){
                prevAngle=yawr;
                display.update(yawr,("Magnetic North"),110,0, String(yawr*RAD_TO_DEG),140,8);} // режим магнитного севера
            break;
        case 2:
            if (abs(yawr-compass.magnetDecl-prevAngle)>0.02){
                prevAngle=yawr-compass.magnetDecl;
                display.update(yawr-compass.magnetDecl,("True North"),120,0, String((yawr-compass.magnetDecl)*RAD_TO_DEG),140,8);} // режим географического севера
            break;
        case 1:
            angle0=compass.getAngleTo(lat0,lng0,lat2,lng2);
            if (abs(angle0-prevAngle)>0.02){
                prevAngle=angle0;
                display.update(angle0,("Saved no gps"),110,0, String(angle0*RAD_TO_DEG),140,8);}} // режим запомненных координат относительно начальной точки
            break;
        break;
}
}
}