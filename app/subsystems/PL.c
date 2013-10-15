/******************************************************************************

Swiss Space Center

Filename: PL.c
Author:   Louis Masson

Created:  02/07/2013
Modified: 02/07/2013

Description:
This library contains all the commands that are needed to communicate with
the PL subsystem.

******************************************************************************/



#include <includes.h>


/*
*********************************************************************************************************
*                                      MACROS
*********************************************************************************************************
*/   

// This define is here solely for the purpose of the development of the CDMS  
// This MACRO displays the content of the request package sent to the PL
#define REQ_DEBUG_MACRO         int x;\
                                printf("\n\n");\
                                for( x = 0; x < reqLength; x++ )\
                                  printf("%d|", request[x]);\
                                printf("\nCRC check: %d\n", UTI_crc16(request, reqLength));\
                                printf("-------------");
                                
// This define is here solely for the purpose of the development of the CDMS  
// This MACRO displays the content of the report package sent by the PL                             
#define REP_DEBUG_MACRO         int y;\
                                printf("\n");\
                                for( y = 0; y < repLength; y++ )\
                                  printf("%d|", report[y]);\
                                printf("\nCRC check: %d\n", UTI_crc16(report, repLength));\
                                printf("\n\n");                                
                                
 //(report[1]<<8)+report[2]+HDR_SZ                               


                                
/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/                            
                                
                                
void PL_HK_ParseError(uint8_t* report, uint16_t repLength, uint8_t* errors, uint16_t* errorFlag);                             
int16_t PL_HK_ParseTemp(uint8_t* report, uint16_t repLength, uint16_t* errorFlag);
int8_t PL_FC_ParseExec(uint8_t* report, uint16_t repLength, uint16_t* errorFlag);
int8_t PL_FC_ParseScenarioStatus(uint8_t* report, uint16_t repLength, uint16_t* errorFlag);
uint16_t PL_FC_ParseScienceData(uint8_t* report, uint16_t repLength, uint8_t* data, uint16_t* errorFlag);
void PL_FC_ParseMCLChanges(uint8_t* report, uint16_t repLength, uint8_t* data, uint16_t* errorFlag);
uint8_t PL_FC_ParseScenarioCreate(uint8_t* report, uint16_t repLength, uint16_t* errorFlag);
uint8_t PL_FC_ParseScenarioDelete(uint8_t* report, uint16_t repLength, uint16_t* errorFlag);
  






/********************************************************************************************************
*                                     PL_HK_GetError()
*
* @brief      Request error codes from the PL, and interpret report
*
* @param[in]  none       
* @exception  none
* @return     I2C transmission success (or error)       16-bit signed integer
*
********************************************************************************************************/

void PL_HK_GetError(uint8_t* errors, uint16_t* errorFlag){
  
  uint8_t request[HDR_SZ + PL_HK_ERROR_REQ_SZ];   // Request buffer
  uint8_t report [HDR_SZ + PL_HK_ERROR_REP_SZ];   // Report buffer
  int16_t crc;                                    // CRC variable
 
  // Calculate total length of request and report
  uint16_t reqLength = HDR_SZ + PL_HK_ERROR_REQ_SZ; 
  uint16_t repLength = HDR_SZ + PL_HK_ERROR_REP_SZ;
  
  // Create message 
  request[0] = PL_MT_HK_REQ;                     // Message type
  request[1] = PL_HK_ERROR_REQ_SZ >> 8;          // Message size (HIGH)
  request[2] = PL_HK_ERROR_REQ_SZ & 0xFF;        // Message size (LOW)
  request[3] = PL_HK_ERROR;                      // Request ID
  crc = UTI_crc16(request, reqLength - CRC_SZ);  // Calculate CRC-16
  request[4] = crc >> 8;                         // CRC (HIGH)
  request[5] = crc & 0xFF;                       // CRC (LOW)
  
  // Display the request buffer on the UART console (debug purposes)
  REQ_DEBUG_MACRO;
  
  // Start communication
  SATI2C_Communicate(request, reqLength, report, repLength);
  
  // Display the report buffer on the UART console (debug purposes)
  REP_DEBUG_MACRO;
 
  // Parse and write result in the 'errors' buffer
  PL_HK_ParseError(report, repLength, errors, errorFlag);
}



