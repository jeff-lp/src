/******************************************************************************

Swiss Space Center

Filename: itg3200.h
Author:   Louis Masson

Created:  24/06/2013
Modified: 24/06/2013

Description:
Functions for converting the values read from the ITG3200

******************************************************************************/



#ifndef __ITG3200_H
#define __ITG3200_H


#ifdef __cplusplus
extern "C" {
#endif
  
  
/********************************************************************************************************
*                                              DEFINES
********************************************************************************************************/

// Gyroscope parameters
#define GYRO_OFFSET       0
#define GYRO_REFERENCE    0
#define GYRO_SENSITIVITY  14.375 //LSb/(rad/s)
  
// Temperature parameters
#define TEMP_OFFSET       (-13200)
#define TEMP_REFERENCE    35
#define TEMP_SENSITIVITY  280
  

// ITG3200 Address and Registers definitions
//      The 7-bit address is shifted by 1 bit to the left due to 
//      how Energy Micro's I2C libraries are written.

#define ITG3200_ADDR            0x68 << 1 // Device address

#define ITG3200_WHO_AM_I        0       // R/W
#define ITG3200_SMPLRT_DIV      21      // R/W
#define ITG3200_DLPF_FS         22      // R/W
#define ITG3200_INT_CFG         23      // R/W
#define ITG3200_INT_STATUS      26      // R
#define ITG3200_TEMP_OUT_H      27      // R
#define ITG3200_TEMP_OUT_L      28      // R
#define ITG3200_GYRO_XOUT_H     29      // R
#define ITG3200_GYRO_XOUT_L     30      // R
#define ITG3200_GYRO_YOUT_H     31      // R
#define ITG3200_GYRO_YOUT_L     32      // R
#define ITG3200_GYRO_ZOUT_H     33      // R
#define ITG3200_GYRO_ZOUT_L     34      // R
#define ITG3200_PWR_MGM         62      // R/W


// ITG3200 configuration defines
#define SMPLRT_DIV      9
  
#define FS_SEL          (3<<3)
#define DLPF_CFG        (0<<0)
#define DLPF_FS         (FS_SEL|DLPF_CFG)
  
#define ACTL            (0<<7)
#define OPEN            (0<<6)
#define LATCH_INT_EN    (1<<5)
#define INT_ANYRD_2CLR  (1<<4)
#define ITG_RDY_EN      (0<<2)
#define RAW_RDY_EN      (1<<0)
#define INT_CFG         (ACTL|OPEN|LATCH_INT_EN|INT_ANYRD_2CLR|ITG_RDY_EN|RAW_RDY_EN)
  
#define H_RESET         (0<<7)
#define SLEEP           (0<<6)
#define STBY_XG         (1<<5)
#define STBY_YG         (0<<4)
#define STBY_ZG         (0<<3)
#define CLK_SEL         (1<<0)
#define PWR_MGM         (H_RESET|SLEEP|STBY_XG|STBY_YG|STBY_ZG|CLK_SEL)

  
  
/********************************************************************************************************
*                                          STRUCTURES
********************************************************************************************************/

  
  
typedef struct ITGValues ITGV;

struct ITGValues {
  float X;         
  float Y;
  float Z;
  float temp;
};

  
  
/********************************************************************************************************
*                                         FUNCTION PROTOTYPES
********************************************************************************************************/
 
void  ITG3200_Init();
ITGV  ITG3200_GetMeasurements();
float ITG3200_ConvertGyro(float value);
float ITG3200_ConvertTemp(float value);
  
  
  
  
  #ifdef __cplusplus
}
#endif

#endif /* end of __APP_TASK_ONE_H */