/***********************************************************************************

Inspried by PHPPLD.
2021 Rue Mohr.
This generic ROM-generator framework was customized for:

web FSM demo - migrate to position.

INPUTS

 A4 A5 A6 A7   4 button keyboard

OUTPUTS

 D4: LED 1
 D5: LED 2
 D6: LED 3
 D7: LED 4

 
FEEDBACK
 D0 -> A0 
 D1 -> A1 
 D2 -> A2 
 D3 -> A3  
 
 
-- generic information --

Address bits      8 bit rom size

       -- no parallel roms available --
     8                  2 k
     9                  4 k
     10                 8 k
     
       -- eeproms available from here --
     11                 16 k  (28C16)
     12                 32 k  (28C32)
     
       -- eprom practical sizes from here --
     13                 64 k  (2764)
     14                 128 k (27128)
     15                 256 k 
     16                 512 k
     17                 1 M  (27010)
     18                 2 M
     19                 4 M
     20                 8 M

       -- flash from here up --



**************************************************************************************/


#include <stdio.h>
#include <stdint.h>
#include "ROMLib.h"


// the number of address lines you need
// 28C64
#define InputBits 13

// the output data size, 8 or 16
// 28C64 is 8 bit.
#define OutputBits 8

// default output value
#define DFOutput  0x00




int main(void) {

  // define input variables
  uint16_t keyboardI;
  uint16_t feedbackProgI;
  uint16_t feedbackStateI;
  
  // define output variables
  uint8_t outputsO;
  uint8_t feedbackProgO;
  uint8_t feedbackStateO;
  
  
  uint32_t out;  // leave it alone!
  setup();       // open output file.
  
  
  // loop thru each address
  for( A=0; A<(1<<InputBits); A++) { // A is a bitfield (a mash-up of the vars we want)
       
     // ---------------------- reset vars -------------------------
     
     
     // -------------------- build input values --------------------
     
      // program  variable is 2 bits, made of bits 0 and 1 of the address
    spliceValueFromField( &feedbackProgI,       A,  2,   0, 1);      
    
      // state    variable is 2 bits, made of bits 0 and 1 of the address
    spliceValueFromField( &feedbackStateI,      A,  2,   2, 3);    
       
      // keyboard variable is 4 bits, made of bits 4, 5, 6, 7 of the address  
    spliceValueFromField( &keyboardI,           A,  4,   4, 5, 6, 7); 
         
      
    // ------------------------ do stuff ---------------------------
    // This is were the output variables are defined. 
     
     // The outputs are based on the system state.
     switch(feedbackStateI) {
       case 0: outputsO = 1; break;
       case 1: outputsO = 2; break;
       case 2: outputsO = 4; break;
       case 3: outputsO = 8; break;          
     }
     
      // I could have broken the keyboard down into 4 variables
     switch(keyboardI) {
       case 1:  feedbackProgO = 0; break;
       case 2:  feedbackProgO = 1; break;
       case 4:  feedbackProgO = 2; break;
       case 8:  feedbackProgO = 3; break;  
       default: feedbackProgO = feedbackProgI; // default is to keep the same program    
     }

     // for this program I'm putting the magic here:
     feedbackStateO = feedbackStateI; // by default, stay where you are.
     switch (feedbackProgO) { // based on the program...
       case 0:
          if (feedbackStateI > 0) feedbackStateO = feedbackStateI - 1; // go towards zero         
       break;
       
       case 1: 
         if (feedbackStateI > 1) feedbackStateO = feedbackStateI - 1; // go towards 1
         if (feedbackStateI < 1) feedbackStateO = feedbackStateI + 1; // go towards 1
       break;
       
       case 2: 
         if (feedbackStateI > 2) feedbackStateO = feedbackStateI - 1; // go towards 2
         if (feedbackStateI < 2) feedbackStateO = feedbackStateI + 1; // go towards 2
       break;
       
       case 3: 
         if (feedbackStateI < 3) feedbackStateO = feedbackStateI + 1; // go towards 3      
       break;
     }

     
                                                                                                                                                                                             
                               
     // ------------------- reconstitute the output ------------------------------
     // assign default values for outputs     
     out = DFOutput;                              
          
       // the feedback program number is 2 bits of feedbackProgO, at bits 0 and 1 of the ROM
     spliceFieldFromValue( &out, feedbackProgO,   2,   0, 1);   
          
       // the feedback state number is 2 bits of feedbackStateO,  at bits 2 and 3 of the ROM
     spliceFieldFromValue( &out, feedbackStateO,  2,   2, 3);   
          
       // the output is 4 bits of outputsO,  at bits 4, 5, 6, 7 of the ROM
     spliceFieldFromValue( &out, outputsO,        4,   4, 5, 6, 7);  
                                             
     // submit entry to file
     write(fd, &out, OutputBits>>3);  // >>3 converts to bytes, leave it!
  }
  
  cleanup(); // close file
  
  return 0;
}









