/******************************************************************************

Swiss Space Center

Filename: itg3200.c
Author:   Louis Masson

Created:  24/06/2013
Modified: 24/06/2013

Description:
Functions for converting the values read from the ITG3200

******************************************************************************/



#include <includes.h>



/********************************************************************************************************
*                                         ITG3200_Init()
*
* @brief      Initialization of the ITG3200 configuration registers.
*             Only call when I2C1 initizalized.   (See datasheet for more info)
*
* @param[in]  none
* @exception  none
* @return     node
*
*
********************************************************************************************************/

void  ITG3200_Init(){
  
    // Write on the configuration registers of the ITG3200
    SENI2C_WriteRegister8U(ITG3200_ADDR, ITG3200_SMPLRT_DIV, SMPLRT_DIV);
    SENI2C_WriteRegister8U(ITG3200_ADDR, ITG3200_DLPF_FS,    DLPF_FS);
    SENI2C_WriteRegister8U(ITG3200_ADDR, ITG3200_INT_CFG,    INT_CFG);
    SENI2C_WriteRegister8U(ITG3200_ADDR, ITG3200_PWR_MGM,    PWR_MGM);
}




/********************************************************************************************************
*                                         ITG3200_GetMeasurements()
*
* @brief      Get measurements from the ITG3200 sensor
*
* @param[in]  none
* @exception  none
* @return     ITGV structure with X, Y, Z and temp values.
*
*
********************************************************************************************************/


ITGV  ITG3200_GetMeasurements(){
  
  ITGV res;
  int16_t i;
  
  // Get X rotation
  i = SENI2C_ReadRegister16(ITG3200_ADDR, ITG3200_GYRO_XOUT_H, ITG3200_GYRO_XOUT_L);
  res.X = ITG3200_ConvertGyro((float)i);
  
  // Get Y rotation
  i = SENI2C_ReadRegister16(ITG3200_ADDR, ITG3200_GYRO_YOUT_H, ITG3200_GYRO_YOUT_L);
  res.Y = ITG3200_ConvertGyro((float)i);
  
  // Get Z rotation
  i = SENI2C_ReadRegister16(ITG3200_ADDR, ITG3200_GYRO_ZOUT_H, ITG3200_GYRO_ZOUT_L);
  res.Z = ITG3200_ConvertGyro((float)i);
  
  // Get temperature
  i = SENI2C_ReadRegister16(ITG3200_ADDR, ITG3200_TEMP_OUT_H, ITG3200_TEMP_OUT_L);
  res.temp = ITG3200_ConvertTemp((float)i);
  
  return res;
}





/********************************************************************************************************
*                                         ITG3200_ConvertGyro()
*
* @brief      Converts gyro reading into degrees / s
*
* @param[in]  value       gyro reading
* @exception  none
* @return     result
*
*
********************************************************************************************************/

float ITG3200_ConvertGyro(float value){
  
  return (float) (value - GYRO_OFFSET)/GYRO_SENSITIVITY + GYRO_REFERENCE;

}



/********************************************************************************************************
*                                         ITG3200_ConvertTemp()
*
* @brief      Converts temp reading into degrees Celsius
*
* @param[in]  value       temp reading
* @exception  none
* @return     result
*
*
********************************************************************************************************/

float ITG3200_ConvertTemp(float value){
  
  return (float) ((value - TEMP_OFFSET)/TEMP_SENSITIVITY + TEMP_REFERENCE);
}