


#include <includes.h>


// Generator polynomial for CRC-16
#define POLY_16 0x8005

// Size of the tables which contain precalculated values
#define CRC_TABLE_SIZE 256








// Table which holds precalculated CRC-16 values
static uint16_t sTable16[CRC_TABLE_SIZE]; 


// Flag which indicates if the table for CRC-16 already is calculated
static bool sTable16Calculated = FALSE;

// Function for precomputing CRC-16 table
static void calcTable16(void);









//----------------------------------------------

uint16_t UTI_crc16(uint8_t* pData, unsigned int length)
{
    // Calculate table if not already done
    if(!sTable16Calculated)
    {
        calcTable16();
    }
    
    // Set initial checksum value
    uint16_t crc = 0x0000;
    
    // Step trough the single bytes
    for (int i = 0; i < length; ++i)
    {
        // Calculate the CRC-16 with the precalculated values from the table
        crc = sTable16[pData[i] ^ (crc >> 8)] ^ (crc << 8);
    }

    return crc;
}



//----------------------------------------------

static void calcTable16(void)
{
    // Temporary CRC-16 value
    uint16_t crc;

    for (uint16_t i = 0; i < CRC_TABLE_SIZE; i++)
    {
        crc = i << 8;

        uint8_t j = 8;
        while(j--)
        {
            // Calculate CRC-16 value
            if (crc & 0x8000)
            {
                crc = (crc << 1) ^ POLY_16;
            }
            else
            {
                crc = (crc << 1);
            }
        }
        // Store value into table
        sTable16[i] = crc;
    }
    // Set flag to prevent multiple table calculations
    sTable16Calculated = TRUE;
}







