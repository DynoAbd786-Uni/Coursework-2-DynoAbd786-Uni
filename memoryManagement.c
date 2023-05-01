#include <stdio.h>
#include <stdlib.h>

#include "fileStructs.h"
#include "errorChecking.h"
#include "memoryManagement.h"


/*      EBF DATA MEMORY RELATED FUNCTIONS      */

// mallocs an uninitialised struct of type ebfData to store file information to
// returns the data struct
ebfData *mallocEbf()
{
    ebfData *data = (ebfData *) malloc(sizeof(ebfData));
    return data;
}

// frees 1 data entry from the type ebfData
void freeEbfData(ebfData *data)
{
    // checks if imageData has been malloc'd
    if (data->imageData != NULL)
    {
        // free and dereference imageData to NULL to avoid hanging pointer
        free(data->imageData);
        data->imageData = NULL;
    }

    // checks if dataBlock has been malloc'd
    if (data->dataBlock != NULL)
    {
        // free and dereference dataBlock to NULL to avoid hanging pointer
        free(data->dataBlock);
        data->dataBlock = NULL;
    }

    // checks if data struct has been malloc'd
    if (data != NULL)
    {
        // free and dereference data struct to NULL to avoid hanging pointer
        free(data);
        data = NULL;
    }
}

// frees an array of structs (for comp files related to ebf files)
void freeEbfDataArray(ebfData *dataToCompare[])
{
    // for the 2 structs in the file storing data 
    for (int fileNumber = 0; fileNumber < MAX_FILE_COMPARISON; fileNumber++)
    {
        // check if the struct at position fileNumber has some data. if so, free the struct
        if (dataToCompare[fileNumber] != NULL)
        {
            freeEbfData(dataToCompare[fileNumber]);
        }
    }
}

// frees arrays related to reading in a file and storing the data
void freeEbfReadDataArrays(char *input, unsigned int *inputIntArray)
{
    // checks if input array has been malloc'd
    if (input != NULL)
    {
        // free and dereference input to NULL to avoid hanging pointer
        free(input);
        input = NULL;
    }

    // checks if inputIntArray struct has been malloc'd
    if (inputIntArray != NULL)
    {
        // free and dereference inputIntArray to NULL to avoid hanging pointer
        free(inputIntArray);
        inputIntArray = NULL;
    }
}


/*      EBU DATA MEMORY RELATED FUNCTIONS      */

// mallocs an uninitialised struct of type ebuData to store file information to
// returns the data struct
ebuData *mallocEbu()
{
    ebuData *data = (ebuData *) malloc(sizeof(ebuData));
    return data;
}

// frees 1 data entry from the type ebuData
void freeEbuData(ebuData *data)
{
    // checks if imageData has been malloc'd
    if (data->imageData != NULL)
    {
        // free and dereference imageData to NULL to avoid hanging pointer
        free(data->imageData);
        data->imageData = NULL;
    }

    // checks if dataBlock has been malloc'd
    if (data->dataBlock != NULL)
    {
        // free and dereference dataBlock to NULL to avoid hanging pointer
        free(data->dataBlock);
        data->dataBlock = NULL;
    }

    // checks if data struct has been malloc'd
    if (data != NULL)
    {
        // free and dereference data struct to NULL to avoid hanging pointer
        free(data);
        data = NULL;
    }
}

// frees an array of structs (for comp files related to ebu files)
void freeEbuDataArray(ebuData *dataToCompare[])
{
    // for the 2 structs in the file storing data 
    for (int fileNumber = 0; fileNumber < MAX_FILE_COMPARISON; fileNumber++)
    {
        // check if the struct at position fileNumber has some data. if so, free the struct
        if (dataToCompare[fileNumber] != NULL)
        {
            freeEbuData(dataToCompare[fileNumber]);
        }
    }
}


/*      EBC DATA MEMORY RELATED FUNCTIONS      */

// mallocs an uninitialised struct of type ebcData to store file information to
// returns the data struct
ebcData *mallocEbc()
{
    ebcData *data = (ebcData *) malloc(sizeof(ebcData));
    return data;
}

// frees 1 data entry from the type ebcData
void freeEbcData(ebcData *data)
{
    // checks if dataBlockUncompressed has been malloc'd
    if (data->dataBlockUncompressed != NULL)
    {
        // free and dereference dataBlock to NULL to avoid hanging pointer
        free(data->dataBlockUncompressed);
        data->dataBlockUncompressed = NULL;
    }

    // checks if dataBlockCompressed has been malloc'd
    if (data->dataBlockCompressed != NULL)
    {
        // free and dereference dataBlock to NULL to avoid hanging pointer
        free(data->dataBlockCompressed);
        data->dataBlockCompressed = NULL;
    }

    // checks if data struct has been malloc'd
    if (data != NULL)
    {
        // free and dereference data struct to NULL to avoid hanging pointer
        free(data);
        data = NULL;
    }
}

// frees an array of structs (for comp files related to ebc files)
void freeEbcDataArray(ebcData *dataToCompare[])
{
    // for the 2 structs in the file storing data 
    for (int fileNumber = 0; fileNumber < MAX_FILE_COMPARISON; fileNumber++)
    {
        // check if the struct at position fileNumber has some data. if so, free the struct
        if (dataToCompare[fileNumber] != NULL)
        {
            freeEbcData(dataToCompare[fileNumber]);
        }
    }
}


/*      EBC BLOCK DATA MEMORY RELATED FUNCTIONS     */

// mallocs an uninitialised struct of type ebcBlockData to store file information to
// returns the data struct
ebcBlockData *mallocEbcBlock()
{
    ebcBlockData *data = (ebcBlockData *) malloc(sizeof(ebcBlockData));
    return data;
}

// frees 1 data entry from the type ebcBlockData
void freeEbcBlockData(ebcBlockData *data)
{
    // checks if blocksCompressed has been malloc'd
    if (data->blocksCompressed != NULL)
    {
        // free and dereference dataBlock to NULL to avoid hanging pointer
        free(data->blocksCompressed);
        data->blocksCompressed = NULL;
    }

    // checks if blocksUncompressed has been malloc'd
    if (data->blocksUncompressed != NULL)
    {
        // free and dereference dataBlock to NULL to avoid hanging pointer
        free(data->blocksUncompressed);
        data->blocksUncompressed = NULL;
    }

    // checks if imageDataUncompressed has been malloc'd
    if (data->imageDataUncompressed != NULL)
    {
        // free and dereference imageDataUncompressed to NULL to avoid hanging pointer
        free(data->imageDataUncompressed);
        data->imageDataUncompressed = NULL;
    }

    // checks if dataBlockUncompressed has been malloc'd
    if (data->dataBlockUncompressed != NULL)
    {
        // free and dereference dataBlockUncompressed to NULL to avoid hanging pointer
        free(data->dataBlockUncompressed);
        data->dataBlockUncompressed = NULL;
    }

    // checks if data struct has been malloc'd
    if (data != NULL)
    {
        // free and dereference data struct to NULL to avoid hanging pointer
        free(data);
        data = NULL;
    }
}

