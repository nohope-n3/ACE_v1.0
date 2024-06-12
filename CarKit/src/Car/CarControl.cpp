#include "MotorControl.h"

class CarControl {
  private:
    static MotorControl motorControl;

  public:
    CarControl carControl() {}
    // Enumeration defining Conqueror Car motion control directions.
    enum CarMotion {
        Forward = 1,   //(1) Forward motion
        Backward,      //(2) Backward motion
        Left,          //(3) Left motion
        Right,         //(4) Right motion
        LeftForward,   //(5) Diagonal left forward motion
        LeftBackward,  //(6) Diagonal left backward motion
        RightForward,  //(7) Diagonal right forward motion
        RightBackward, //(8) Diagonal right backward motion
        Stop           //(9) Stop motion
    };

    static void Init() {
        motorControl.Init();
    }

    static void CarMotionControl(CarMotion direction, uint8_t speed) {
        switch (direction) {
        case Forward:
            // Control motor to move forward
            motorControl.SettingMotor(direction_forward, speed, direction_forward, speed, control_enable);
            break;
        case Backward:
            // Control motor to move backward
            motorControl.SettingMotor(direction_backward, speed, direction_backward, speed, control_enable);
            break;
        case Left:
            // Control motor to turn left
            motorControl.SettingMotor(direction_forward, 0, direction_forward, speed, control_enable);
            break;
        case Right:
            // Control motor to turn right
            motorControl.SettingMotor(direction_forward, speed, direction_forward, 0, control_enable);
            break;
        case LeftForward:
            // Control motor for diagonal left forward motion
            motorControl.SettingMotor(direction_forward, speed-22, direction_forward, speed+22, control_enable);
            break;
        case LeftBackward:
            // Control motor for diagonal left backward motion
            motorControl.SettingMotor(direction_backward, 0, direction_backward, speed, control_enable);
            break;
        case RightForward:
            // Control motor for diagonal right forward motion
            motorControl.SettingMotor(direction_forward, speed+22, direction_forward, speed-22, control_enable);
            break;
        case RightBackward:
            // Control motor for diagonal right backward motion
            motorControl.SettingMotor(direction_forward, speed, direction_backward, 0, control_enable);
            break;
        // case Stop:
        //     motorControl.SettingMotor(direction_standby, 0, direction_standby, 0, control_disable);
        //     break;
        default:
            // Stop motion
            motorControl.SettingMotor(direction_standby, 0, direction_standby, 0, control_disable);
            break;
        }
    }

    static void HandleSerialCommand() {
        if (Serial.available() > 0) {
            // Read the entire message until newline character is received
            String motionCmd = Serial.readStringUntil('\n'); // Corrected variable name and removed duplicate declaration
            // Print the received message
            Serial.print("Received command: ");
            Serial.println(motionCmd);

            // Find positions of '=' and '+'
            size_t equalPos = motionCmd.indexOf('=');
            size_t plusPos = motionCmd.indexOf('+');

            if (equalPos != -1 && plusPos != -1) {
                // Extract direction and speed substrings
                String directionStr = motionCmd.substring(equalPos + 1, plusPos);
                String speedStr = motionCmd.substring(plusPos + 1);

                // Convert direction and speed to integer values
                int direction = directionStr.toInt();
                int speed = speedStr.toInt();
                CarMotion enumDirection = static_cast<CarMotion>(direction);
                // Control car motion based on received command
                CarMotionControl(enumDirection, speed);
            }
        }
    }
};
