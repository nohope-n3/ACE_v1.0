#include "CarControl.cpp"

CarControl carControl;

void setup() {
    Serial.begin(9600);
}

void loop() {
    carControl.HandleSerialCommand();
}
