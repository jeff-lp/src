/******************************************************************************

Swiss Space Center

Filename: sati2c.h
Author:   Louis Masson

Created:  02/07/2013
Modified: 02/07/2013

Description:
This library contains all the tasks pertaining to memory management. These tasks
are in charge of gathering sensor data, housekeeping data and scientific data,
in order to store everything in the flash memory banks.

******************************************************************************/



#ifndef __SATI2C_H
#define __SATI2C_H

#ifdef __cplusplus
extern "C" {
#endif
  
  
  
/********************************************************************************************************
*                                              DEFINES
********************************************************************************************************/
  
#define SATI2C_TIMEOUT 300000   // Number of attempts for an I2C communication




/********************************************************************************************************
*                                         FUNCTION PROTOTYPES
********************************************************************************************************/
  


void SATI2C_Init(const I2C_Init_TypeDef *init);
I2C_TransferReturn_TypeDef SATI2C_Transfer(I2C_TransferSeq_TypeDef *seq);

void SATI2C_Communicate(uint8_t* request,
                        uint16_t reqLength,
                        uint8_t* report,
                        uint16_t repLength);

#ifdef __cplusplus
}
#endif

#endif