unsigned long ts = millis () ;   // time accounting.

void pan(int from, int too, int wait){
  Serial.println("panning");
  boolean forward = false;
  int position = pos_pan;
  
  if(too<from){
    forward = false;     
  }else{
    forward = true;
  }

  if (millis () - ts >= wait){
    ts += wait ;   // setup timestamp for next time.

    //going in a positive direction
    if (forward && position < too){
      servo_pan.write (++ position) ;  // progress the servo
    }
    if (!forward && position > too){
      servo_pan.write (-- position) ;  // progress the servo
    }
  }
  pos_pan = too;
}

void tilt(int from, int too, int wait){
  Serial.println("tilting");
  boolean forward = false;
  int position = pos_tilt;
  
  if(too<from){
    forward = false;     
  }else{
    forward = true;
  }

  if (millis () - ts >= wait){
    ts += wait ;   // setup timestamp for next time.

    //going in a positive direction
    if (forward && position < too){
      servo_tilt.write (++ position) ;  // progress the servo
    }
    if (!forward && position > too){
      servo_tilt.write (-- position) ;  // progress the servo
    }
  }
  pos_tilt = too;
}
