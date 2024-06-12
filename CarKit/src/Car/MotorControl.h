#ifndef _Motor_Control_H_
#define _Motor_Control_H_

#include <arduino.h>

class MotorControl {
  public:
    void Init();

    // Motor control
    void SettingMotor(
        uint8_t direction_A, uint8_t speed_A, // Motor A parameters
        uint8_t direction_B, uint8_t speed_B, // Motor B parameters
        boolean controlMode                   // Enable motor control AB (true)
    );

  private:
    // Pin definitions for motor control
#define PIN_Motor_PWMA 5  // Pin for PWM signal to motor A
#define PIN_Motor_PWMB 6  // Pin for PWM signal to motor B
#define PIN_Motor_AIN_1 7 // Pin for direction control of motor A
#define PIN_Motor_BIN_1 8 // Pin for direction control of motor B
#define PIN_Motor_STBY 3  // Pin for standby mode control of the motor driver

  public:
    // Constants for motor control
#define speed_Max 255        // Maximum speed for the motor
#define direction_forward 1  // Direction forward
#define direction_backward 2 // Direction backward
#define direction_standby 3  // Direction backward

#define Duration_enable true   // Enable duration
#define Duration_disable false // Disable duration
#define control_enable true    // Control enable
#define control_disable false  // Control disable
};
#endif