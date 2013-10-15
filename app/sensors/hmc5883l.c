/******************************************************************************

Swiss Space Center

Filename: itg3200.c
Author:   Louis Masson

Created:  25/06/2013
Modified: 25/06/2013

Description:
Functions for converting the values read from the ITG3200

******************************************************************************/



#include <includes.h>






/********************************************************************************************************
*                                         HMC5883L_Init()
*
* @brief      Initialization of the HMC5883L configuration registers.
*             Only call when I2C1 initizalized.   (See datasheet for more info)
*
* @param[in]  none
* @exception  none
* @return     node
*
*
********************************************************************************************************/

void  HMC5883L_Init(){
  
  // Write on the configuration registers of the HMC5883L
  SENI2C_WriteRegister8U(HMC5883L_ADDR, HMC5883L_CFGA,    CFGA);
  SENI2C_WriteRegister8U(HMC5883L_ADDR, HMC5883L_CFGB,    CFGB);
  SENI2C_WriteRegister8U(HMC5883L_ADDR, HMC5883L_MODEREG, MODEREG);
}



/********************************************************************************************************
*                                         HMC5883L_GetMeasurements()
*
* @brief      Get measurements from the HMC5883L sensor
*
* @param[in]  none
* @exception  none
* @return     HMCV structure with X, Y and Z values
*
*
********************************************************************************************************/

HMCV  HMC5883L_GetMeasurements(){
  
  HMCV res;
  int16_t i;
  
  // Get X magnetic field
  i = SENI2C_ReadRegister16(HMC5883L_ADDR, HMC5883L_MAG_XH, HMC5883L_MAG_XL);
  res.X = HMC5883L_ConvertMag((float)i);
  
  // Get Y magnetic field
  i = SENI2C_ReadRegister16(HMC5883L_ADDR, HMC5883L_MAG_YH, HMC5883L_MAG_YL);
  res.Y = HMC5883L_ConvertMag((float)i);
  
  // Get Z magnetic field
  i = SENI2C_ReadRegister16(HMC5883L_ADDR, HMC5883L_MAG_ZH, HMC5883L_MAG_ZL);
  res.Z = HMC5883L_ConvertMag((float)i);
  
  return res;
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

float HMC5883L_ConvertMag(float value){
  
  return (float)  (value*HMC5883L_SENSITIVITY);
}





