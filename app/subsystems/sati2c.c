/******************************************************************************

Swiss Space Center

Filename: sati2c.c
Author:   Louis Masson

Created:  02/07/2013
Modified: 02/07/2013

Description:
This library contains the driver and the functions for communication with the 
subsystems through the EFM32GG880's I2C0 bus.

******************************************************************************/




#include <includes.h>





/********************************************************************************************************
*                                         SATI2C_Init()
*
* @brief      Initialisation of the I2C0 bus on location 1 of the EFM32GG880F1024 as master
*
* @param[in]  init       I2C_Init_TypeDef containing initialisation parameters for I2C1
* @exception  none
* @return     none.
*
*
********************************************************************************************************/

void SATI2C_Init(const I2C_Init_TypeDef *init){
  
   int i;
  
  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_I2C0,  true);
  
  // Location 1: Set GPIO pins to 1 to avoid driving the lines low
  // Configure SCL first to ensure that it is high before SDA
  GPIO_PinModeSet(gpioPortD, 7, gpioModeWiredAnd, 1);  
  GPIO_PinModeSet(gpioPortD, 6, gpioModeWiredAnd, 1);  
  
  // The slave device could be left in an unknown state:
  //    -> send 9 clock pulses just in case
  for(i=0; i<9; i++){
    GPIO_PinModeSet(gpioPortD, 7, gpioModeWiredAnd, 0);
    GPIO_PinModeSet(gpioPortD, 7, gpioModeWiredAnd, 1);
  }
  
  // Enable I2C0 pins at location 1
  I2C0->ROUTE = I2C_ROUTE_SDAPEN |
                I2C_ROUTE_SCLPEN |
                (1 << _I2C_ROUTE_LOCATION_SHIFT);
  
  I2C_Init(I2C0, init);
}







/********************************************************************************************************
*                                         SATI2C_Transfer()
*
* @brief      Satellite subsystem I2C Transfer function.
*
* @param[in]  seq       Contains information on the message to be sent/received
* @exception  none
* @return     none.
*
*
********************************************************************************************************/

I2C_TransferReturn_TypeDef SATI2C_Transfer(I2C_TransferSeq_TypeDef *seq){
  
  I2C_TransferReturn_TypeDef ret;
  uint32_t timeout = SATI2C_TIMEOUT; 
  
  // Do a polled transfer
  ret = I2C_TransferInit(I2C0, seq);
  
  while(ret == i2cTransferInProgress && timeout--){
    ret = I2C_Transfer(I2C0);
  }
  
  return (ret);
}





void SATI2C_Communicate(uint8_t* request, uint16_t reqLength, uint8_t* report, uint16_t repLength){

  I2C_TransferReturn_TypeDef  ret;               // I2C Return structure
  I2C_TransferSeq_TypeDef     seq;               // I2C Message structure
  
  // Initialise I2C request parameters and initiate transfer
  seq.addr  = PL_ADDR;
  seq.flags = I2C_FLAG_WRITE;
  seq.buf[0].data = request;
  seq.buf[0].len  = reqLength;
  ret = SATI2C_Transfer(&seq);
  
  // Initialise I2C report parameters and initiate reception
  seq.flags = I2C_FLAG_READ;
  seq.buf[0].data = report;
  seq.buf[0].len  = repLength;
  ret = SATI2C_Transfer(&seq);
  
  // TEMP: Shift all the bits by 1 to the left
  int i;
  for(i = 0; i < repLength; i++)
    report[i] = report[i] << 1;
}