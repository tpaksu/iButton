///////////////////////////////////////////////////////////////////////////
////                        Dallas Touch Driver                        ////
////                                                                   ////
////  present = touch_present()   Issues a reset and returns TRUE      ////
////                              if the touch device is there.        ////
////                                                                   ////
////  data = touch_read_BYTE()    Reads one BYTE from a touch device.  ////
////                                                                   ////
////  ok = touch_write_BYTE(data) Writes one BYTE to a touch device    ////
////                              and returns TRUE if all went OK.     ////
////                              A FALSE indicates a collision with   ////
////                              another device.                      ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2003 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
///////////////////////////////////////////////////////////////////////////

#ifndef TOUCH_PIN
   #define TOUCH_PIN  PIN_B3
#endif


BYTE touch_read_byte() {
   BYTE i,data;

   for(i=1;i<=8;++i) {
     output_low(TOUCH_PIN);
     delay_us(14);
     output_float(TOUCH_PIN);
     delay_us(5);
     shift_right(&data,1,input(TOUCH_PIN));
     delay_us(100);
   }
   return(data);
}

BYTE touch_write_byte(BYTE data) {
   BYTE i;

   for(i=1;i<=8;++i) {
     output_low(TOUCH_PIN);
     delay_us(10);
     if(shift_right(&data,1,0)) {
       output_high(TOUCH_PIN);
       delay_us(10);
		 if(!input_state(TOUCH_PIN))
         return(0);
     } else {
       output_low(TOUCH_PIN);
       delay_us(10);
		 if(input_state(TOUCH_PIN))
            return(0);
     }
     delay_us(50);
     output_high(TOUCH_PIN);
     delay_us(50);
   }
   return(TRUE);
}

BYTE touch_present() {
    BOOLEAN present;

    output_low(TOUCH_PIN);
    delay_us(500);
    output_float(TOUCH_PIN);

    delay_us(5);

    if(!input(TOUCH_PIN))
      return(FALSE);
    delay_us(65);
    present=!input(TOUCH_PIN);
    delay_us(240);
    if(present)
      return(TRUE);
    else
      return(FALSE);
}
