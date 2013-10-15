/******************************************************************************

Swiss Space Center

Filename: app_data_management.h
Author:   Louis Masson

Created:  16/05/2013
Modified: 16/05/2013

Description:
This header contains the necessary declarations for the data management tasks

******************************************************************************/

#ifndef __APP_DATA_MANAGEMENT_H
#define __APP_DATA_MANAGEMENT_H

#ifdef __cplusplus
extern "C" {
#endif
  
  
  
/********************************************************************************************************
*                                         FUNCTION PROTOTYPES
********************************************************************************************************/

  void APP_MemoryManagement(void *Ptr_Arg);
  void APP_SensorDataHandler(void *Ptr_Arg);
  void APP_SensorTimeHandler(void *Ptr_Arg);
  void APP_HKDataHandler(void *Ptr_Arg);
  void APP_PLDataHandler(void *Ptr_Arg);    
  
  
  
  
#ifdef __cplusplus
}
#endif

#endif