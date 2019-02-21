#include <Wire.h>
#include <Servo.h>

#define SHUTOFF (0xEE)
#define POWER_BOARD (8)
#define SHUTOFF_REQ (88)
Servo M0;
Servo M1;
Servo M2;
Servo M3;
Servo M4;
Servo M5;
Servo M6;
Servo M7;

Servo servos[8] = { M0, M1, M2, M3, M4, M5, M6, M7 };
int servo_pins[8] = { 0 };

string signal;
bool kill = false;

void setup() {
  Wire.begin();
  Serial2.begin(115200);
  for (int s = 0; s < 8; s++) {
    servos[s].attach(servo_pins[s]); // attach servos to their pins
  }
}

void loop() {
  if (kill) {
    Wire.beginTransmission(POWER_BOARD); 
    Wire.write(SHUTOFF);
    Wire.endTransmission();
    delay(50);
  } else {
    if (Serial2.availible()) {
      signal = Serial2.readString();
      if (signal[0] == SHUTOFF_REQ) {
          kill = true;
      } else if (signal.length() == 5 && signal[1] == ':') {
        motor = (int)signal[0];
        velocity = (int)signal.substring(2);
        servos[motor].write(velocity);
        delay(15);
        Serial2.writeLn("ye");
      }
    }
  }
}
