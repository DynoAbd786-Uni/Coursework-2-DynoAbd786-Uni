#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "fileStructs.h"
#include "errorChecking.h"
#include "conversionFunctions.h"
#include "memoryManagement.h"
#include "readFromInputFile.h"

// executes a series of funcions to gather and check all data from an ebf file
// returns respected error code to the error that may have occured in the file, 0 if successful
int getFileData(ebfData *inputData, char* filename, FILE *inputFile)
{   
    // set first 2 characters which should be magic number
    int errCode = getMagicNumber(inputFile, inputData->magicNumber);
    // checking for correct file format
    if (errCode != 0)
    {
        return errCode;
    }

    // checking if the magic number matches the known magic number value
    // checking against the casted value due to endienness.
    if (badMagicNumberEbf(getMagicNumberValue(inputData->magicNumber), filename))
    { // check magic number
        return BAD_MAGIC_NUMBER;
    } // check magic number


    // scan for the dimensions
    // and capture fscanfs return to ensure we got 2 values.
    int check = getDimensions(&inputData->height, &inputData->width, inputFile);
    // check if dimensions satisfy requirements
    if (badDimensions(inputData->height, inputData->width, check, filename))
    { // check dimensions
        return BAD_DIM;
    } // check dimensions

    // set up data array to store pixel values later
    // checks for any error codes that may have been returned
    errCode = setImageDataArray(inputData);
    if (errCode != 0)
    {
        return errCode;
    }

    // get image data from the file and store it to the struct 
    // checks for any error codes that may have been returned
    errCode = getImageDataArray(inputData, inputFile, filename);
    if (errCode != 0)
    {
        return errCode;
    }

    // return 0 for success
    return 0;
}


// read in magic number from file are store it to the struct
// returns 100 if the file format is wrong after reading in the magic number
int getMagicNumber(FILE *inputFile, unsigned char *magicNumber)
{
    // grab magic numbers
    magicNumber[0] = getc(inputFile);
    magicNumber[1] = getc(inputFile);

    // check for correct file format
    if (noWhitespaceOrNull(getc(inputFile)))
    {
        return MISCELLANEOUS;
    }

    return 0;
}


// finds and returns magic number value
unsigned short *getMagicNumberValue(unsigned char *magicNumber)
{
    // casting chars to unsigned short to get magic number value
    unsigned short *magicNumberValue = (unsigned short *) magicNumber;
    return magicNumberValue;   
}


// read in dimensions and return number of values scanned
// returns number of values read from file, or -1 for error
int getDimensions(int *height, int *width, FILE *inputFile)
{
    int check = fscanf(inputFile, "%d %d", height, width);
    return check;
}


/*      FOR EBF FILES       */

// mallocs an array for the data to be stored in.
// returns any error code that may arise during the malloc, 0 for no errors
int setImageDataArray(ebfData *data)
{
    // caclulate total size and allocate memory for array
    data->numBytes = data->height * data->width;
    data->imageData = (unsigned int **) malloc(data->numBytes * sizeof(unsigned int *));

    // if malloc is unsuccessful, it will return a BAD MALLOC error code
    if (badMalloc(data->imageData))
    { // check malloc
        return BAD_MALLOC;
    } // check malloc

    // data block malloc'd to set up 2D array for imageData
    data->dataBlock = (unsigned int *) malloc(data->numBytes * sizeof(unsigned int));

    // if malloc is unsucessful, it will return a null pointer
    if (badMalloc(data->dataBlock))
    { // check malloc
        return BAD_MALLOC;
    } // check malloc

    // pointer arithmetic to set up 2D array 
    for (int row = 0; row < data->height; row++)
    {
        data->imageData[row] = data->dataBlock + row * data->width;
    }
    return 0;
}

