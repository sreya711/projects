#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>

#define BLYNK_PRINT Serial

SoftwareSerial serial_info_to_arduino(D2, D3); // for serial communication with arduino
// D2 is used as Rx pin and D3 is used as Tx pin

// Blynk authorization token
char auth[] = "arbTMNFpk18iGtb0e2SmJosJLJ41uagS";

// WiFi credentials
char ssid[] = "Galaxy M31EA2F";
char pass[] = "12344321";

const int stepsPerRevolution = 200; // steps per revolution for the motors
const float distPerstep = 0.02; //(in cm)

int Recall = 0;
bool obj_recal = false;
bool scr_recal = true;


int obj_endpin1 = 16, obj_endpin2 = 5, scr_endpin1 = 2, scr_endpin2 = 13;
int obj_end1 = 0, obj_end2 = 0, scr_end1 = 0, scr_end2 = 0;

float obj_dist;
float scr_dist;

float prev_obj = 9.5;
float prev_scr = 9.5;
int obj_StepsToTake = 0, scr_StepsToTake = 0 ;


// function for object recalibration
float obj_Recallibrate(float obj_dist) {
    // set the distance to 9.5 (cm), which is the closet the object can get to the lens
    obj_dist = 9.5;
    Blynk.virtualWrite(2, 9.5);

    obj_end1 = digitalRead(obj_endpin1);

    // read the state of endpin
    // if the endpin is high, the object has reached the end of the track (i.e., 9.5 cm from the lens)
    // until the endpin is high, keep moving the object towards the lens
    while(obj_end1 != 1) {
    obj_end1 = digitalRead(obj_endpin1);
    serial_info_to_arduino.println("O" + String(-1));
    }

    obj_end1 = 0;
    return obj_dist;
}

// function for image recalibration
float scr_Recallibrate(float scr_dist) {
    // set the distance to 9.5 (cm), which is the closet the screen can get to the lens
    scr_dist = 9.5;
    Blynk.virtualWrite(3, 9.5);

    scr_end1 = digitalRead(scr_endpin1);

    // read the state of endpin
    // if the endpin is high, the screen has reached the end of the track (i.e., 9.5 cm from the lens)
    // until the endpin is high, keep moving the screen towards the lens
    while(scr_end1 != 1) {
        scr_end1 = digitalRead(scr_endpin1);
        serial_info_to_arduino.println("S"+ String(-1));
    }
    scr_end1 = 0;
    return scr_dist;
}

void setup() {

  Blynk.begin(auth, ssid, pass); // To make the connection with Blynk

  Serial.begin(9600);
  serial_info_to_arduino.begin(4800); // for serial communication

  pinMode(16, INPUT);

  // for recallibration at the start of the setup
  obj_dist = obj_Recallibrate(obj_dist);
  scr_dist = scr_Recallibrate(scr_dist);
}

BLYNK_WRITE(V2) {
  obj_dist = param.asFloat();
}

BLYNK_WRITE(V3) {
  scr_dist = param.asFloat();
}

BLYNK_WRITE(V4) {
  Recall = param.asInt();
}


void loop()  {
  Blynk.run(); // to get the info from Blynk

  // if Recall is 1, do recalibration
  if(Recall == 1) {
    Recall = 0;
    prev_obj = 0;
    prev_scr = 0;

    obj_dist = obj_Recallibrate(obj_dist); // recalibrate the object
    scr_dist = scr_Recallibrate(scr_dist); // recalibrate the image

    Blynk.virtualWrite(4, 0);
  }

  // calculate the number of steps to take
  obj_StepsToTake = (obj_dist-prev_obj)/distPerstep;
  scr_StepsToTake = (scr_dist-prev_scr)/distPerstep;

  // serially communicate the required movement information with the arduino
  serial_info_to_arduino.println("O"+ String(obj_StepsToTake));
  serial_info_to_arduino.println("S"+ String(scr_StepsToTake));

  // update the previous distances
  prev_obj = obj_dist;
  prev_scr = scr_dist;

  // calculate the focal length and write it to the dashboard
  float focal_length = prev_obj * prev_scr / (prev_obj + prev_scr);
  Blynk.virtualWrite(5, focal_length);
}
