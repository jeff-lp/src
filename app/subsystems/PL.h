/******************************************************************************

Swiss Space Center

Filename: PL.h
Author:   Louis Masson

Created:  02/07/2013
Modified: 02/07/2013

Description:
This library contains all the commands that are needed to communicate with
the PL subsystem.

******************************************************************************/



#ifndef __PL_H
#define __PL_H

#ifdef __cplusplus
extern "C" {
#endif
  
  
#include  <stdint.h>
  
  
  
  
/********************************************************************************************************
*                                              DEFINES
********************************************************************************************************/
  
// I2C Parameters
#define PL_ADDR                 0x04    // PL subsystem I2C address
  


// Length of a header (type (1B) + length (2B)) 
#define HDR_SZ  0x03

// Length of the CRC of a request/report in bytes
#define CRC_SZ  0x02

// Maximum length for a report in bytes
#define MAX_REPORT_LENGTH 512

// Maximum length for scientific data in bytes
#define MAX_SCDATA_LENGTH 512

// Maximum number of changes that will be recorded between CDMS calls
#define MAX_CHANGES 10

// Size of a change in bytes
#define CHANGE_SIZE 16

// Maximum size of error report buffer
#define MAX_ERRORS_BUFFER_SZ 11


// Message types
#define PL_MT_HK_REQ            0x00    // HK request
#define PL_MT_HK_REP            0x01    // HK report 
#define PL_MT_FC_REQ            0x02    // Function call request
#define PL_MT_FC_REP            0x03    // Function call report
#define PL_MT_REP_NRDY          0xFE    // Report not ready
#define PL_MT_ERR_REP           0xFF    // Error report

// Housekeeping calls
#define PL_HK_ERROR             0x00    // Error codes housekeeping request
#define PL_HK_TEMP              0x01    // Temperature housekeeping request
  
// Scenario command IDs
#define ADD_SCEN                0x00
#define MOD_SCEN                0x01
#define DEL_SCEN                0x02

// Function calls
#define PL_FC_MEAS_EXEC         0x00    // Measurement execution function call
#define PL_FC_MCL_CHANGES       0x01    // MCL changes request
#define PL_FC_SCIENCE_DATA      0x02    // Scientific data request
#define PL_FC_SCENARIO_CMD      0x03    // Update/Create/Delete scenario function call
#define PL_FC_SCENARIO_STATUS   0x04    // Scenario status request
//#define PL_FC_FW_UPDATE         0x05    // Firmware update request
//#define PL_FC_SW_UPDATE         0x06    // Software update request
//#define PL_FC_FW_LOAD           0x07    // Load firmware request
  
// Message sizes
#define PL_HK_ERROR_REQ_SZ              0x0003
#define PL_HK_ERROR_REP_SZ             (0x03+MAX_REPORT_LENGTH)
#define PL_HK_TEMP_REQ_SZ               0x0003
#define PL_HK_TEMP_REP_SZ               0x0005
#define PL_FC_MEAS_EXEC_REQ_SZ          0x0003
#define PL_FC_MEAS_EXEC_REP_SZ          0x0004
#define PL_FC_MCL_CHANGES_REQ_SZ        0x0003
#define PL_FC_MCL_CHANGES_REP_SZ       (0x0004+MAX_CHANGES*CHANGE_SIZE)
#define PL_FC_SCIENCE_DATA_REQ_SZ       0x0003
#define PL_FC_SCIENCE_DATA_REP_SZ      (0x0003+MAX_SCDATA_LENGTH)
#define PL_FC_SCENARIO_CMD_REQ_C_SZ     0x0013
#define PL_FC_SCENARIO_CMD_REP_C_SZ     0x0004
#define PL_FC_SCENARIO_CMD_REQ_U_SZ     0x0012
#define PL_FC_SCENARIO_CMD_REQ_D_SZ     0x0005
#define PL_FC_SCENARIO_CMD_REP_D_SZ     0x0004
#define PL_FC_SCENARIO_CMD_REP_SZ       0x0004
#define PL_FC_SCENARIO_STATUS_REQ_SZ    0x0003
#define PL_FC_SCENARIO_STATUS_REP_SZ    0x0004

// Error definitions
#define CRC_ERR   0x01
#define COM_ERR   0x02
#define REP_NRDY  0x03




/********************************************************************************************************
*                                         FUNCTION PROTOTYPES
********************************************************************************************************/
  
// Housekeeping calls
void PL_HK_GetError(uint8_t* errors, uint16_t* errorFlag);
int16_t PL_HK_GetTemperature(uint16_t* errorFlag);

// Command function calls
int8_t PL_FC_MeasurementExec(uint16_t* errorFlag);

// Command functions
uint8_t PL_FC_ScenarioCreate(uint8_t op, uint8_t id, uint8_t index, uint8_t* description, uint16_t* errorFlag);
uint8_t PL_FC_ScenarioDelete(uint8_t id, uint16_t* errorFlag);


void PL_FC_ScenarioCommand(uint8_t op, uint8_t id, uint8_t index, uint8_t* description);

// Get function calls
int8_t PL_FC_GetScenarioStatus(uint16_t* errorFlag);
void PL_FC_GetMCLChanges(uint8_t* buffer, uint16_t* errorFlag);
uint16_t PL_FC_GetScienceData(uint8_t* data, uint16_t* errorFlag);

// SW/FW update function calls
//void    PL_FC_FWUpdate(uint8_t* buffer);
//void    PL_FC_SWUpdate(uint8_t* buffer);
//void    PL_FC_FWLoad(uint8_t* buffer);




#ifdef __cplusplus
}
#endif

#endif