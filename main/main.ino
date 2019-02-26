#include <Wire.h>
#include <Servo.h>
#include "common_defs/defs_enum.h"

Servo M0;
Servo M1;
Servo M2;
Servo M3;
Servo M4;
Servo M5;
Servo M6;
Servo M7;

Servo servos[8] = { M0, M1, M2, M3, M4, M5, M6, M7 };
int servo_pins[8] = { PINS.MAIN_M0,
                      PINS.MAIN_M1,
                      PINS.MAIN_M2,
                      PINS.MAIN_M3,
                      PINS.MAIN_M4,
                      PINS.MAIN_M5,
                      PINS.MAIN_M6,
                      PINS.MAIN_M7 };

String signal;
bool kill = false;

void setup() {
  Wire.begin();
  Serial2.begin(115200);
  for (int s = 0; s < 8; s++) {
    servos[s].attach(servo_pins[s]); // attach servos to their pins
  }
}

void loop() {
  int motor; // which motor is being set
  int velocity; // speed motor is set to
  if (kill) {
    Wire.beginTransmission(POWER_BOARD); 
    Wire.write(SHUTOFF); // TODO
    Wire.endTransmission();
    delay(50);
  } else {
    if (Serial2.available()) {
      signal = Serial2.readString();
      if (signal[0] == SHUTOFF_REQ) { // TODO
          kill = true;
      } else if (signal.length() == 5 && signal[1] == ':') {
        motor = signal[0] - '0'; // subtract value for char 0
        velocity = signal.substring(2).toInt();
        servos[motor].write(velocity);
        delay(15);
        Serial2.println(motor);
        Serial2.println(velocity);
      }
    }
  }
}
