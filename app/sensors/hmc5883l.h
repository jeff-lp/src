/******************************************************************************

Swiss Space Center

Filename: hmc5883l.h
Author:   Louis Masson

Created:  25/06/2013
Modified: 25/06/2013

Description:
Functions for converting the values read from the HMC5883L

******************************************************************************/



#ifndef __HMC5883L_H
#define __HMC5883L_H

#ifdef __cplusplus
extern "C" {
#endif
  
  
/********************************************************************************************************
*                                              DEFINES
********************************************************************************************************/

// Conversion parameters (depends of Gain configuration, see datasheet)
#define HMC5883L_SENSITIVITY    4.35    //mG/LSb

// HMC5883L Address and Registers definitions
//      The 7-bit address is shifted by 1 bit to the left due to 
//      how Energy Micro's I2C libraries are written.

#define HMC5883L_ADDR           0x1E<<1 // Device address

#define HMC5883L_CFGA           0       // R/W
#define HMC5883L_CFGB           1       // R/W
#define HMC5883L_MODEREG        2       // R/W
#define HMC5883L_MAG_XH         3       // R
#define HMC5883L_MAG_XL         4       // R
#define HMC5883L_MAG_ZH         5       // R
#define HMC5883L_MAG_ZL         6       // R
#define HMC5883L_MAG_YH         7       // R
#define HMC5883L_MAG_YL         8       // R
#define HMC5883L_STATREG        9       // R
#define HMC5883L_IDREGA         10      // R
#define HMC5883L_IDREGB         11      // R
#define HMC5883L_IDREGC         12      // R

// HMC5883L configuration defines
#define MA1     (0<<6)
#define MA0     (0<<5)
#define DO2     (1<<4)     
#define DO1     (0<<3)
#define DO0     (0<<2)
#define MS1     (0<<1)
#define MS0     (0<<0)
#define CFGA    (MA1|MA0|DO2|DO1|DO0|MS1|MS0)
  
#define GN2     (1<<7)
#define GN1     (1<<6)
#define GN0     (1<<5)
#define CFGB    (GN2|GN1|GN0)
  
#define HS      (0<<7)
#define MD1     (0<<1)
#define MD0     (0<<0)
#define MODEREG (HS|MD1|MD0)
  
  
  
/********************************************************************************************************
*                                          STRUCTURES
********************************************************************************************************/

  
  
typedef struct HMCValues HMCV;

struct HMCValues {
  float X;         
  float Y;
  float Z;
};

  
  
/********************************************************************************************************
*                                         FUNCTION PROTOTYPES
********************************************************************************************************/
 
void  HMC5883L_Init();
HMCV  HMC5883L_GetMeasurements();
float HMC5883L_ConvertMag(float value);

  
  
  
#ifdef __cplusplus
}
#endif

#endif /* end of __APP_TASK_ONE_H */