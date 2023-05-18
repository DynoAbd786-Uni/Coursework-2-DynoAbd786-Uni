#include "fileStructs.h"
#include "conversionFunctions.h"

// function coverts 1 pixel value from type ebfData file to binary format.
// does a bitwise calculation to find the 8 least significant bits from the 4 byte unsigned int type
// error checking unnecessary since pixel values have already been checked to be within limits
BYTE convertEbf2Ebu(unsigned int pixelValue)
{
    BYTE byteValue = (BYTE) (pixelValue & 0xFF);        // adapted from ChatGPT. Query: "Can i cast an unsigned int ** to uint8_t **"
    return byteValue;
}

// function coverts 1 pixel value from type ebuData file to readable numeric format.
unsigned int convertEbu2Ebf(BYTE pixelValueBinary)
{
    unsigned int pixelValue = (unsigned int) pixelValueBinary;
    return pixelValue;
}



/*      THESE ARE DEFAULT COMPRESSION ALGORITHMS USED TO COMPRESS TO AND FROM MAX_BITS_IN_UNCOMPRESSED_BYTE     */

// function converts an array of uncompressed binary pixels to compressed binary format 
void convertEbu2Ebc(BYTE *uncompressedPixelValues, BYTE *compressedBinaryArray, int numBytesUncompressed)
{    
    // variables for storing and tracking compressed binary information
    BYTE storageByte = 0;
    int compressedBitPostitionTracker = 0;
    long compressedBinaryArrayPostitionTracker = 0;

    // for every byte stored inside the uncompressed byte array
    for (int byteNumber = 0; byteNumber < numBytesUncompressed; byteNumber++)
    {
        // for the first 5 bits inside the selected byte, starting at byte 5
        for (int bitNumber = MAX_BITS_IN_UNCOMPRESSED_BYTE; bitNumber > 0; bitNumber--)
        {
            // grab the bit at the position bitNumber
            int bitAtBitNumber = (uncompressedPixelValues[byteNumber] >> (bitNumber - 1)) & 0x01;
            // overwrite the storageByte to contain the LSB and the information of the storageByte at correct positions
            storageByte = (storageByte << 1) | bitAtBitNumber;

            // moving to the next bit
            compressedBitPostitionTracker++;            
            
            // checking if the byte is full of information
            if (compressedBitPostitionTracker == MAX_BITS_IN_BYTE)
            {
                // write the storageByte to the compressed array and increment the array to the next position
                compressedBinaryArray[compressedBinaryArrayPostitionTracker] = storageByte;
                compressedBinaryArrayPostitionTracker++;
                // reset all variables for the next byte
                storageByte = 0;
                compressedBitPostitionTracker = 0;
            }
        }
    }

    // shift bytes to remaining positions in case the byte isnt full
    storageByte = (storageByte << (MAX_BITS_IN_BYTE - compressedBitPostitionTracker));

    // output final byte to the compressed array
    compressedBinaryArray[compressedBinaryArrayPostitionTracker] = storageByte;
}

// function converts an array of compressed binary pixels to uncompressed binary format
// works within the data struct since arrays for both compressed and uncompressed binary arrays have been defined
// reason for this: need to check for amount of pixels, so only way to do that is to decompress image data
long convertEbc2Ebu(BYTE *compressedPixelValues, BYTE *uncompressedBinaryArray, int numBytesCompressed, int numBytesUncompressed)
{
    // variables for storing and tracking compressed binary information
    BYTE storageByte = 0;
    long uncompressedBinaryArrayPostitionTracker = 0;
    int uncompressedBitPostitionTracker = 0;

    // for every compressed byte inside the compressed array
    for (int byteNumber = 0; byteNumber < numBytesCompressed; byteNumber++)
    {
        // for every bit inside every byte
        for (int bitNumber = MAX_BITS_IN_BYTE; bitNumber > 0; bitNumber--)
        {
            // grab the bit at the position bitNumber
            int bitAtBitNumber = (compressedPixelValues[byteNumber] >> (bitNumber - 1)) & 0x01;
            // write the bit to the storage byte and overwrite it with the bits shifted left by 1
            storageByte = (storageByte << 1) | bitAtBitNumber;
            // increment to keep track of how much the byte has been filled by
            uncompressedBitPostitionTracker++;

            // checking if the byte is full of information
            if (uncompressedBitPostitionTracker == MAX_BITS_IN_UNCOMPRESSED_BYTE)
            {
                // accounting for the problem where if the algorithm reaches the end of the array, NULL chars may be picked up
                // this happens when the compressed array has numBytesCompressed that isnt divisible by the COMPRESSION_FACTOR
                if (!(storageByte == 0 && uncompressedBinaryArrayPostitionTracker >= numBytesUncompressed))
                {
                    // write the storageByte to the uncompressed array and increment the array to the next position
                    uncompressedBinaryArray[uncompressedBinaryArrayPostitionTracker] = storageByte;
                    uncompressedBinaryArrayPostitionTracker++;
                }

                // reset all variables for the next byte
                storageByte = 0;
                uncompressedBitPostitionTracker = 0;
                
            }
        }
    }

    // returns the number of decompressed bytes read into the array
    return uncompressedBinaryArrayPostitionTracker;
}



