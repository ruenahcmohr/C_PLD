#include <stdio.h>
#include <stdint.h>
#include "ROMLib.h"


// the number of address lines you need
#define InputBits 11

// the output data size, 8 or 16
#define OutputBits 8

// default output value
#define DFOutput  0xFC





int main(void) {

  uint16_t ramp, mode, setpoint;  // values are 16 bits  
  
  uint32_t out, highspeed, drive;  // bit fields are 32 bits

  setup();  // open output file.
  
  // loop thru each address
  for( A=0; A<(1<<InputBits); A++) {
  
     // assign default values
     drive     = 0;
     highspeed = 0;
     out       = DFOutput;
     
     // build input values
     spliceValueFromField( &ramp,     A,  5,   0,1,2,3,4);  
     spliceValueFromField( &mode,     A,  1,   5);
     spliceValueFromField( &setpoint, A,  5,   6,7,8,9,10);

     // do task

     if ((mode==0)&&(ramp>24))                drive     = 1;
     if (((mode==0)&&(ramp>24))||(mode==1))   highspeed = 1;
     if ((mode==1)&&(ramp<setpoint))          drive     = 1;

   
     // reconstitute the output
     spliceFieldFromValue( &out, drive,     1,  0);
     spliceFieldFromValue( &out, highspeed, 1,  1);
     
     // submit to file
     write(fd, &out, OutputBits>>3);  
  }
  
  cleanup(); // close file
  return 0;
}




