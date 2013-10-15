/******************************************************************************

Swiss Space Center

Filename: app_data.h
Author:   Louis Masson

Created:  15/05/2013
Modified: 15/05/2013

Description:
app_data.c nests the declaration of the AppData structure, as well as the
function allowing the system to access the data.

******************************************************************************/




#include <includes.h>




static APPDATA data = {

  .gyro_X = 0,
  .gyro_Xdrift = 0,
  .gyro_Y = 0,
  .gyro_Ydrift = 0,
  .gyro_Z = 0,
  .gyro_Zdrift = 0,
  .gyro_temp = 0,
  .gyro_time = 0,  
  
  .mag1_X = 0, 
  .mag1_Y = 0,
  .mag1_Z = 0,
  .mag1_time = 0,   
  
  .mag2_X = 0, 
  .mag2_Y = 0,
  .mag2_Z = 0,
  .mag2_time = 0,    

  .changeTbl = {0,0,0,0,0,0,0,0,0},

  .hk_time = 0,  
  
  .adcs_mode = ADCS_FULL,
  .pl_scenario = 0,
};


/*********************************************************************************************************
*                                         APP_AppDataPtr()
* @brief      AppData structure access function
*
* @param[in]  none
* @exception  none
* @return     pointer on AppData structure
*/
/* Notes      :(1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                   used.  The compiler should not generate any code for this statement.
*
*
*********************************************************************************************************/


APPDATA* APP_AppDataPtr(){
  return &data;
}