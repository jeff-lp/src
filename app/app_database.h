/******************************************************************************

Swiss Space Center

Filename: app_data.h
Author:   Louis Masson

Created:  15/05/2013
Modified: 15/05/2013

Description:
app_data.h holds all the function declarations for the app_data.c file, as well 
as the type definition of the system's data structure containing information
on latest sensor measurements, housekeeping data, etc.

******************************************************************************/

#ifndef __APP_DATABASE_H
#define __APP_DATABASE_H

#ifdef __cplusplus
extern "C" {
#endif


#define ADCS_BDOT       1
#define ADCS_FULL       2
  
#define GYRO_X          1
#define GYRO_Y          2
#define GYRO_Z          3
  
#define MAG1_X          4
#define MAG1_Y          5
#define MAG1_Z          6
  
#define MAG2_X          7
#define MAG2_Y          8
#define MAG3_Z          9
  
  
/********************************************************************************************************
*                                              TYPES
********************************************************************************************************/


// Application database structure
typedef struct AppData APPDATA;
struct AppData {
  
  float gyro_X;          // Gyroscope measurements
  float gyro_Xdrift;
  float gyro_Y;
  float gyro_Ydrift;
  float gyro_Z;
  float gyro_Zdrift;
  float gyro_temp;
  INT32U gyro_time;      // Gyroscope time of last measurement
  
  float mag1_X;         // MagMet1 measurements
  float mag1_Y;
  float mag1_Z;
  INT32U mag1_time;      // MagMet1 time of last measurement
  
  INT16S mag2_X;         // MagMet2 measurements
  INT16S mag2_Y;
  INT16S mag2_Z;
  INT32U mag2_time;      // MagMet2 time of last measurement

  BOOLEAN changeTbl[9];  // Variable change flag

  //                     // Housekeeping variables
  INT32U hk_time;        // Housekeeping time of last measurement
  
  //                     // Statistics variables
  
  INT8U adcs_mode;       // Current ADCS mode
  INT8U pl_scenario;     // Current PL scenario (0 if no scenario is running)
  
};
  
  


/********************************************************************************************************
*                                         FUNCTION PROTOTYPES
********************************************************************************************************/

APPDATA* APP_AppDataPtr();



#ifdef __cplusplus
}
#endif

#endif /* end of __APP_TASK_ONE_H */
