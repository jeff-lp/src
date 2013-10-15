#ifndef  __UTILITIES_H
#define  __UTILITIES_H

#ifdef __cplusplus
extern "C" {
#endif
  
  
  
  
#include  <stdint.h>

  
  



// Define TRUE and FALSE states
#define TRUE    1
#define FALSE   0


// Return value if crc check was successfull
#define CRC_OK 0




  


// CRC-16 calculation and check
/*! Calculates the a checksum for the given data using CRC-16.

    To check a calculated CRC-16 checksum, add it at the end of the data before call this function.
    When the return value is 0x0000, it means the data with and the checksum are correct.
 
    \param pData Pointer to the data for creating the checksum 
    \param length Size of the data in bytes 
    \return Calculated 16bit checksum
*/  
  
uint16_t UTI_crc16(uint8_t* pData, unsigned int length);



#ifdef __cplusplus
}
#endif

#endif /* end of __APP_CFG_MODULE_H */