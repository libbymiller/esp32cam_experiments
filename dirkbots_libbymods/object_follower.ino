
static uint8_t diffbuff[ 160 * 120];
static camera_fb_t resultFrame {
  .buf = diffbuff,
  .len = sizeof(diffbuff),
  .width = 160,
  .height = 120,
  .format = PIXFORMAT_GRAYSCALE
};

camera_fb_t * diff(camera_fb_t * fb)
{
  // make this static rather than malloc() as we have more of the
  // first than the latter.
  //
  static uint8_t img_buf_bg[ 160 * 120];

  int bitsPerPixel = 8; //8 bit greyscale
  uint16_t delta = bitsPerPixel * 4;

  assert(sizeof(img_buf_bg) == fb->len);
  assert(resultFrame.len == fb->len);
  assert(resultFrame.format == fb->format);

  //loop over all the pixels in the frame
  //could also  do it by x and y

  for (size_t i = 0; i < fb->len; i++) {
    //compare the same pixel in 2 frames

    uint16_t pb = fb->buf[ i ];
    uint16_t pf = img_buf_bg[ i ];

    //if the change is greter than the delta in either direction

    if ( ( pf > pb && pf - pb > delta ) || ( pf < pb && pb - pf > delta )) {
      // different from background
      //resultFrame->setPixel( i, pf ); //fixme
      //Log.println("DIFFERENT");
      //Log.println(pf - pb);
      resultFrame.buf[ i ] = (uint8_t) 255;
    } else {
      // same-ish as background
      //Log.println("SAME");
      //Log.println(pf - pb);
      resultFrame.buf[ i ] = (uint8_t) 0;
    }

    //background is not the previous frame, instead it's a slowly changing average background
    //i.e. we don't compare the previous frame with this one but the bg frame with this one
    //and make the background a little bit more like this pixel, to adjust to slow changes in lighting

    if ( pf > pb ) {
      img_buf_bg[i] = pb + 1;
    }
    if ( pf < pb ) {
      img_buf_bg[i] = pb - 1;
    }
  };
  return &resultFrame;
}

// Return value is the percentage of the pixels that have changed
//
//  0..2   no real target/COG - ignore returnX,Y
//  > 2    returnX,Y usable - higher numers mean more confidence
//
int calculate_cog(camera_fb_t * resultFrame, float * returnX, float * returnY)
{
  int bitsPerPixel = 8; //8 bit greyscale
  int numPixels = (8 * resultFrame->len) / bitsPerPixel; //stolen from Richard, cancles out!

  assert(numPixels == resultFrame->len);
  assert(resultFrame->width ==  160);
  assert(resultFrame->height ==  120);

  //laying groundwork for where the centre of gravity' - the average position of all the diferent bits
  uint32_t sumX = 0;
  uint32_t sumY = 0;
  uint32_t sumN = 0;

  for ( uint32_t i = 0, x = 0, y = 0; i < numPixels; i += 1 ) {
    x ++;
    if ( x >= resultFrame->width ) {
      y++;
      x = 0;
    }
    if (resultFrame->buf[ i ]) {
      sumX += x;
      sumY += y;
      sumN ++;
    };
  };

  // there is not much sensible stuff that we can do - so bail out and
  // do not change returnX/returnY.
  //
  if (sumN == 0)
    return 0;

  ///centre of gravity is "cog"
  uint32_t cogX = 0;
  uint32_t cogY = 0;

  uint32_t percentage = (sumN * 100) / numPixels;
  cogX = sumX / sumN;
  cogY = sumY / sumN;

  //where we're going to look
  //cogx is scaled to width of frame, so we get range 0-1
  *returnX = ((float)cogX  / resultFrame->width);
  *returnY = 1. - ((float)cogY  / resultFrame->width); // use the larger dimension so x & y get the same scaling

  return percentage;
}