/********************************************************************************************************
*                                     PL_HK_GetTemperature()
*
* @brief      Request temperature from the PL, and interpret report
*
* @param[in]  none       
* @exception  none
* @return     temperature       16-bit signed integer
*
********************************************************************************************************/

int16_t PL_HK_GetTemperature(uint16_t* errorFlag){
  
  uint8_t request[HDR_SZ + PL_HK_TEMP_REQ_SZ];   // Request buffer
  uint8_t report [HDR_SZ + PL_HK_TEMP_REP_SZ];   // Report buffer
  int16_t crc;                                   // CRC variable
 
  // Calculate total length of request and report
  uint16_t reqLength = HDR_SZ + PL_HK_TEMP_REQ_SZ; 
  uint16_t repLength = HDR_SZ + PL_HK_TEMP_REP_SZ;
  
  // Create message 
  request[0] = PL_MT_HK_REQ;                     // Message type
  request[1] = PL_HK_TEMP_REQ_SZ >> 8;           // Message size (HIGH)
  request[2] = PL_HK_TEMP_REQ_SZ & 0xFF;         // Message size (LOW)
  request[3] = PL_HK_TEMP;                       // Request ID
  crc = UTI_crc16(request, reqLength - CRC_SZ);  // Calculate CRC-16
  request[4] = crc >> 8;                         // CRC (HIGH)
  request[5] = crc & 0xFF;                       // CRC (LOW)
  
  // Display the request buffer on the UART console (debug purposes)
  REQ_DEBUG_MACRO;
  
  // Start communication
  SATI2C_Communicate(request, reqLength, report, repLength);
  
  // Display the report buffer on the UART console (debug purposes)
  REP_DEBUG_MACRO;

  // Parse and return value
  return PL_HK_ParseTemp(report, repLength, errorFlag);
}




/********************************************************************************************************
*                                   PL_FC_MeasurementExec()
*
* @brief      Allow Payload to execute the next scenario in its MCL
*
* @param[in]  none       
* @exception  none
* @return     scenario ID (or error code if a communication error occured)
*
********************************************************************************************************/

int8_t PL_FC_MeasurementExec(uint16_t* errorFlag){
  
  uint8_t request[HDR_SZ + PL_FC_MEAS_EXEC_REQ_SZ];   // Request buffer
  uint8_t report [HDR_SZ + PL_FC_MEAS_EXEC_REP_SZ];   // Report buffer
  int16_t crc;                                        // CRC variable
 
  // Calculate total length of request and report
  uint16_t reqLength = HDR_SZ + PL_FC_MEAS_EXEC_REQ_SZ; 
  uint16_t repLength = HDR_SZ + PL_FC_MEAS_EXEC_REP_SZ;
  
  // Create message 
  request[0] = PL_MT_FC_REQ;                     // Message type
  request[1] = PL_FC_MEAS_EXEC_REQ_SZ >> 8;      // Message size (HIGH)
  request[2] = PL_FC_MEAS_EXEC_REQ_SZ & 0xFF;    // Message size (LOW)
  request[3] = PL_FC_MEAS_EXEC;                  // Request ID
  crc = UTI_crc16(request, reqLength - CRC_SZ);  // Calculate CRC-16
  request[4] = crc >> 8;                         // CRC (HIGH)
  request[5] = crc & 0xFF;                       // CRC (LOW)
  
  // Display the request buffer on the UART console (debug purposes)
  REQ_DEBUG_MACRO;
  
  // Start communication
  SATI2C_Communicate(request, reqLength, report, repLength);
  
  // Display the report buffer on the UART console (debug purposes)
  REP_DEBUG_MACRO;

  // Parse and return value
  return PL_FC_ParseExec(report, repLength, errorFlag);
}





/********************************************************************************************************
*                                 PL_FC_GetScenarioStatus()
*
* @brief      Get the ID of the scenario currently running on the PL
*
* @param[in]  none       
* @exception  none
* @return     scenario status (or error code if a communication error occured)
*
********************************************************************************************************/

