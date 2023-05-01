#include <stdio.h>

#include "fileStructs.h"
#include "memoryManagement.h"
#include "errorChecking.h"
#include "loadFiles.h"
#include "readFromInputFile.h"
#include "writeToOutputFile.h"
#include "blockHandling.h"


int main(int argc, char **argv)
{
    /*      CHEKCING ARGUEMENTS     */

    // Provide the user with correct usage if no arguements are provided
    if (noArguements(argc, argv))
    {
        return SUCCESS;
    }

    // validate that user has enter 2 arguments (plus the executable name)
    if (badArguements(argc))
    { // check arg count
        return BAD_ARGS;
    } // check arg count


    /*      TAKING INPUT FROM FILE      */

    // malloc a struct of type ebfData to store data to
    ebcData *inputData = mallocEbc();
    // checking if struct has been malloc'd
    if (badMalloc(inputData))
    {
        return BAD_MALLOC;
    }

    // get and open the input file in read mode
    char *inputFilename = argv[1];
    FILE *inputFile = loadInputFileBinary(inputFilename);
    // check file opened successfully
    if (badFile(inputFile, inputFilename))
    { // check file pointer
        // free data and exit
        freeEbcData(inputData);
        return BAD_FILE;
    } // check file pointer

    // checking if any error codes arise when getting data (0 means success)
    int errCode = getFileDataCompressedBinary(inputData, inputFilename, inputFile);
    if (errCode != 0)
    {
        // exit with the error code and free any data used in the program
        freeEbcData(inputData);
        fclose(inputFile);
        return errCode;
    }

    fclose(inputFile);


    /*      CONVERTING IMAGE DATA INTO BLOCKS     */

    // malloc a struct of type ebcBlockData to store data to
    ebcBlockData *outputData = mallocEbcBlock();
    // checking if struct has been malloc'd
    if (badMalloc(outputData))
    {
        freeEbcData(inputData);
        return BAD_MALLOC;
    }

    // copy over data from ebcData struct to ebcBlockData struct for easier data handling
    copyEbcDataToEbcBlockData(inputData, outputData);

    // set up the rest of ebcBlockData
    // checks for any error codes that may have been returned
    check = setEbcBlockData(outputData);
    if (check != 0)
    {
        return check;
    }

    calculateBlockData(outputData);

    




    
    




    // compress blocks
    // output block file data



}