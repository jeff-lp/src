/******************************************************************************

Swiss Space Center

Filename: app_command.c
Author:   Louis Masson

Created:  16/05/2013
Modified: 16/05/2013

Description:
This library contains the command task which is responsible for communication
with the other subsystems of the satellite with new commands coming from the 
ground station.
For the development board of the CDMS, this function receives commands through
the UART line and interprets them. The commands allow the user to control the 
payload and test the communication protocol. A list of the available commands
can be seen by typing 'help'.

******************************************************************************/

#include <includes.h>


/*
*********************************************************************************************************
*                                      LOCAL DEFINES
*********************************************************************************************************
*/

// Menu states
#define DISPLAY_STATE   0
#define ADD_SCEN_STATE  1
#define ALT_SCEN_STATE  2
#define DEL_SCEN_STATE  3
#define DEFAULT_STATE   4



// Position of commands in the commandList table
#define ERR     0
#define TMP     1
#define HELP    2
#define EXEC    3
#define MCL     4
#define SCI     5
#define RDY     6
#define FWUP    7
#define FWLD    8
#define SWUP    9
#define ADD     10      
#define ALT     11
#define DEL     12
#define DISP    13

// Total number of commands
#define NB_COM  14



/*
*********************************************************************************************************
*                                      LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/



uint8_t currentState = DEFAULT_STATE;

const char* commandList[NB_COM] = { "err", "tmp", "help", "exec", "mcl",
                                    "sci", "rdy", "fwup", "fwld", "swup",
                                    "add", "alt", "del", "disp"};




/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void printHelp();
void getCommand(char* buffer, uint8_t strlen);
uint8_t interpretCommand(char* buffer);

void defaultState(char* buffer);
void displayState(char* buffer);








/********************************************************************************************************
*                                         APP_Command()
*
* @brief      Command task of the satellite
*
* @param[in]  p_arg       Argument passed to 'APP_TaskOne()' by 'OSTaskCreate()'.
* @exception  none
* @return     none.
*/
/* Notes      :(1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                   used.  The compiler should not generate any code for this statement.
*
********************************************************************************************************/


void APP_Command(void *Ptr_Arg){
  
  (void)Ptr_Arg; /* Note(1) */
  INT8U err;
  int i;
  char buffer[10];
  //static uint8_t currentContext
  
  while(1){
    
    OSMutexPend(sysI2CMutex, 0, &err);    // Wait for resources to be available
    OSMutexPend(dataMutex, 0, &err);
    
    
    // Get command from UART buffer
    getCommand(buffer, 10);
      
    // If a command has been received
    if(buffer[0] != 0) {

      switch(currentState) {
        
      case DEFAULT_STATE:
        defaultState(buffer);
        break;
        
      case DISPLAY_STATE:
        displayState(buffer);
        break;
      }
      
      
      // Prompt the user to enter a new command
      printf("\n\nEnter your command:  ");
    }

    
    // Empty buffer
    for(i = 0; i < 10; i++)
      buffer[i] = 0;
    
    
    // Do stuff
    // Check mailbox to see if PL data transfer is under way
    
    OSMutexPost(dataMutex);
    OSMutexPost(sysI2CMutex);             // Make the resources available to other tasks
    
    
    
    OSTimeDlyHMSM(0, 0, 0, 20);
  }
}



/*
*********************************************************************************************************
*                                      LOCAL FUNCTIONS
*********************************************************************************************************
*/


void getCommand(char* buffer, uint8_t strlen){
  
    int i = 0;
    int c;
    
    while( i++ < strlen ){
      c = getchar();
      if(c == '\n'){
        printf("%s", buffer);
        break;
      }
      else if(c != -1)
        strcat(buffer, (char*) &c);
    }
}

/******************************************************************************/


uint8_t interpretCommand(char* buffer) {
  
  int i;
  
  for(i = 0; i < NB_COM; i++) {
    if( strcmp( buffer, commandList[i]) == 0 )
      break;
  }
        
  return i;
}

/******************************************************************************/

