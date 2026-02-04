#include "display.h"
Display display;
Display::Display() 
  : display(GxEPD2_290_BS(10, 6, 7, 8)) 
{
}
void Display::wakeup(){
SPI.begin(
  3,   // SCK(scl)
  -1,  // MISO (не используется)
  2,   // MOSI(sda)
  10 //cs
  );
  display.init(115200,true,50,false);
  display.setRotation(1);
  display.setFont(0);
  display.setTextColor(GxEPD_BLACK);
  display.setFullWindow();
  display.firstPage();

}
void Display::cleanup(){
  display.setFullWindow();
  display.firstPage();
  display.fillScreen(GxEPD_WHITE);
  do {
    display.fillScreen(GxEPD_WHITE);
  } while(display.nextPage());
}
void Display::dprint(String txt,int x,int y){
int16_t tbx, tby; uint16_t tbw, tbh;
display.getTextBounds(txt, x, y, &tbx, &tby, &tbw, &tbh);
display.setPartialWindow(tbx, tby, tbw, tbh);
display.firstPage();
do {
    display.fillRect(tbx, tby, tbw, tbh, GxEPD_WHITE);
    display.setCursor(x, y);
    display.print(txt);
  } while(display.nextPage());
}

void Display::arrow(float yawr){
  display.setPartialWindow(98, 14, 100, 100);
  display.firstPage();
    do {
    display.fillRect(98, 14, 100, 100, GxEPD_WHITE);
    display.drawLine(148, 64, 148+50*sin(yawr), 64-50*cos(yawr), GxEPD_BLACK);
  } while(display.nextPage());

}
void Display::update(float yawr, String txt,int x,int y, String txt2, int x2, int y2){
  display.setPartialWindow(98, 0, 100, 128);
  display.firstPage();
    do {
    uarrow(yawr);
    udprint(txt,x,y);
    udprint(txt2,x2,y2);
  } while(display.nextPage());
}
void Display::uarrow(float yawr){
display.fillRect(98, 15, 100, 100, GxEPD_WHITE);
display.drawLine(148, 64, 148+50*sin(yawr), 64-50*cos(yawr), GxEPD_BLACK);
}

void Display::udprint(String txt,int x,int y){
int16_t tbx, tby; uint16_t tbw, tbh;
display.getTextBounds(txt, x, y, &tbx, &tby, &tbw, &tbh);
display.fillRect(tbx, tby, tbw, tbh, GxEPD_WHITE);
display.setCursor(x, y);
display.print(txt);
}