// gets image data pixel by pixel and checks against specified parameters
// works in line by line fashion, in case height and width of pixels doesnt match the specified parameters
int getImageDataArray(ebfData *data, FILE *inputFile, char *filename)
{
    // initialise array to null
    unsigned int *inputIntArray = NULL;
    // set up quantity for worst case scenario of integer line ups (2 digit ints plus space across the width, with newline char at the end)
    long Max_Char_Bytes = sizeof(char) * 3 * (data->width + 1);
    // malloc empty buffer to store 1 line of data into as type char *
    char *input = (char *) malloc(Max_Char_Bytes);

    // if malloc is unsucessful, it will return a null pointer
    if (badMalloc(input))
    { // check malloc
        return BAD_MALLOC;
    } // check malloc

    // checks and cycles to next line (sitting on line with the dimensions currently)
    if (noWhitespaceOrNull(getc(inputFile)))
    {
        return MISCELLANEOUS;
    }

    // malloc an array of pointers to unsigned ints
    // used to store numbers when converted from char *input array
    inputIntArray = (unsigned int *) malloc(sizeof(unsigned int) * (data->width + 1));

    // if malloc is unsucessful, it will return a null pointer
    if (badMalloc(inputIntArray))
    { // check malloc
        freeEbfReadDataArrays(input, inputIntArray); 
        return BAD_MALLOC;
    } // check malloc

    // read in each grey value from the file, line by line
    for (int currentRow = 0; currentRow < data->height; currentRow++)
    { // reading in row
        // read in line on row. Error check to see if line has been sucessfully read
        if (noMoreLines(fgets(input, Max_Char_Bytes, inputFile), filename))
        {
            // ensure that allocated data is freed before exit.
            freeEbfReadDataArrays(input, inputIntArray);
            return BAD_DATA;
        }

        // find location of \n inside input string
        int len = strcspn(input, "\n");
        // and overwrite it to a null char (helps with error checking)
        input[len] = '\0';

        // separate array of chars according to whitespace char
        char *token = strtok(input, " ");
        // and keep an account of how many times this has happened
        int sizeOfIntArray = 0;

        // separate the char *array until end of string
        while (token != NULL)
        {
            // write the number obtained from the char *array to the array of ints, according to the incrementing variable sizeOfArray
            inputIntArray[sizeOfIntArray] = (unsigned int) atoi(token);
            sizeOfIntArray++;
            token = strtok(NULL, " ");
        }        

        // BAD DATA CHECK: checks to see if the sizeOfIntArray matches the inputted width
        if (wrongArraySize(data->width, sizeOfIntArray, filename))
        {
            // ensure that allocated data is freed before exit.
            freeEbfReadDataArrays(input, inputIntArray);
            return BAD_DATA;
        }
        
        // for every element inside the line
        for (int currentColumn = 0; currentColumn < data->width; currentColumn++)
        {
            // BAD DATA CHECK: check if data collected is within permitted ranges
            if (badPixelValue(inputIntArray[currentColumn], filename))
            {
                // ensure that allocated data is freed before exit.
                freeEbfReadDataArrays(input, inputIntArray);
                return BAD_DATA;
            }

            // writes every element of the line to the respective position in the imageData array
            data->imageData[currentRow][currentColumn] = inputIntArray[currentColumn];
        }
    }

    // BAD DATA CHECK: check if end of file has been reached (no more lines to read from)
    if (notEndOfFile(inputFile, filename))
    {
        freeEbfReadDataArrays(input, inputIntArray);
        return BAD_DATA;
    }

    // free and dereference char *array and pointer to unsigned int array since the purpose of these has been fulfilled
    freeEbfReadDataArrays(input, inputIntArray);
    return 0;
}


/*      FOR EBU FILES       */

