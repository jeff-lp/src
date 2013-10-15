/******************************************************************************

Swiss Space Center

Filename: seni2c.c
Author:   Louis Masson

Created:  16/05/2013
Modified: 16/05/2013

Description:
This library contains the driver and the functions for communication with the 
sensors through the EFM32GG880's I2C1 bus.

******************************************************************************/




#include <includes.h>





/********************************************************************************************************
*                                         SENI2C_Init()
*
* @brief      Initialisation of the I2C1 bus on location 1 of the EFM32GG880F1024 as master
*
* @param[in]  init       I2C_Init_TypeDef containing initialisation parameters for I2C1
* @exception  none
* @return     none.
*
*
********************************************************************************************************/

void SENI2C_Init(const I2C_Init_TypeDef *init){
  
   int i;
  
  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_I2C1,  true);
  
  // Location 1: Set GPIO pins to 1 to avoid driving the lines low
  // Configure SCL first to ensure that it is high before SDA
  GPIO_PinModeSet(gpioPortB, 12, gpioModeWiredAnd, 1);  
  GPIO_PinModeSet(gpioPortB, 11, gpioModeWiredAnd, 1);  
  
  // The slave device could be left in an unknown state:
  //    -> send 9 clock pulses just in case
  for(i=0; i<9; i++){
    GPIO_PinModeSet(gpioPortB, 12, gpioModeWiredAnd, 0);
    GPIO_PinModeSet(gpioPortB, 12, gpioModeWiredAnd, 1);
  }
  
  // Enable I2C1 pins at location 1
  I2C1->ROUTE = I2C_ROUTE_SDAPEN |
                I2C_ROUTE_SCLPEN |
                (1 << _I2C_ROUTE_LOCATION_SHIFT);
  
  I2C_Init(I2C1, init);
}







/********************************************************************************************************
*                                         SENI2C_Transfer()
*
* @brief      Sensor I2C Transfer function.
*
* @param[in]  seq       Contains information on the message to be sent/received
* @exception  none
* @return     none.
*
*
********************************************************************************************************/

I2C_TransferReturn_TypeDef SENI2C_Transfer(I2C_TransferSeq_TypeDef *seq){
  
  I2C_TransferReturn_TypeDef ret;
  uint32_t timeout = SENI2C_TIMEOUT; 
  
  // Do a polled transfer
  ret = I2C_TransferInit(I2C1, seq);
  
  while(ret == i2cTransferInProgress && timeout--){
    ret = I2C_Transfer(I2C1);
  }
  
  return (ret);
}


/********************************************************************************************************
*                                         SENI2C_ReadRegister8U()
*
* @brief      Read an 8-bit unsigned register
*
* @param[in]  device address       
*             register address
* @exception  none
* @return     value.
*
*
********************************************************************************************************/

uint8_t SENI2C_ReadRegister8U(uint8_t address, uint8_t reg){
 
  uint8_t data;
  
  I2C_TransferReturn_TypeDef  ret;
  I2C_TransferSeq_TypeDef     seq;
  
  // Read the register
  data = reg;
  seq.addr = address;
  
  seq.flags = I2C_FLAG_WRITE;
  seq.buf[0].data = &data;
  seq.buf[0].len = 1;
  ret = SENI2C_Transfer(&seq);
  
  seq.flags = I2C_FLAG_READ;
  ret = SENI2C_Transfer(&seq);
  
  return data;
}





/********************************************************************************************************
*                                         SENI2C_ReadRegister16()
*
* @brief      Read two 8-bit registers from the target device and convert them into a 16-bit value
*
* @param[in]  device address       
*             H register address
              L register address
* @exception  none
* @return     value.
*
*
********************************************************************************************************/

int16_t SENI2C_ReadRegister16(uint8_t address, uint8_t rh, uint8_t rl){

  return (int16_t) ((SENI2C_ReadRegister8U(address, rh) << 8) |
                     SENI2C_ReadRegister8U(address, rl));
}




/********************************************************************************************************
*                                         SENI2C_WriteRegister8()
*
* @brief      Write on an unsigned 8-bit register
*
* @param[in]  device address       
*             register address
              value
* @exception  none
* @return     none.
*
*
********************************************************************************************************/

void SENI2C_WriteRegister8U(uint8_t address, uint8_t reg, uint8_t value){
  
  I2C_TransferReturn_TypeDef  ret;
  I2C_TransferSeq_TypeDef     seq;
  
  // Write the register
  seq.addr = address;
  seq.flags = I2C_FLAG_WRITE_WRITE;
  
  seq.buf[0].data = &reg;
  seq.buf[0].len = 1;
  seq.buf[1].data = &value;
  seq.buf[1].len = 1;
  ret = SENI2C_Transfer(&seq);
}