int8_t PL_FC_GetScenarioStatus(uint16_t* errorFlag){
  
  uint8_t request[HDR_SZ + PL_FC_SCENARIO_STATUS_REQ_SZ];   // Request buffer
  uint8_t report [HDR_SZ + PL_FC_SCENARIO_STATUS_REP_SZ];   // Report buffer
  int16_t crc;                                              // CRC variable
 
  // Calculate total length of request and report
  uint16_t reqLength = HDR_SZ + PL_FC_SCENARIO_STATUS_REQ_SZ; 
  uint16_t repLength = HDR_SZ + PL_FC_SCENARIO_STATUS_REP_SZ;
  
  // Create message 
  request[0] = PL_MT_FC_REQ;                            // Message type
  request[1] = PL_FC_SCENARIO_STATUS_REQ_SZ >> 8;       // Message size (HIGH)
  request[2] = PL_FC_SCENARIO_STATUS_REQ_SZ & 0xFF;     // Message size (LOW)
  request[3] = PL_FC_SCENARIO_STATUS;                   // Request ID
  crc = UTI_crc16(request, reqLength - CRC_SZ);         // Calculate CRC-16
  request[4] = crc >> 8;                                // CRC (HIGH)
  request[5] = crc & 0xFF;                              // CRC (LOW)
  
  // Display the request buffer on the UART console (debug purposes)
  REQ_DEBUG_MACRO;
  
  // Start communication
  SATI2C_Communicate(request, reqLength, report, repLength);
  
  // Display the report buffer on the UART console (debug purposes)
  REP_DEBUG_MACRO;

  // Parse and return value
  return PL_FC_ParseScenarioStatus(report, repLength, errorFlag);
}




/********************************************************************************************************
*                                 PL_FC_GetScienceData()
*
* @brief      Retrieve scientific data from the PL
*
* @param[in]  data*             A pointer on a data buffer       
* @exception  none
* @return     data size
*
********************************************************************************************************/

uint16_t PL_FC_GetScienceData(uint8_t* data, uint16_t* errorFlag){
  
  uint8_t request[HDR_SZ + PL_FC_SCIENCE_DATA_REQ_SZ];   // Request buffer
  uint8_t report [HDR_SZ + PL_FC_SCIENCE_DATA_REP_SZ];   // Report buffer
  int16_t crc;                                           // CRC variable
 
  // Calculate total length of request and report
  uint16_t reqLength = HDR_SZ + PL_FC_SCIENCE_DATA_REQ_SZ; 
  uint16_t repLength = HDR_SZ + PL_FC_SCIENCE_DATA_REP_SZ;
  
  // Create message 
  request[0] = PL_MT_FC_REQ;                           // Message type
  request[1] = PL_FC_SCIENCE_DATA_REQ_SZ >> 8;         // Message size (HIGH)
  request[2] = PL_FC_SCIENCE_DATA_REQ_SZ & 0xFF;       // Message size (LOW)
  request[3] = PL_FC_SCIENCE_DATA;                     // Request ID
  crc = UTI_crc16(request, reqLength - CRC_SZ);        // Calculate CRC-16
  request[4] = crc >> 8;                               // CRC (HIGH)
  request[5] = crc & 0xFF;                             // CRC (LOW)
  
  // Display the request buffer on the UART console (debug purposes)
  REQ_DEBUG_MACRO;
  
  // Start communication
  SATI2C_Communicate(request, reqLength, report, repLength);
  
  // Display the report buffer on the UART console (debug purposes)
  REP_DEBUG_MACRO;

  // Parse and return value
  return PL_FC_ParseScienceData(report, repLength, data, errorFlag);
}



/********************************************************************************************************
*                                 PL_FC_GetMCLChanges()
*
* @brief      Get measurement control list of the PL
*
* @param[in]  buffer*      A pointer on a data buffer in which the MCL will be stored       
* @exception  none
* @return     data size
*
********************************************************************************************************/