// executes a series of funcions to gather and check all data from an ebu file
// returns respected error code to the error that may have occured in the file, 0 if successful
int getFileDataBinary(ebuData *inputData, char* filename, FILE *inputFile)
{   
    // set first 2 characters which should be magic number
    int errCode = getMagicNumber(inputFile, inputData->magicNumber);
    // checking for correct file format
    if (errCode != 0)
    {
        return errCode;
    }

    // checking if the magic number matches the known magic number value
    // checking against the casted value due to endienness.
    if (badMagicNumberEbu(getMagicNumberValue(inputData->magicNumber), filename))
    { // check magic number
        return BAD_MAGIC_NUMBER;
    } // check magic number


    // scan for the dimensions
    // and capture fscanfs return to ensure we got 2 values.
    int check = getDimensions(&inputData->height, &inputData->width, inputFile);
    // check if dimensions satisfy requirements
    if (badDimensions(inputData->height, inputData->width, check, filename))
    { // check dimensions
        return BAD_DIM;
    } // check dimensions

    // set up data array to store pixel values later
    // checks for any error codes that may have been returned
    errCode = setBinaryImageDataArrayEbu(inputData);
    if (errCode != 0)
    {
        return errCode;
    }

    // get image data from the file and store it to the struct 
    // checks for any error codes that may have been returned
    errCode = getBinaryImageDataArray(inputData, inputFile, filename);
    if (errCode != 0)
    {
        return errCode;
    }

    // return 0 for success
    return 0;
}

// mallocs a binary array for the data to be stored in.
// returns any error code that may arise during the malloc, 0 for no errors
int setBinaryImageDataArrayEbu(ebuData *data)
{
    // caclulate total size and allocate memory for array
    data->numBytes = data->height * data->width;
    data->imageData = (BYTE **) malloc(data->numBytes * sizeof(BYTE *));

    // if malloc is unsuccessful, it will return a BAD MALLOC error code
    if (badMalloc(data->imageData))
    { // check malloc
        return BAD_MALLOC;
    } // check malloc

    // data block malloc'd to set up 2D array for imageData
    data->dataBlock = (BYTE *) malloc(data->numBytes * sizeof(BYTE));

    // if malloc is unsucessful, it will return a null pointer
    if (badMalloc(data->dataBlock))
    { // check malloc
        return BAD_MALLOC;
    } // check malloc

    // pointer arithmetic to set up 2D array 
    for (int row = 0; row < data->height; row++)
    {
        data->imageData[row] = data->dataBlock + row * data->width;
    }
    return 0;
}

// gets image data from an ebu binary file
int getBinaryImageDataArray(ebuData *data, FILE *inputFile, char *filename)
{
    // checks and cycles to next line (sitting on line with the dimensions currently)
    if (noWhitespaceOrNull(getc(inputFile)))
    {
        return MISCELLANEOUS;
    }
    
    // reading directly into the 1D array dataBlock, which should by definition also store to the 2D block
    // keeping a track of count in case # of pixels in file doesnt match # of bytes stated in header of file 
    for (int byteNumber = 0; byteNumber < data->numBytes; byteNumber++)
    {
        // checking if no bytes has been read (for low bad data)
        if (badByteRead(fread(&data->dataBlock[byteNumber], sizeof(BYTE), 1, inputFile), filename))
        {
            return BAD_DATA;
        }
        // checking if pixel value is correct by converting to an int and passing it as an arguement
        else if (badPixelValue(convertEbu2Ebf(data->dataBlock[byteNumber]), filename))
        {
            return BAD_DATA;
        }
    }
    
    // extra bit of code to get rid of the null char so the file indicates EOF
    BYTE tmp;
    fread(&tmp, sizeof(BYTE), 2, inputFile);
    

    // check if end of file has been reached
    if (notEndOfFile(inputFile, filename))
    {
        return BAD_DATA;
    }

    return 0;
}




/*      FOR EBC FILES       */

