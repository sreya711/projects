#include <SoftwareSerial.h>
#include <Stepper.h>

SoftwareSerial serial_info_from_nodemcu(3, 2); // Rx and Tx pins

const int stepPerRev = 200;
Stepper obj_stepper(stepPerRev, 13, 12, 11, 10); // pins 10, 11, 12 and 13 are used to control the object motor
Stepper scr_stepper(stepPerRev, 4, 5, 6, 7); // pins 10, 11, 12 and 13 are used to control the image motor

const int motor_speed = 15;

String steps;
char To_move;
int steps1;

void setup() {
  Serial.begin(9600);
  serial_info_from_nodemcu.begin(4800); // for serial communcation with nodemcu

  // setting the motor speeds
  obj_stepper.setSpeed(motor_speed);
  scr_stepper.setSpeed(motor_speed);
}

void loop() {
  if(serial_info_from_nodemcu.available() > 0) {
    String str = serial_info_from_nodemcu.readStringUntil('\n');

    Serial.println(str);
    // str will be of the form "O+#" or "O-#" or "S+#" or "S-#"
    // the first character says whether to move the object or image
    // the next two charaters specify the direction and the number of steps

    To_move = str[0]; // str[0] will either be 'O' or 'S'
    steps = str.substring(1);
    steps1 = steps.toInt();

    if(To_move == 'O') {
      obj_stepper.step(steps1);
    }
    else if (To_move == 'S') {
      scr_stepper.step(steps1);
    }
  }
}
