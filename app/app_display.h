/******************************************************************************

Swiss Space Center

Filename: app_display.h
Author:   Louis Masson

Created:  16/05/2013
Modified: 16/05/2013

Description:
This header contains the necessary declarations for the display tasks

******************************************************************************/



#ifndef __APP_DISPLAY_H
#define __APP_DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif



/********************************************************************************************************
*                                         DISPLAY CONFIGURATION
********************************************************************************************************/
// The following defines allow the user to configure what kind of data will be sent over the UART line.
// Set to 1U to allow print of data, set to 0U to disable it
  
#define PRINT_GYRO_EN           1U  
#define PRINT_MAG1_EN           1U
#define PRINT_MAG2_EN           0U
#define PRINT_OS_STAT_EN        1U

// Defines the refresh rate of the display (refresh rate = S + MS)
#define DISP_FREQ_S             0       // Max 59
#define DISP_FREQ_MS            250     // Max 999
  

/********************************************************************************************************
*                                         FUNCTION PROTOTYPES
********************************************************************************************************/

void APP_SerialDisplay(void *Ptr_Arg);
void APP_LedDisplay(void *Ptr_Arg);
  
void APP_SerialDisplayEn();
void APP_SerialDisplayDis();
  
  
  
#ifdef __cplusplus
}
#endif

#endif /* end of __APP_TASK_ONE_H */