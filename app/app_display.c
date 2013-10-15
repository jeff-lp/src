/******************************************************************************

Swiss Space Center

Filename: app_display.c
Author:   Louis Masson

Created:  16/05/2013
Modified: 16/05/2013

Description:
This library contains the 2 display tasks of the CDMS. The purposes of these tasks
is limited to the FlatSat version of CubETH, as they will merely allow us to 
determine wether everything is working or not. UART will be used to communicate
with a computer (by sending sensor values, system statistics, etc.), and the LED 
will allow us to perform very simple debug tasks.

******************************************************************************/


#include <includes.h>


/*
*********************************************************************************************************
*                                      LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

uint8_t allowDisplay = false;



/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void printOSStat();
void printGyro();
void printMag1();
void printMag2();










/********************************************************************************************************
*                                         APP_SerialDisplay()
*
* @brief      Serial display task using the UART communication bus
*
* @param[in]  p_arg       Argument passed to 'APP_TaskOne()' by 'OSTaskCreate()'.
* @exception  none
* @return     none.
*/
/* Notes      :(1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                   used.  The compiler should not generate any code for this statement.
*
********************************************************************************************************/

void APP_SerialDisplay(void *Ptr_Arg){
  
  (void)Ptr_Arg; /* Note(1) */
  INT8U err;
  
  while(1){
    
    if( allowDisplay == true ) {
      
      OSMutexPend(dataMutex, 0, &err);    // Wait for resource to be available
    
      // Print Gyro measurements
      #if (PRINT_GYRO_EN > 0)
        printGyro();
      #endif
    
      // Print Mag1 measurements
      #if (PRINT_MAG1_EN > 0)
        printMag1();
      #endif
    
      // Print Mag2 measurements
      #if (PRINT_MAG2_EN > 0)
        printMag2();
      #endif
  
      // Print OS statistics (task usage, CPU usage, etc)
      #if (PRINT_OS_STAT_EN > 0)
        printOSStat();
      #endif
    
      // Print separation carriage return
      printf("\n");
    
      OSMutexPost(dataMutex);             // Make the resource available to other tasks
    }
    
    OSTimeDlyHMSM(0, 0, DISP_FREQ_S, DISP_FREQ_MS);
  }
}



/********************************************************************************************************
*                                         APP_LedDisplay()
*
* @brief      Controls the EFM32GG's 2 LEDs for a visual based output of the system
*
* @param[in]  p_arg       Argument passed to 'APP_TaskOne()' by 'OSTaskCreate()'.
* @exception  none
* @return     none.
*/
/* Notes      :(1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                   used.  The compiler should not generate any code for this statement.
*
********************************************************************************************************/

void APP_LedDisplay(void *Ptr_Arg){
  
  (void)Ptr_Arg; /* Note(1) */
  static INT32U ledPos = 0; /* LED position variable  */
  
  while (1)
  { 
    /* Delay with 100msec                             */
    OSTimeDlyHMSM(0, 0, 0, 200);

    /* Animate LEDs */
    BSP_LedToggle(ledPos);

    /* Modify LED position variable                   */
    if (BSP_NO_OF_LEDS == ++ledPos)
    {
      ledPos = 0; /* 3bit overflow */
    }

    OSTimeDly(1);
  }
  
}



/********************************************************************************************************
*                                      APP_SerialDisplayEn()
*
* @brief      Enables the display of info on the UART console
*
* @param[in]  none
* @exception  none
* @return     none.
*
********************************************************************************************************/


void APP_SerialDisplayEn() {
  allowDisplay = true;
}



/********************************************************************************************************
*                                     APP_SerialDisplayDis()
*
* @brief      Enables the display of info on the UART console
*
* @param[in]  none
* @exception  none
* @return     none.
*
********************************************************************************************************/

void APP_SerialDisplayDis() {
  allowDisplay = false;
}





/*
*********************************************************************************************************
*                                      LOCAL FUNCTIONS
*********************************************************************************************************
*/

