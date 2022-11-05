// here we assume that the camera 'see's the same as the
// pan tilt platform; so if the image is in the center, the
// pan tilt is at 90,90 degree's.
//
void move_straight_away(float cogX, float cogY)
{
  Log.printf("COG is %.2f, %.2f\r\n", cogX, cogY);
  Log.printf("Servo attention is %.2f, %.2f\r\n", m_attentionX, m_attentionY);

  // map from 0.0--1.0 normalized to servo degree space.
  // So cogX/Y is from 0..1 from entire left/bottom and right/top of what the camera can see.
  // and we simply move the servo that way.
  //
  const float SERVER_LEFTMOST = 0;
  const float SERVER_FAR_SIDE = 180;

  m_attentionX = SERVER_LEFTMOST + cogX * (SERVER_FAR_SIDE - SERVER_LEFTMOST);
  m_attentionY = SERVER_LEFTMOST + cogY * (SERVER_FAR_SIDE - SERVER_LEFTMOST);

  // simple mode - move to the right place - we assume that
  // the camera is fixed.
  Log.print("Moving to x ");
  Log.print((int)(m_attentionX));
  Log.print(" and y ");
  Log.println((int)(m_attentionY));

  pan((int)pos_pan, (int)(m_attentionX), wait);
  tilt((int)pos_tilt, (int)(m_attentionY), wait);
}

// point the camera looks at / or point that we try to
// keep the CoG nearest to by moving camera by moving
// the pan and tilt controls.
//
void move_relative(float cogX, float cogY)
{
  static unsigned long lst = millis();

  // 1. how far is the camera off from what we look at ?
  //    and do this in (servo) degrees.
  //
  float delta_X = (CENTER_CAM_X - cogX) * 180;
  float delta_Y = (CENTER_CAM_Y - cogY) * 180;
  float delta   = sqrt(delta_X * delta_X + delta_Y * delta_Y);

  // we do a simple 'P' controller here - the further away we
  // are - the higher the speed. We should use a PD or PID
  // most propably.

  // 2. figure out fast we want to move
  //    the further away we are - the more OK we
  //    we are with a higher speed and do so
  //    quadratic
  //
  const float MAX_SPEED = 30; // 30 degrees per second max speed.
  float speed = delta * delta;

  if (speed > MAX_SPEED)
    speed = MAX_SPEED;

  // 3. how long since we were last called - assume that this is
  //    how often we get called.

  unsigned long deltaT = millis() - lst; // since last change in milli seconds.
  float dT = deltaT / 1000.; // to seconds.

  float pos_pan_new  = pos_pan + delta_X  * speed / dT;
  float pos_tilt_new  = pos_tilt + delta_Y * speed / dT;

  Log.print("Moving to x ");
  Log.print((int)(pos_pan_new));
  Log.print(" and y ");
  Log.print((int)(pos_tilt_new));
  Log.print(" with speed ");
  Log.println(speed);

  // how much (in servo degrees) should at least move; anyting under that - no changes to the servo output.
  //
  const float MIN_MOVE = 1;

  if (abs(pos_pan_new - pos_pan) >= MIN_MOVE) {
    servo_pan.write(pos_pan_new);
    pos_pan = pos_pan_new;
  };

  if (abs(pos_tilt_new - pos_tilt) >= MIN_MOVE) {
    servo_tilt.write(pos_tilt_new);
    pos_tilt = pos_tilt_new;
  };
}
