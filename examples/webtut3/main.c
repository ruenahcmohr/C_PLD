/***********************************************************************************

Inspried by PHPPLD.
2021 Rue Mohr.
This generic ROM-generator framework was customized for:

web FSM demo - keypad lock
buttons 1, 2, 3 and combination digits, 4 is a reset that will also lock it.

as coded the code is 32132123123123

LEDs:
1 = locked
2 = unlocked
4 = key acknowledge (I'm only running at 2Hz, so this helps)
     

this uses all 4 bits of feedback for the machine state.

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
  uint16_t feedbackStateI;
  
  // define output variables
  uint8_t outputsO;
  uint8_t feedbackStateO;
  
  
  uint32_t out;  // leave it alone!
  setup();       // open output file.
  
  
  // specifically no repeats so that keyboard bouce isn't an issue.
  uint8_t keycode[] = {3, 2, 1, 3, 2, 1, 2, 3, 1, 2, 3, 1, 2, 3};
  
  uint8_t keyStroke; // were going to translate the keyboard a bit in code.
  
  // loop thru each address
  for( A=0; A<(1<<InputBits); A++) { // A is a bitfield (a mash-up of the vars we want)
       
     // ---------------------- reset vars -------------------------
     
     
     // -------------------- build input values --------------------
     
      // program  variable is 2 bits, made of bits 0 and 1 of the address
    spliceValueFromField( &feedbackStateI,       A,  4,   0, 1, 2, 3);        
       
      // keyboard variable is 4 bits, made of bits 4, 5, 6, 7 of the address  
    spliceValueFromField( &keyboardI,           A,  4,   4, 5, 6, 7); 
         
      
    // ------------------------ do stuff ---------------------------
    // This is were the output variables are defined. 
     
     // The outputs are based on the system state.
     switch(feedbackStateI) {   // if you get to state 14, the lock opens, state 15 will be a lockout.
       case 14: outputsO = 0x01; break;
       default: outputsO = 0x02; break;          
     }
     

     // lets translate the keyboard a bit here
     
     switch(keyboardI) {
       case 1:  keyStroke = 1;  outputsO |= 8; break;
       case 2:  keyStroke = 2;  outputsO |= 8; break;
       case 4:  keyStroke = 3;  outputsO |= 8; break;       
       case 8:  keyStroke = 16; outputsO |= 8; break; // reset code       
       default: keyStroke = 17; break; // invalid key (not counted)       
     }

     feedbackStateO = feedbackStateI;  // by default, keep the same state


// then lets check the keystrokes

       if (keyStroke == 16) { // reset
           feedbackStateO = 0;     
       } else if (keyStroke != 17)   // only check valid keys.                                                               
         if (feedbackStateI < 14)                                                   // if were in lockout, or unlocked, reject keys.         
           if (keyStroke != ((feedbackStateI==0)?99:keycode[feedbackStateI-1]))     // if thee was one, ignore last key code.
             if (keyStroke == keycode[feedbackStateI] ) { feedbackStateO = feedbackStateI + 1; } // if the key was correct, advance the state
             else                                       { feedbackStateO = 15;                 } // enter the lockout state otherwise                       
         
                                                                                                                                                                                             
                               
     // ------------------- reconstitute the output ------------------------------
     // assign default values for outputs     
     out = DFOutput;                              
          
       // the feedback program number is 2 bits of feedbackProgO, at bits 0 and 1 of the ROM
     spliceFieldFromValue( &out, feedbackStateO,   4,   0, 1, 2, 3);   
                    
       // the output is 4 bits of outputsO,  at bits 4, 5, 6, 7 of the ROM
     spliceFieldFromValue( &out, outputsO,        4,   4, 5, 6, 7);  
                                             
     // submit entry to file
     write(fd, &out, OutputBits>>3);  // >>3 converts to bytes, leave it!
  }
  
  cleanup(); // close file
  
  return 0;
}