/*      THESE ARE SPECIALISED COMPRESSION ALGORITHMS USED TO COMPRESS TO AND FROM numBitsToCompressTo     */

// function converts an array of uncompressed binary pixels to compressed binary format, with a given bit to compress to 
void compressDataToGivenBit(BYTE *uncompressedPixelValues, BYTE *compressedBinaryArray, int numBytesUncompressed, int numBitsToCompressTo)
{    
    // variables for storing and tracking compressed binary information
    BYTE storageByte = 0;
    int compressedBitPostitionTracker = 0;
    long compressedBinaryArrayPostitionTracker = 0;

    // for every byte stored inside the uncompressed byte array
    for (int byteNumber = 0; byteNumber < numBytesUncompressed; byteNumber++)
    {
        // for the first 5 bits inside the selected byte, starting at byte 5
        for (int bitNumber = numBitsToCompressTo; bitNumber > 0; bitNumber--)
        {
            // grab the bit at the position bitNumber
            int bitAtBitNumber = (uncompressedPixelValues[byteNumber] >> (bitNumber - 1)) & 0x01;
            // overwrite the storageByte to contain the LSB and the information of the storageByte at correct positions
            storageByte = (storageByte << 1) | bitAtBitNumber;

            // moving to the next bit
            compressedBitPostitionTracker++;            
            
            // checking if the byte is full of information
            if (compressedBitPostitionTracker == MAX_BITS_IN_BYTE)
            {
                // write the storageByte to the compressed array and increment the array to the next position
                compressedBinaryArray[compressedBinaryArrayPostitionTracker] = storageByte;
                compressedBinaryArrayPostitionTracker++;
                // reset all variables for the next byte
                storageByte = 0;
                compressedBitPostitionTracker = 0;
            }
        }
    }

    // shift bytes to remaining positions in case the byte isnt full
    storageByte = (storageByte << (MAX_BITS_IN_BYTE - compressedBitPostitionTracker));

    // output final byte to the compressed array
    compressedBinaryArray[compressedBinaryArrayPostitionTracker] = storageByte;
}


// function converts an array of compressed binary pixels to uncompressed binary format, with a given bit to decompress from
long decompressDataFromGivenBit(BYTE *compressedPixelValues, BYTE *uncompressedBinaryArray, int numBytesCompressed, int numBytesUncompressed, int numBitsToDecompressFrom)
{
    // variables for storing and tracking compressed binary information
    BYTE storageByte = 0;
    long uncompressedBinaryArrayPostitionTracker = 0;
    int uncompressedBitPostitionTracker = 0;

    // for every compressed byte inside the compressed array
    for (int byteNumber = 0; byteNumber < numBytesCompressed; byteNumber++)
    {
        // for every bit inside every byte
        for (int bitNumber = MAX_BITS_IN_BYTE; bitNumber > 0; bitNumber--)
        {
            // grab the bit at the position bitNumber
            int bitAtBitNumber = (compressedPixelValues[byteNumber] >> (bitNumber - 1)) & 0x01;
            // write the bit to the storage byte and overwrite it with the bits shifted left by 1
            storageByte = (storageByte << 1) | bitAtBitNumber;
            // increment to keep track of how much the byte has been filled by
            uncompressedBitPostitionTracker++;

            // checking if the byte is full of information
            if (uncompressedBitPostitionTracker == numBitsToDecompressFrom)
            {
                // accounting for the problem where if the algorithm reaches the end of the array, NULL chars may be picked up
                // this happens when the compressed array has numBytesCompressed that isnt divisible by the COMPRESSION_FACTOR
                if (!(storageByte == 0 && uncompressedBinaryArrayPostitionTracker >= numBytesUncompressed))
                {
                    // write the storageByte to the uncompressed array and increment the array to the next position
                    uncompressedBinaryArray[uncompressedBinaryArrayPostitionTracker] = storageByte;
                    uncompressedBinaryArrayPostitionTracker++;
                }

                // reset all variables for the next byte
                storageByte = 0;
                uncompressedBitPostitionTracker = 0;
                
            }
        }
    }

    // returns the number of decompressed bytes read into the array
    return uncompressedBinaryArrayPostitionTracker;
}