// executes a series of funcions to gather and check all data from an ebu file
// returns respected error code to the error that may have occured in the file, 0 if successful
int getFileDataCompressedBinary(ebcData *inputData, char* filename, FILE *inputFile)
{   
    // set first 2 characters which should be magic number
    int errCode = getMagicNumber(inputFile, inputData->magicNumber);
    // checking for correct file format
    if (errCode != 0)
    {
        return errCode;
    }

    // checking if the magic number matches the known magic number value
    // checking against the casted value due to endienness.
    if (badMagicNumberEbc(getMagicNumberValue(inputData->magicNumber), filename))
    { // check magic number
        return BAD_MAGIC_NUMBER;
    } // check magic number


    // scan for the dimensions
    // and capture fscanfs return to ensure we got 2 values.
    int check = getDimensions(&inputData->height, &inputData->width, inputFile);
    // check if dimensions satisfy requirements
    if (badDimensions(inputData->height, inputData->width, check, filename))
    { // check dimensions
        return BAD_DIM;
    } // check dimensions

    // set up data array to store pixel values later
    // checks for any error codes that may have been returned
    check = setCompressedBinaryImageDataArrayEbc(inputData);
    if (check != 0)
    {
        return check;
    }

    // get image data from the file and store it to the struct 
    // checks for any error codes that may have been returned
    check = getCompressedBinaryImageDataArray(inputData, inputFile, filename);
    if (check != 0)
    {
        return check;
    }

    // return 0 for success
    return 0;
}

// mallocs a binary array for the data to be stored in.
// returns any error code that may arise during the malloc, 0 for no errors
int setCompressedBinaryImageDataArrayEbc(ebcData *data)
{
    // caclulate total size for uncompressed array
    data->numBytesUncompressed = data->height * data->width;

    // calculates numBytesCompressed 
    // extra bit of logic to account for any overhead in the file
    // if there is a remainder from numBytesUncompressed, there is an extra byte that is storing information that needs to be collected
    if (fmod(data->numBytesUncompressed, COMPRESSION_FACTOR) != 0.0)
    {
        data->numBytesCompressed = ((data->numBytesUncompressed) * (COMPRESSION_FACTOR)) + 1;
    }
    else
    {
        data->numBytesCompressed = ((data->numBytesUncompressed) * (COMPRESSION_FACTOR));
    }


    // uncompressed data block malloc'd
    // 2D imageData not needed since the program can just write the whole block in 1 go
    data->dataBlockUncompressed = (BYTE *) malloc(data->numBytesUncompressed * sizeof(BYTE));

    // if malloc is unsucessful, it will return a null pointer
    if (badMalloc(data->dataBlockUncompressed))
    { // check malloc
        return BAD_MALLOC;
    } // check malloc

    // compressed data block malloc'd to store and convert compressed imageData
    data->dataBlockCompressed = (BYTE *) malloc(data->numBytesCompressed * sizeof(BYTE));

    // if malloc is unsucessful, it will return a null pointer
    if (badMalloc(data->dataBlockCompressed))
    { // check malloc
        return BAD_MALLOC;
    } // check malloc

    return 0;
}

// gets image data from an ebc compressed binary file
int getCompressedBinaryImageDataArray(ebcData *data, FILE *inputFile, char *filename)
{
    // checks and cycles to next line (sitting on line with the dimensions currently)
    if (noWhitespaceOrNull(getc(inputFile)))
    {
        return MISCELLANEOUS;
    }
    
    // read in all the compressed pixel data
    fread(data->dataBlockCompressed, sizeof(BYTE), data->numBytesCompressed, inputFile);

    // decompress data to check individual amount of pixels in file
    // not possible in this case to check pixel values, since compressed size has only 2^5 bits
    // therefore has guaranteed range of 0 to 31
    // check if number of decompressed bytes matches the assigned numBytesUncompressed
    if (badNumBytes(convertEbc2Ebu(data->dataBlockCompressed, data->dataBlockUncompressed, data->numBytesCompressed, data->numBytesUncompressed), data->numBytesUncompressed, filename))
    {
        return BAD_DATA;
    } 
    
    // extra bit of code to get rid of the null char so the file indicates EOF
    BYTE tmp;
    fread(&tmp, sizeof(BYTE), 1, inputFile);
    

    // check if end of file has been reached
    if (notEndOfFile(inputFile, filename))
    {
        return BAD_DATA;
    }

    return 0;
}


