#include <PID_v2.h>

// Aggressive / fast
//    Kp = 4, Ki = 0.2, Kd = 1;
// Gentle / slow
//  Kp = 1, Ki = 0.05, Kd = 0.25;

// Default/standard baseline
double Kp = 1., Ki = 0.05 / 20, Kd = 0.2 / 20;

PID_v2 pidControllerTiltAxis(Kp, Ki, Kd, PID::Direct);
PID_v2 pidControllerPanAxis(Kp, Ki, Kd, PID::Direct);

const int SERVO_WAIT = 100;

void setup_pid() {
  pidControllerTiltAxis.Start(0 /* current input */, 0 /* current servo position */, 0  /* desired input value */);
  pidControllerTiltAxis.SetSampleTime(SERVO_WAIT);

  pidControllerPanAxis.Start(0 /* current input */, 0 /* current servo position */, 0 /* desired input value */);
  pidControllerPanAxis.SetSampleTime(SERVO_WAIT);

}

float targetX = 0, targetY = 0;
unsigned long last_update = 0;

void move_pid(float cogX, float cogY)
{
  Log.printf("CoG %.1f,%.1f\n", cogX, cogY);
  targetX = cogX - CENTER_CAM_X;
  targetY = cogY - CENTER_CAM_Y;
  Log.printf("Error from center %.1f,%.1f\n", targetX, targetY);

  last_update = millis();
  return;
}

void loop_pid() {
  // do not update overly fast - so the servo's do not
  // jitter that much.
  //
  static unsigned long last_servo_update = 0;
  if (millis() - last_servo_update < SERVO_WAIT)
    return;
  last_servo_update = millis();

  // run the PID controller and get the new steer adjustment. We keep the P/I/D
  // simple by telling our current relative error; i.e how far we
  // are off CENTER_CAM_X. And expect the PID to try to get this
  // error as close to zero as possible.
  // note that we have a speed issue here - if both X and Y
  // move with the same speed - then the head moves a square root
  // of two faster.
  //
  m_attentionX += pidControllerPanAxis.Run(targetX);
  m_attentionY += pidControllerTiltAxis.Run(targetY);

  servo_pan.write(m_attentionX);
  servo_tilt.write(m_attentionY);

#if 0
  static unsigned long last_servo_update_debug = 0;
  if (millis() - last_servo_update_debug < 500)
    return;
  last_servo_update_debug = millis();
#endif

  Log.printf("Servo to %.1f,%.1f -> %.1f,%.1f\n", targetX, targetY, m_attentionX, m_attentionY);
}
