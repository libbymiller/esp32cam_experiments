// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33
// you have to use a timer higher than 2, see https://www.esp32.com/viewtopic.php?t=11379
int servoPin_pan = 14;
int servoPin_tilt = 2;

unsigned long ts = millis () ;   // time accounting.

void setup_servo() {
  pinMode(servoPin_pan, OUTPUT);
  pinMode(servoPin_tilt, OUTPUT);

  //https://stackoverflow.com/questions/64402915/esp32-cam-with-servo-control-wont-work-arduino-ide
  //and
  //https://www.esp32.com/viewtopic.php?t=11379
  servo_pan.attach(servoPin_pan, 4);
  servo_tilt.attach(servoPin_tilt, 5);

  servo_pan.write(pos_pan);
  servo_tilt.write(pos_tilt);
}

void pan(int from, int too, int wait) {
  //Log.println("panning");
  boolean forward = false;
  int position = pos_pan;

  if (too < from) {
    forward = false;
  } else {
    forward = true;
  }

  if (millis () - ts >= wait) {
    ts += wait ;   // setup timestamp for next time.

    //going in a positive direction
    if (forward && position < too) {
      servo_pan.write (++ position) ;  // progress the servo
    }
    if (!forward && position > too) {
      servo_pan.write (-- position) ;  // progress the servo
    }
  }
  pos_pan = too;
}

void tilt(int from, int too, int wait) {
  //Log.println("tilting");
  boolean forward = false;
  int position = pos_tilt;

  if (too < from) {
    forward = false;
  } else {
    forward = true;
  }

  if (millis () - ts >= wait) {
    ts += wait ;   // setup timestamp for next time.

    //going in a positive direction
    if (forward && position < too) {
      servo_tilt.write (++ position) ;  // progress the servo
    }
    if (!forward && position > too) {
      servo_tilt.write (-- position) ;  // progress the servo
    }
  }
  pos_tilt = too;
}
