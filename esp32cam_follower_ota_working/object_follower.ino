
int img_buf_len = 19200; //hardcoded but it's FRAMESIZE_QQVGA (160x120) grayscale pixels with 8 bits

uint8_t * img_buf_bg = NULL; //new uint8_t[19200];

float m_attentionX = 0.5;
float m_attentionY = 0.5;

void diff(camera_fb_t * fb){
   //Serial.println("diffing");
   //first run
   if(!img_buf_bg){
       //Serial.println("returning!");
       //create a new buffer based on this one to be our background
       img_buf_bg = new uint8_t[img_buf_len];
       memcpy(img_buf_bg, fb->buf, img_buf_len);
   }

   //compare bufs
   int bitsPerPixel = 8; //8 bit greyscale
   int numPixels = (8 * fb->len) / bitsPerPixel; //stolen from Richard, cancles out!

//how big a change is significant
//if it's an 8 bit frame, we make the delta 32
//i.e. a value of 32 in pixel brightness
//our pixel brightness goes from 0-255
//I just tried a bunch of numbers here   

   uint16_t delta = bitsPerPixel * 4; 
   uint32_t sumX = 0;
   uint32_t sumY = 0;
   uint32_t sumN = 0;
   uint16_t x = 0, y = 0;

//loop over all the pixels in the frame
//could also  do it by x and y

   for( uint32_t i = 0; i < numPixels; i += 1 ){
        x ++;
        if( x >= fb->width ){
           y++;
           x = 0;
        }

//compare the same pixel in 2 frames

        uint16_t pb = fb->buf[ i ];
        uint16_t pf = img_buf_bg[ i ];
        //Serial.print("pf - pb");
        //Serial.println(pf - pb);

 //if the change is greter than the delta in either direction
 
        if( ( pf > pb && pf - pb > delta ) || ( pf < pb && pb - pf > delta )) {
            // different from background
            //resultFrame->setPixel( i, pf ); //fixme
            //Serial.println("DIFFERENT");
            //Serial.println(pf - pb);
            fb->buf[ i ] = (uint8_t) 255;

//laying groundwork for where the centre of gravity' - the average position of all the diferent bits

            sumX += x;
            sumY += y;
            sumN ++;
         } else {
            // same-ish as background
            //Serial.println("SAME");
            //Serial.println(pf - pb);
            fb->buf[ i ] = (uint8_t) 0;
         }

//background is not the previous frame, instead it's a slowly changing average background
//i.e. we don't compare the previous frame with this one but the bg frame with this one
//and make the background a little bit more like this pixel, to adjust to slow changes in lighting
           
         if( pf > pb ){
            img_buf_bg[i] = pb +1;
         }
         if( pf < pb ){
            img_buf_bg[i] = pb -1;
         }
   
   }
   

///centre of gravity is "cog"

    uint32_t cogX = 0;
    uint32_t cogY = 0;

    uint32_t percentage = (sumN * 100) / fb->len;
    //Serial.println();
    //Serial.printf("%d percent changed pixels: ", percentage);
    //Serial.println(sumN);
    //Serial.println(resultFrame->len);

//2 is a tuning parameter, will depend on resolution of camera
//nothing's really happening so slow and blinks

    if( percentage < 2 ) // no real target, no COG
    {
        ///Serial.println("No COG\r\n");
 
//telling servos to go slowly
//don't currently use this bit

//where it's looking
//drift slowly towards the centre
//0.5 and 0.7 are neutral positions of the servos, 0-1
//20 is what fraction of the distance to move
        m_attentionX = m_attentionX + (((0.5 - m_attentionX))/20);
        m_attentionY = m_attentionY + (((0.7 - m_attentionY))/20);
    }
// if some pixels have changed (stop /0 error)
    else if( sumN > 0 )
    {
        Serial.printf("%d percent changed pixels: ", percentage);
//servos how fast to travel
        int wait = 15;

//where the centre of gravity is for all the change
//[if 2 people wave at it they go in between]
        cogX = sumX / sumN;
        cogY = sumY / sumN;

//where we're going to look
//cogx is scaled to width of frame, so we get range 0-1
        m_attentionX = ((float)cogX  / fb->width);
        m_attentionY = 1 - ((float)cogY  / fb->width); // use the larger dimension so x & y get the same scaling

//not so bored
        //blinker->setBoredom( 0 );
//more pixels change less bored they are
        float boredom = (2*percentage)/100.0;
        if(boredom > 1) boredom = 1;
        //blinker->setBoredom( boredom );

        Serial.printf("COG is %d, %d\r\n", (int) cogX, (int) cogY);
        Serial.printf("m_attentionX, m_attentionY is %f, %f\r\n", (float) m_attentionX, (float) m_attentionY);
        Serial.print("Moving to x ");
        Serial.print((int)(m_attentionX * 180));
        Serial.print(" and y ");
        Serial.println((int)(m_attentionY * 180));
        pan((int)pos_pan, (int)(m_attentionX *180), wait);
        tilt((int)pos_tilt, (int)(m_attentionY *180), wait);
      
    }
}