void PL_FC_GetMCLChanges(uint8_t* buffer, uint16_t* errorFlag){
  
  uint8_t request[HDR_SZ + PL_FC_MCL_CHANGES_REQ_SZ];   // Request buffer
  uint8_t report [HDR_SZ + PL_FC_MCL_CHANGES_REP_SZ];   // Report buffer
  int16_t crc;                                           // CRC variable
 
  // Calculate total length of request and report
  uint16_t reqLength = HDR_SZ + PL_FC_MCL_CHANGES_REQ_SZ; 
  uint16_t repLength = HDR_SZ + PL_FC_MCL_CHANGES_REP_SZ;
  
  // Create message 
  request[0] = PL_MT_FC_REQ;                           // Message type
  request[1] = PL_FC_MCL_CHANGES_REQ_SZ >> 8;          // Message size (HIGH)
  request[2] = PL_FC_MCL_CHANGES_REQ_SZ & 0xFF;        // Message size (LOW)
  request[3] = PL_FC_MCL_CHANGES;                      // Request ID
  crc = UTI_crc16(request, reqLength - CRC_SZ);        // Calculate CRC-16
  request[4] = crc >> 8;                               // CRC (HIGH)
  request[5] = crc & 0xFF;                             // CRC (LOW)
  
  // Display the request buffer on the UART console (debug purposes)
  REQ_DEBUG_MACRO;
  
  // Start communication
  SATI2C_Communicate(request, reqLength, report, repLength);
  
  // Display the report buffer on the UART console (debug purposes)
  REP_DEBUG_MACRO;

  // Parse and return value
  PL_FC_ParseMCLChanges(report, repLength, buffer, errorFlag);
}







uint8_t PL_FC_ScenarioCreate(uint8_t op, uint8_t id, uint8_t index, uint8_t* description, uint16_t* errorFlag){
  
  uint8_t request[HDR_SZ + PL_FC_SCENARIO_CMD_REQ_C_SZ];   // Request buffer
  uint8_t report [HDR_SZ + PL_FC_SCENARIO_CMD_REP_C_SZ];   // Report buffer
  int16_t crc;                                           // CRC variable
  int i;
  
  // Calculate total length of request and report
  uint16_t reqLength = HDR_SZ + PL_FC_SCENARIO_CMD_REQ_C_SZ; 
  uint16_t repLength = HDR_SZ + PL_FC_SCENARIO_CMD_REP_C_SZ;
  
  // Create message 
  request[0] = PL_MT_FC_REQ;                           // Message type
  request[1] = PL_FC_SCENARIO_CMD_REQ_C_SZ >> 8;       // Message size (HIGH)
  request[2] = PL_FC_SCENARIO_CMD_REQ_C_SZ & 0xFF;     // Message size (LOW)
  request[3] = PL_FC_SCENARIO_CMD;                     // Request ID
  request[4] = ADD_SCEN;
  request[5] = id;
  request[6] = index;
  for(i = 0; i < 13; i++)
      request[7+i] = description[i];
  crc = UTI_crc16(request, reqLength - CRC_SZ);        // Calculate CRC-16
  request[20] = crc >> 8;                               // CRC (HIGH)
  request[21] = crc & 0xFF;                             // CRC (LOW)
  
  // Display the request buffer on the UART console (debug purposes)
  REQ_DEBUG_MACRO;
  
  // Start communication
  SATI2C_Communicate(request, reqLength, report, repLength);
  
  // Display the report buffer on the UART console (debug purposes)
  REP_DEBUG_MACRO;

  // Parse and return value
  return PL_FC_ParseScenarioCreate(report, repLength, errorFlag);
}






