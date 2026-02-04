#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <GxEPD2_BW.h>
class Display{
private:
    GxEPD2_BW<GxEPD2_290_BS, GxEPD2_290_BS::HEIGHT> display;
    void udprint(String txt,int x,int y); // внутренний метод для вывода текста на дисплей
    void uarrow(float yaw); // внутренний метод для отображения стрелки направления
public:
    Display();
    void wakeup(); // пробуждение дисплея
    void dprint(String txt,int,int); // вывод текста на дисплей
    void arrow(float yaw); // отображение стрелки направления
    void update(float yawr, String txt,int x,int y, String txt2,int x2,int y2); // обновление дисплея с новыми данными
    void cleanup(); // очистка дисплея
};
extern Display display;