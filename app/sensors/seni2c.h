/******************************************************************************

Swiss Space Center

Filename: seni2c.h
Author:   Louis Masson

Created:  16/06/2013
Modified: 16/06/2013

Description:
This library contains all the tasks pertaining to memory management. These tasks
are in charge of gathering sensor data, housekeeping data and scientific data,
in order to store everything in the flash memory banks.

******************************************************************************/



#ifndef __SENI2C_H
#define __SENI2C_H

#ifdef __cplusplus
extern "C" {
#endif
  
  
  
/********************************************************************************************************
*                                              DEFINES
********************************************************************************************************/
  
#define SENI2C_TIMEOUT 300000   // Number of attempts for an I2C communication




/********************************************************************************************************
*                                         FUNCTION PROTOTYPES
********************************************************************************************************/
  


void SENI2C_Init(const I2C_Init_TypeDef *init);
I2C_TransferReturn_TypeDef SENI2C_Transfer(I2C_TransferSeq_TypeDef *seq);

uint8_t  SENI2C_ReadRegister8U(uint8_t address, uint8_t reg);
int16_t SENI2C_ReadRegister16(uint8_t address, uint8_t rh, uint8_t rl);
void    SENI2C_WriteRegister8U(uint8_t address, uint8_t reg, uint8_t value);







#ifdef __cplusplus
}
#endif

#endif