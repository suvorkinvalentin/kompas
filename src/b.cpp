#include "b.h"



int Stick::check(){
int xVal = analogRead(PIN_X);
if (xVal > THRESHOLD_HIGH && stickReady) {
        currentMode--;
        
        if (currentMode < 0) {
            currentMode = Modes - 1;
        }
        Serial.print("Mode changed to: ");
        Serial.println(currentMode);

        stickReady = false; 
    }
else if (xVal < THRESHOLD_LOW && stickReady) {
        currentMode++;
        

        if (currentMode >= Modes) {
            currentMode = 0;
        }
        
        Serial.print("Mode changed to: ");
        Serial.println(currentMode);

        stickReady = false; 
    }
else if (xVal > CENTER_MIN && xVal < CENTER_MAX) {
        stickReady = true;
    }
return currentMode;
}

bool Stick::Bcheck(){
    bool b=!(digitalRead(PIN_B));
    return b;
}