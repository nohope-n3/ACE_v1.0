#include "MotorControl.h"

// Initialization function for the motor device driver set.
void MotorControl::Init() {
    // Setting pins for motor control as output pins.
    pinMode(PIN_Motor_PWMA, OUTPUT);
    pinMode(PIN_Motor_PWMB, OUTPUT);
    pinMode(PIN_Motor_AIN_1, OUTPUT);
    pinMode(PIN_Motor_BIN_1, OUTPUT);
    pinMode(PIN_Motor_STBY, OUTPUT);
}

// Function to control the motor.
void MotorControl::SettingMotor(uint8_t direction_A, uint8_t speed_A,
                                uint8_t direction_B, uint8_t speed_B,
                                boolean controlMode) {

    // Check if control enable
    if (controlMode == control_enable) {
        // Enable motor control pin
        digitalWrite(PIN_Motor_STBY, HIGH);

        // Motor A control
        {
            // Direction control
            switch (direction_A) {
            case direction_forward:
                // Set motor A direction to forward
                digitalWrite(PIN_Motor_AIN_1, HIGH);
                // Set motor A speed
                analogWrite(PIN_Motor_PWMA, speed_A);
                break;
            case direction_backward:
                // Set motor A direction to backward
                digitalWrite(PIN_Motor_AIN_1, LOW);
                // Set motor A speed
                analogWrite(PIN_Motor_PWMA, speed_A);
                break;
            default:
                // Stop motor A
                analogWrite(PIN_Motor_PWMA, 0);
                // Disable motor control
                digitalWrite(PIN_Motor_STBY, LOW);
                break;
            }
        }

        // Motor B control
        {
            switch (direction_B) {
            case direction_forward:
                // Set motor B direction to forward
                digitalWrite(PIN_Motor_BIN_1, HIGH);
                // Set motor B speed
                analogWrite(PIN_Motor_PWMB, speed_B);
                break;
            case direction_backward:
                // Set motor B direction to backward
                digitalWrite(PIN_Motor_BIN_1, LOW);
                // Set motor B speed
                analogWrite(PIN_Motor_PWMB, speed_B);
                break;
            default:
                // Stop motor B
                analogWrite(PIN_Motor_PWMB, 0);
                // Disable motor control
                digitalWrite(PIN_Motor_STBY, LOW);
                break;
            }
        }
    } else {
        // Disable motor control
        digitalWrite(PIN_Motor_STBY, LOW);
        return;
    }
}