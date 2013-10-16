/******************************************************************************

Swiss Space Center

Filename: app_data_management.c
Author:   Louis Masson

Created:  16/05/2013
Modified: 16/05/2013

Description:
This library contains all the tasks pertaining to memory management. These tasks
are in charge of gathering sensor data, housekeeping data and scientific data,
in order to store everything in the flash memory banks.

******************************************************************************/

#include <includes.h>



/********************************************************************************************************
*                                         APP_MemoryManagement()
*
* @brief      Manages the flash memory banks
*
* @param[in]  p_arg       Argument passed to 'APP_TaskOne()' by 'OSTaskCreate()'.
* @exception  none
* @return     none.
*/
/* Notes      :(1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                   used.  The compiler should not generate any code for this statement.
*
********************************************************************************************************/

void APP_MemoryManagement(void *Ptr_Arg){
  
  (void)Ptr_Arg; /* Note(1) */
  
  while(1){
    OSTimeDlyHMSM(0, 0, 1, 0);
  }
  
}





/********************************************************************************************************
*                                         APP_SensorDataHandler()
*
* @brief      Queries for sensor measurements and stores them in app database
              Also initialises the sensors before entering the infinite loop.
*
* @param[in]  p_arg       Argument passed to 'APP_TaskOne()' by 'OSTaskCreate()'.
* @exception  none
* @return     none.
*/
/* Notes      :(1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                   used.  The compiler should not generate any code for this statement.
*
********************************************************************************************************/

void APP_SensorDataHandler(void *Ptr_Arg){
  
  (void)Ptr_Arg; /* Note(1) */
  INT8U err;
  
  
  
  // Initialise ITG3200 and make a first measurement to determine drift values
  ITG3200_Init();
  ITGV gyro = ITG3200_GetMeasurements();
  APP_AppDataPtr()->gyro_Xdrift = gyro.X;
  APP_AppDataPtr()->gyro_Ydrift = gyro.Y;
  APP_AppDataPtr()->gyro_Zdrift = gyro.Z;
  
  // Initialise HMC5883L
  HMC5883L_Init();
  
  
  
  while(1){
    
    //char c = 0;
        
    // Wait for resources to be available
    OSMutexPend(dataMutex, 0, &err);      

    
    // ITG3200
    ITGV gyro = ITG3200_GetMeasurements();
    APP_AppDataPtr()->gyro_X = gyro.X;
    APP_AppDataPtr()->gyro_Y = gyro.Y;
    APP_AppDataPtr()->gyro_Z = gyro.Z;
    APP_AppDataPtr()->gyro_temp = gyro.temp;
    APP_AppDataPtr()->gyro_time = gyro.time;
    
    
    // HMC5883L
    HMCV mag1 = HMC5883L_GetMeasurements();
    APP_AppDataPtr()->mag1_X = mag1.X;
    APP_AppDataPtr()->mag1_Y = mag1.Y;
    APP_AppDataPtr()->mag1_Z = mag1.Z;
    
    // If successful
    //c=1;
  
    OSMutexPost(dataMutex);              // Make the resources available to other tasks             
    
    //if(c)
    //  OSMboxPost(memMngmtMsgObj, &c);     // If operation is succesful, signal memory management
    
    OSTimeDlyHMSM(0, 0, 0, 100);
  }
  
}


/********************************************************************************************************
*                                         APP_SensorTimeHandler()
*
* @brief      Registers time of sensor measurements and stores them in app database
*
* @param[in]  p_arg       Argument passed to 'APP_TaskOne()' by 'OSTaskCreate()'.
* @exception  none
* @return     none.
*/
/* Notes      :(1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                   used.  The compiler should not generate any code for this statement.
*
********************************************************************************************************/

void APP_SensorTimeHandler(void *Ptr_Arg){
  
  (void)Ptr_Arg; /* Note(1) */
  
  while(1){
    OSTimeDlyHMSM(0, 0, 1, 0);
  }
  
}




/********************************************************************************************************
*                                         APP_HKDataHandler()
*
* @brief      Asks for HK data on the subsystem I2C bus and stores it in the app database
*
* @param[in]  p_arg       Argument passed to 'APP_TaskOne()' by 'OSTaskCreate()'.
* @exception  none
* @return     none.
*/
/* Notes      :(1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                   used.  The compiler should not generate any code for this statement.
*
********************************************************************************************************/

void APP_HKDataHandler(void *Ptr_Arg){
  
  (void)Ptr_Arg; /* Note(1) */
  INT8U err;
  
  while(1){
    
    char c = 0;
    
    OSMutexPend(sysI2CMutex, 0, &err);    // Wait for resources to be available
    OSMutexPend(dataMutex, 0, &err);
  
    // Query all other subsystems in order to aquire housekeeping data
    // Query PL to check if current scenario is finished
    
    // If successful
      c=1;
  
    OSMutexPost(dataMutex);
    OSMutexPost(sysI2CMutex);             // Make the resources available to other tasks
    
    if(c)
      OSMboxPost(memMngmtMsgObj, &c);     // If operation is succesful, signal memory management
   
    OSTimeDlyHMSM(0, 0, 1, 0);
  }
  
}



/********************************************************************************************************
*                                         APP_PLDataHandler()
*
* @brief      Asks for PL data on the subsystem I2C bus and stores it in the app database
*
* @param[in]  p_arg       Argument passed to 'APP_TaskOne()' by 'OSTaskCreate()'.
* @exception  none
* @return     none.
*/
/* Notes      :(1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                   used.  The compiler should not generate any code for this statement.
*
********************************************************************************************************/

void APP_PLDataHandler(void *Ptr_Arg){
  
  (void)Ptr_Arg; /* Note(1) */
  INT8U err;
  
  while(1){
    
    char c = 0;
    
    OSMutexPend(sysI2CMutex, 0, &err);    // Wait for resources to be available
    OSMutexPend(dataMutex, 0, &err);
  
    // Query all other subsystems in order to aquire housekeeping data
    // Query PL to check if current scenario is finished
    
    // If successful
      c=1;
  
    OSMutexPost(dataMutex);
    OSMutexPost(sysI2CMutex);             // Make the resources available to other tasks
    
    if(c)
      OSMboxPost(memMngmtMsgObj, &c);     // If operation is succesful, signal memory management
   
    OSTimeDlyHMSM(0, 0, 1, 0);
  }
  
}

