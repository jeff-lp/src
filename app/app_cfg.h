/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2010; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                         uC/OS-II example code
*                                  Application configuration header file
*
*                                   Energy Micro EFM32 (EFM32GG990F1024)
*                                              with the
*                               Energy Micro EFM32GG990F1024-STK Starter Kit
*
* @file   app_cfg.h       
* @brief
* @author Energy Micro AS
* @version 3.20.0
******************************************************************************
* @section License
* <b>(C) Copyright 2013 Energy Micro AS, http://www.energymicro.com</b>
*******************************************************************************
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
* 4. The source and compiled code may only be used on Energy Micro "EFM32"
*    microcontrollers and "EFR4" radios.
*
* DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Energy Micro AS has no
* obligation to support this Software. Energy Micro AS is providing the
* Software "AS IS", with no express or implied warranties of any kind,
* including, but not limited to, any implied warranties of merchantability
* or fitness for any particular purpose or warranties against infringement
* of any proprietary rights of a third party.
*
* Energy Micro AS will not be liable for any consequential, incidental, or
* special damages, or any other relief, or for any claim by any third party,
* arising from your use of this Software.
*
*********************************************************************************************************
*/
#ifndef  __APP_CFG_H
#define  __APP_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

  

  
/*
*********************************************************************************************************
*                                         TASKS PRIORITIES
*********************************************************************************************************
*/
#define  OS_TASK_TMR_PRIO       (OS_LOWEST_PRIO-2U)

#define  APP_CFG_TASK_START_PRIO                 30U 
#define  APP_CFG_SERIAL_DISP_PRIO                 5U
#define  APP_CFG_LED_DISP_PRIO                    6U
#define  APP_CFG_COMMAND_PRIO                    18U
#define  APP_CFG_MEM_MAN_PRIO                    20U
#define  APP_CFG_SEN_DATA_PRIO                   22U
#define  APP_CFG_HK_DATA_PRIO                    24U
#define  APP_CFG_PL_DATA_PRIO                    26U
#define  APP_CFG_SEN_TIME_PRIO                   28U 



// PIP
#define  APP_CFG_DATA_PIP                         4U
#define  APP_CFG_SAT_I2C_PIP                     31U
#define  APP_CFG_NAND1_PIP                       32U
#define  APP_CFG_NAND2_PIP                       33U
#define  APP_CFG_NOR_PIP                         34U
#define  APP_CFG_SYSI2C_PIP                      35U


/*
*********************************************************************************************************
*                                         TASK STACK SIZES
*                          Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/
#define  APP_CFG_TASK_START_STK_SIZE           128U
#define  APP_CFG_SERIAL_DISP_STK_SIZE          256U
#define  APP_CFG_LED_DISP_STK_SIZE              16U
#define  APP_CFG_COMMAND_STK_SIZE             2048U
#define  APP_CFG_MEM_MAN_STK_SIZE             1024U
#define  APP_CFG_SEN_DATA_STK_SIZE             128U
#define  APP_CFG_SEN_TIME_STK_SIZE             128U
#define  APP_CFG_HK_DATA_STK_SIZE             1024U
#define  APP_CFG_PL_DATA_STK_SIZE             1024U

#define  APP_CFG_TOTAL_STK_SIZE   APP_CFG_TASK_START_STK_SIZE  + \
                                  APP_CFG_SERIAL_DISP_STK_SIZE + \
                                  APP_CFG_LED_DISP_STK_SIZE + \
                                  APP_CFG_COMMAND_STK_SIZE + \
                                  APP_CFG_MEM_MAN_STK_SIZE + \
                                  APP_CFG_SEN_DATA_STK_SIZE + \
                                  APP_CFG_SEN_TIME_STK_SIZE + \
                                  APP_CFG_HK_DATA_STK_SIZE + \
                                  APP_CFG_PL_DATA_STK_SIZE
                                  

/*
*********************************************************************************************************
*                                         TASK IDs
*********************************************************************************************************
*/

#define TASK_START_ID           0
#define SERIAL_DISP_ID          1
#define LED_DISP_ID             2
#define COMMAND_ID              3
#define MEM_MAN_ID              4
#define SEN_DATA_ID             5
#define HK_DATA_ID              6
#define PL_DATA_ID              7
#define SEN_TIME_ID             8
         
                         
                                    
#ifdef __cplusplus
}
#endif

#endif /* end of __APP_CFG_MODULE_H */