void defaultState(char* buffer) {
  
  int i, j;
  uint16_t errorFlag = 0;
  uint8_t  report[MAX_REPORT_LENGTH];
  
  uint8_t dummy_desc[13] = {1, 0, 1, 0, 1, 0, 0, 0, 10, 0, 10, 0, 0};
  
  switch( interpretCommand(buffer) ){
        
  case HELP:
    printHelp();
    break;

  //---------------
    
  case ERR:
    report[0] = 0;
    
    PL_HK_GetError(report, &errorFlag);
   
    if(errorFlag)
      printf("Communication error: %d\n", errorFlag);
    
    else if(report[0] == 0)
      printf("There are no errors to report.\n");
    
    else {
      printf("\nError codes: ");
      for(i = 1; i <= report[0]; i++)
        printf("%d|", report[i]);
      printf("\n");
    }
    
    break;
    
  //---------------
    
  case TMP:
    j = PL_HK_GetTemperature(&errorFlag);
    
    if(errorFlag)
      printf("Communication error: %d\n", errorFlag);
    
    else
      printf("\nPL Temperature: %d\n", j);
    
    break;
    
  //---------------
        
  case EXEC:
    j = PL_FC_MeasurementExec(&errorFlag);
    
    if(errorFlag)
      printf("Communication error: %d\n", errorFlag);
    else
      printf("\nAllowing PL to exec measurement. Report: %d \n", j);
    
    break;
    
  //---------------
        
  case MCL:
    PL_FC_GetMCLChanges(report, &errorFlag);
    j = report[0];
    
    if(errorFlag)
      printf("Communication error: %d\n", errorFlag);
    
    else {
      printf("\nMeasurement control changes: \n");
      for(i = 0; i < j; i++)
        printf("%d|", report[i]);
      printf("\n");
    }
   
    break;
    
  //---------------
        
  case SCI:
    j = PL_FC_GetScienceData(report, &errorFlag);
    
    if(errorFlag)
      printf("Communication error: %d\n", errorFlag);
    
    else {
      printf("\nScientific data: \n");
      for(i = 1; i < j; i++)
        printf("%d|", report[i]);
      printf("\n");
    }
    
    break;
    
  //---------------
        
  case RDY:
    j = PL_FC_GetScenarioStatus(&errorFlag);
    
    if(errorFlag)
      printf("Communication error: %d\n", errorFlag);
    
    else
      printf("\nScenario status: %d \n", j);
    
    break;
        
  //---------------
    
  case FWUP:
    printf("\nFirmware update sent to PL. Report: \n");
    break;
    
  //---------------
        
  case FWLD:
    printf("\nAllowing PL to load firmware. Report: \n");
    break;
    
  //---------------
        
  case SWUP:
    printf("\nSofware update sent to PL. Report: \n");
    break;
   
  //---------------
    
  case DISP:
    APP_SerialDisplayEn();
    currentState = DISPLAY_STATE;
    break;
    
  //---------------
       
  case ADD:
    j = PL_FC_ScenarioCreate(0, 1, 0, dummy_desc, &errorFlag);
  
    if(errorFlag)
      printf("Communication error: %d\n", errorFlag);
    
    else
      printf("\nCreating scenario. Report: %d \n", j);
    
    break;
    
  //---------------
        
  case ALT:
    break;
    
  //---------------
       
  case DEL:
    j = PL_FC_ScenarioDelete(1, &errorFlag);
    
    if(errorFlag)
      printf("Communication error: %d\n", errorFlag);
    
    else
      printf("\nScenario %d deleted. \n", j);
    
    break;
    
  //---------------
        
  default:
    printf("\nUnrecognized command !");
    break;             
  }
}

/******************************************************************************/

void displayState(char* buffer){
  
  APP_SerialDisplayDis();
  currentState = DEFAULT_STATE;
}


/******************************************************************************/

void printHelp() {
  printf("\n\nAvailable CDMS commands for PL :\n");
  printf("-------------------------\n");
  printf("  add  : create a new scenario\n");
  printf("  alt  : modify an existing scenario\n");
  printf("  del  : delete an existing scenario\n");
  printf("  disp : display diagnostics (any key to cancel)\n");
  printf("  err  : get error codes\n");
  printf("  exec : allow measurement execution\n");
  printf("  fwld : firmware load\n");
  printf("  fwup : firmware update\n");
  printf("  help : get list of available commands\n");
  printf("  mcl  : get Measurement Control List\n");
  printf("  rdy  : get scenario status\n");
  printf("  sci  : get scientific data\n");
  printf("  swup : software update\n");
  printf("  tmp : get temperature\n");
}