uint8_t PL_FC_ScenarioDelete(uint8_t id, uint16_t* errorFlag){
  
  uint8_t request[HDR_SZ + PL_FC_SCENARIO_CMD_REQ_D_SZ];   // Request buffer
  uint8_t report [HDR_SZ + PL_FC_SCENARIO_CMD_REP_D_SZ];   // Report buffer
  int16_t crc;                                             // CRC variable
  
  // Calculate total length of request and report
  uint16_t reqLength = HDR_SZ + PL_FC_SCENARIO_CMD_REQ_D_SZ; 
  uint16_t repLength = HDR_SZ + PL_FC_SCENARIO_CMD_REP_D_SZ;
  
  // Create message 
  request[0] = PL_MT_FC_REQ;                           // Message type
  request[1] = PL_FC_SCENARIO_CMD_REQ_D_SZ >> 8;       // Message size (HIGH)
  request[2] = PL_FC_SCENARIO_CMD_REQ_D_SZ & 0xFF;     // Message size (LOW)
  request[3] = PL_FC_SCENARIO_CMD;                     // Request ID
  request[4] = DEL_SCEN;
  request[5] = id;
  crc = UTI_crc16(request, reqLength - CRC_SZ);        // Calculate CRC-16
  request[6] = crc >> 8;                               // CRC (HIGH)
  request[7] = crc & 0xFF;                             // CRC (LOW)
  
  // Display the request buffer on the UART console (debug purposes)
  REQ_DEBUG_MACRO;
  
  // Start communication
  SATI2C_Communicate(request, reqLength, report, repLength);
  
  // Display the report buffer on the UART console (debug purposes)
  REP_DEBUG_MACRO;

  // Parse and return value
  return PL_FC_ParseScenarioDelete(report, repLength, errorFlag);
}






void    PL_FC_ScenarioCommand(uint8_t op, uint8_t id, uint8_t index, uint8_t* description){
}









/*
*********************************************************************************************************
*                                      LOCAL FUNCTIONS
*********************************************************************************************************
*/


// This function interprets the error report received by the PL
void PL_HK_ParseError(uint8_t* report, uint16_t repLength, uint8_t* errors, uint16_t* errorFlag){
  
  int i;
  int size;
  
  // Check if the report is valid (correct message type and ID)
  if(report[0] == PL_MT_HK_REP && report[3] == PL_HK_ERROR)
  {
    size = (report[1]<<8) + report[2];
    
    // Check if the CRC is correct
    if(UTI_crc16(report, size+HDR_SZ) == CRC_OK) {
      
      errors[0] = report[4];
      for(i = 1; i <= report[4]; i++)
        errors[i] = report[4+i];
    }
    
    else if(report[0] == PL_MT_REP_NRDY)
      *errorFlag = REP_NRDY;
    
    else 
      *errorFlag = CRC_ERR;   // The CRC is invalid
  }
  else
    *errorFlag = COM_ERR;     // This is not the packet we expected
}
  



/******************************************************************************/

// This function interprets the temperature report received by the PL
int16_t PL_HK_ParseTemp(uint8_t* report, uint16_t repLength, uint16_t* errorFlag){
  
  // Check if the report is valid (correct message type and ID)
  if(report[0] == PL_MT_HK_REP && report[3] == PL_HK_TEMP)
  {
    // Check if the CRC is correct
    if(UTI_crc16(report, repLength) == CRC_OK)
      return (int16_t) ((report[4] << 8) + report[5]);
        
    else if(report[0] == PL_MT_REP_NRDY)
      *errorFlag = REP_NRDY;
    
    else 
      *errorFlag = CRC_ERR;   // The CRC is invalid
  }
  else
    *errorFlag = COM_ERR;     // This is not the packet we expected
  
  return 0;
}




/******************************************************************************/
  
// This function interprets the scenario ID after the execution function is called
int8_t PL_FC_ParseExec(uint8_t* report, uint16_t repLength, uint16_t* errorFlag){
  
  // Check if the report is valid (correct message type and ID)
  if(report[0] == PL_MT_FC_REP && report[3] == PL_FC_MEAS_EXEC)
  {
    // Check if the CRC is correct
    if(UTI_crc16(report, repLength) == CRC_OK)
      return report[4];
        
    else if(report[0] == PL_MT_REP_NRDY)
      *errorFlag = REP_NRDY;
    
    else 
      *errorFlag = CRC_ERR;   // The CRC is invalid
  }
  else
    *errorFlag = COM_ERR;     // This is not the packet we expected
  
  return 0;
}




/******************************************************************************/
  