void printOSStat(){

  int i;
  uint16_t totalFreeMem = 0;
  uint16_t totalUsedMem = 0;
  OS_STK_DATA data[TASK_USER_NB];
  
  // Create priority table 
  // IMPORTANT!: MUST BE IN THE SAME ORDER AS THE PRIORITY TASK IDs DEFINED IN APP_CFG.H
  const uint8_t  prioTable[TASK_USER_NB] = { APP_CFG_TASK_START_PRIO,
                                APP_CFG_SERIAL_DISP_PRIO,
                                APP_CFG_LED_DISP_PRIO,
                                APP_CFG_COMMAND_PRIO,
                                APP_CFG_MEM_MAN_PRIO,
                                APP_CFG_SEN_DATA_PRIO,
                                APP_CFG_HK_DATA_PRIO,
                                APP_CFG_PL_DATA_PRIO,
                                APP_CFG_SEN_TIME_PRIO };  
  
  // Compute stack statistics and total free and used memory
  for(i = 0; i < TASK_USER_NB; i++){
    OSTaskStkChk(prioTable[i], data+i);
    totalFreeMem += data[i].OSFree;
    totalUsedMem += data[i].OSUsed;
  }
 
    
  //printf("CPU usage:%4d\n", (int) OSCPUUsage);
  
  // Print results | Flexibility needed ?
  printf("------------------------------------------- \n");
  printf("Task name | Prio | Tot. | Free | Used | CPU \n");
  printf("------------------------------------------- \n");
  printf("Serial D. | %4d | %4d | %4d | %4d | -- \n", prioTable[SERIAL_DISP_ID], APP_CFG_SERIAL_DISP_STK_SIZE, data[SERIAL_DISP_ID].OSFree, data[SERIAL_DISP_ID].OSUsed);
  printf("LED Disp. | %4d | %4d | %4d | %4d | -- \n", prioTable[LED_DISP_ID], APP_CFG_LED_DISP_STK_SIZE, data[LED_DISP_ID].OSFree, data[LED_DISP_ID].OSUsed);
  printf("Command   | %4d | %4d | %4d | %4d | -- \n", prioTable[COMMAND_ID], APP_CFG_COMMAND_STK_SIZE, data[COMMAND_ID].OSFree, data[COMMAND_ID].OSUsed);
  printf("Mem. Man. | %4d | %4d | %4d | %4d | -- \n", prioTable[MEM_MAN_ID], APP_CFG_MEM_MAN_STK_SIZE, data[MEM_MAN_ID].OSFree, data[MEM_MAN_ID].OSUsed);
  printf("Sen. Data | %4d | %4d | %4d | %4d | -- \n", prioTable[SEN_DATA_ID], APP_CFG_SEN_DATA_STK_SIZE, data[SEN_DATA_ID].OSFree, data[SEN_DATA_ID].OSUsed);
  printf("HK Data   | %4d | %4d | %4d | %4d | -- \n", prioTable[HK_DATA_ID], APP_CFG_HK_DATA_STK_SIZE, data[HK_DATA_ID].OSFree, data[HK_DATA_ID].OSUsed);
  printf("PL Data   | %4d | %4d | %4d | %4d | -- \n", prioTable[PL_DATA_ID], APP_CFG_PL_DATA_STK_SIZE, data[PL_DATA_ID].OSFree, data[PL_DATA_ID].OSUsed);
  printf("Sen. Time | %4d | %4d | %4d | %4d | -- \n", prioTable[SEN_TIME_ID], APP_CFG_PL_DATA_STK_SIZE, data[SEN_TIME_ID].OSFree, data[SEN_TIME_ID].OSUsed);
  printf("Tsk Start | %4d | %4d | %4d | %4d | -- \n", prioTable[TASK_START_ID], APP_CFG_TASK_START_STK_SIZE, data[TASK_START_ID].OSFree, data[TASK_START_ID].OSUsed);
  printf("------------------------------------------- \n");
  printf("Total     |  --  | %4d | %4d | %4d | %2d \n", APP_CFG_TOTAL_STK_SIZE, totalFreeMem, totalUsedMem, OSCPUUsage);
  printf("------------------------------------------- \n");

  //PC_ElapsedStop();
}


/******************************************************************************/

void printGyro(){
  printf("Gyro measurements (deg/s and 10*celsius): \n");
  printf("X:%3d / Y:%3d / Z:%3d / T: %d \n", 
         (int) (APP_AppDataPtr()->gyro_X/*-APP_AppDataPtr()->gyro_Xdrift*/),
         (int) (APP_AppDataPtr()->gyro_Y/*-APP_AppDataPtr()->gyro_Ydrift*/),
         (int) (APP_AppDataPtr()->gyro_Z/*-APP_AppDataPtr()->gyro_Zdrift*/),
         (int) (APP_AppDataPtr()->gyro_temp*10));
}

/******************************************************************************/

void printMag1(){
  printf("Mag1 measurements (mG): \n");
  printf("X:%4d / Y:%4d / Z:%4d \n",
         (int) (APP_AppDataPtr()->mag1_X),
         (int) (APP_AppDataPtr()->mag1_Y),
         (int) (APP_AppDataPtr()->mag1_Z));
}

/******************************************************************************/

void printMag2(){
  printf("Mag2 measurements: \nUNAVAILABLE \n");
}







