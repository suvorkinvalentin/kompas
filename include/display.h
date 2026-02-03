#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <GxEPD2_BW.h>
class Display{
private:
    GxEPD2_BW<GxEPD2_290_BS, GxEPD2_290_BS::HEIGHT> display;
    void udprint(String txt,int x,int y);
    void uarrow(float yaw);
public:
    Display();
    void wakeup();
    void dprint(String txt,int,int);
    void arrow(float yaw);
    void update(float yawr, String txt,int x,int y, String txt2,int x2,int y2);
    void cleanup();
};
extern Display display;