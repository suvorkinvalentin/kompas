#include "b.h"



int Stick::check(){
int xVal = analogRead(PIN_X);
if (xVal > THRESHOLD_HIGH && stickReady) {
        currentMode--;
        
        // Цикличность: если стало -1, переходим в 3
        if (currentMode < 0) {
            currentMode = Modes - 1;
        }
        Serial.print("Mode changed to: ");
        Serial.println(currentMode);

        stickReady = false; // Блокируем, пока стик не вернется в центр
    }
    // 2. Если стик отклонен ВЛЕВО и он был "готов"
else if (xVal < THRESHOLD_LOW && stickReady) {
        currentMode++;
        
       // Цикличность: если стало 4, сбрасываем в 0
        if (currentMode >= Modes) {
            currentMode = 0;
        }
        
        Serial.print("Mode changed to: ");
        Serial.println(currentMode);

        stickReady = false; // Блокируем
    }
    // 3. Если стик В ЦЕНТРЕ (мертвая зона), разрешаем новое переключение
else if (xVal > CENTER_MIN && xVal < CENTER_MAX) {
        stickReady = true;
    }
return currentMode;
}

bool Stick::Bcheck(){
    bool b=!(digitalRead(PIN_B));
    return b;
}