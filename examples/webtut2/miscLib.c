  #include <stdint.h>

// Tuck this one away!. Bit reverser!  Please dont use this in real fft code,
//   YOU KNOW how many bits your working with, and you can use a 
//   specific case generator for it.
uint8_t uniReverse(uint8_t i, uint8_t bits) {

  uint8_t r, m, b;
  r = 0;             // result
  m = 1 << (bits-1); // mask will travel right
  b = 1;             // bit will travel left
  
  while(m) {
    if (i&b) r |=m;
    b <<= 1;
    m >>= 1;  
  }
  
  return r;  

}


// count set bits, unrolled edition.
// if using assember shift into the carry and use addc, 0
uint8_t bitCount(uint16_t n) {  
   uint8_t rv;
   rv = 0;
   if (n & 0x0001) rv++;
   if (n & 0x0002) rv++;
   if (n & 0x0004) rv++;
   if (n & 0x0008) rv++;
   if (n & 0x0010) rv++;
   if (n & 0x0020) rv++;
   if (n & 0x0040) rv++;
   if (n & 0x0080) rv++;   
   if (n & 0x0100) rv++;
   if (n & 0x0200) rv++;
   if (n & 0x0400) rv++;
   if (n & 0x0800) rv++;
   if (n & 0x1000) rv++;
   if (n & 0x2000) rv++;
   if (n & 0x4000) rv++;
   if (n & 0x8000) rv++; 
   
   return rv;
}

// convert a character and bit position to a serial level, 10 bits per character. 
uint8_t SerialChar(char c, uint16_t bit) {
  
  switch (bit) {
    case 0: return 0; // start bit
    case 1: return (c & 0x01)!=0;
    case 2: return (c & 0x02)!=0;
    case 3: return (c & 0x04)!=0;
    case 4: return (c & 0x08)!=0;
    case 5: return (c & 0x10)!=0;
    case 6: return (c & 0x20)!=0;
    case 7: return (c & 0x40)!=0;
    case 8: return (c & 0x80)!=0;
    case 9: return 1; // stop bit          
  }

}

// convert a minute index into an hour/minute time
void iToTime (uint16_t i, uint8_t * hour10 , uint8_t * hour1, uint8_t * minute10, uint8_t * minute1  ) {
  
  for ((*hour10)    = 0; i > 599; i-=600, (*hour10)++);
  for ((*hour1)     = 0; i > 59;  i-=60,  (*hour1)++ );
  for ((*minute10)  = 0; i > 9;  i-=10,  (*minute10)++ );
  *minute1 = i;
  
  (*hour1)++;
  if ((*hour1) == 10){ (*hour1) = 0; (*hour10)++; }

}