// This function interprets the scenario status after requesting the current scenario status
int8_t PL_FC_ParseScenarioStatus(uint8_t* report, uint16_t repLength, uint16_t* errorFlag){
  
  // Check if the report is valid (correct message type and ID)
  if(report[0] == PL_MT_FC_REP && report[3] == PL_FC_SCENARIO_STATUS)
  {
    // Check if the CRC is correct
    if(UTI_crc16(report, repLength) == CRC_OK)
      return report[4];
        
    else if(report[0] == PL_MT_REP_NRDY)
      *errorFlag = REP_NRDY;
    
    else 
      *errorFlag = CRC_ERR;   // The CRC is invalid
  }
  else
    *errorFlag = COM_ERR;     // This is not the packet we expected
  
  return 0;
}




/******************************************************************************/
 
// This function interprets the scenario status after requesting the current scenario status
uint16_t PL_FC_ParseScienceData(uint8_t* report, uint16_t repLength, uint8_t* data, uint16_t* errorFlag){
  
  int i;
  int size;
  
  // Check if the report is valid (correct message type and ID)
  if(report[0] == PL_MT_FC_REP && report[3] == PL_FC_SCIENCE_DATA)
  {
    size = (report[1]<<8) + report[2];
    
    // Check if the CRC is correct
    if(UTI_crc16(report, size+HDR_SZ) == CRC_OK){
      
      for(i = 0; i < size-CRC_SZ; i++) {
        data[i] = report[4+i];
        return size - CRC_SZ;
      }
    }
      
    else if(report[0] == PL_MT_REP_NRDY)
      *errorFlag = REP_NRDY;
    
    else 
      *errorFlag = CRC_ERR;   // The CRC is invalid
  }
  else
    *errorFlag = COM_ERR;     // This is not the packet we expected
  
  return 0;
}



/******************************************************************************/

void PL_FC_ParseMCLChanges(uint8_t* report, uint16_t repLength, uint8_t* data, uint16_t* errorFlag){
  
  int i;
  int size;
  
  // Check if the report is valid (correct message type and ID)
  if(report[0] == PL_MT_FC_REP && report[3] == PL_FC_MCL_CHANGES)
  {
    size = (report[1]<<8) + report[2];
    
    // Check if the CRC is correct
    if(UTI_crc16(report, size+HDR_SZ) == CRC_OK){
      
      for(i = 0; i < size-CRC_SZ; i++)
        data[i] = report[4+i];
    }
      
    else if(report[0] == PL_MT_REP_NRDY)
      *errorFlag = REP_NRDY;
    
    else 
      *errorFlag = CRC_ERR;   // The CRC is invalid
  }
  else
    *errorFlag = COM_ERR;     // This is not the packet we expected
}




/******************************************************************************/

uint8_t PL_FC_ParseScenarioCreate(uint8_t* report, uint16_t repLength, uint16_t* errorFlag){
  
  // Check if the report is valid (correct message type and ID)
  if(report[0] == PL_MT_FC_REP && report[3] == PL_FC_SCENARIO_CMD)
  {
    // Check if the CRC is correct
    if(UTI_crc16(report, repLength) == CRC_OK)
      return report[4];
        
    else if(report[0] == PL_MT_REP_NRDY)
      *errorFlag = REP_NRDY;
    
    else 
      *errorFlag = CRC_ERR;   // The CRC is invalid
  }
  else
    *errorFlag = COM_ERR;     // This is not the packet we expected
  
  return 0;
}




/******************************************************************************/

uint8_t PL_FC_ParseScenarioDelete(uint8_t* report, uint16_t repLength, uint16_t* errorFlag){
  
  // Check if the report is valid (correct message type and ID)
  if(report[0] == PL_MT_FC_REP && report[3] == PL_FC_SCENARIO_CMD)
  {
    // Check if the CRC is correct
    if(UTI_crc16(report, repLength) == CRC_OK)
      return report[4];
        
    else if(report[0] == PL_MT_REP_NRDY)
      *errorFlag = REP_NRDY;
    
    else 
      *errorFlag = CRC_ERR;   // The CRC is invalid
  }
  else
    *errorFlag = COM_ERR;     // This is not the packet we expected
  
  return 0;
}