
#ifndef __misclib
#define __misclib
  #include <stdint.h>

  uint8_t uniReverse(uint8_t i, uint8_t bits) ;
  uint8_t bitCount(uint16_t n) ;
  uint8_t SerialChar(char c, uint16_t bit) ;
  void iToTime (uint16_t i, uint8_t * hour10 , uint8_t * hour1, uint8_t * minute10, uint8_t * minute1  ) ;


#endif
