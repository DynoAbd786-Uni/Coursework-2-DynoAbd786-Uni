#include <stdio.h>

#include "fileStructs.h"
#include "memoryManagement.h"
#include "errorChecking.h"
#include "loadFiles.h"
#include "readFromInputFile.h"
#include "writeToOutputFile.h"
#include "blockHandling.h"
#include "conversionFunctions.h"


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
    int check = setEbcBlockData(outputData);
    if (check != 0)
    {
        freeEbcData(inputData);
        freeEbcBlockData(outputData);
        return check;
    }

    // find block data and store to data struct
    calculateBlockData(outputData);

    // compress data blocks to 5 bits
    convertEbu2Ebc(outputData->blocksUncompressed, outputData->blocksCompressed, outputData->numBlocksUncompressed);

    // free ebcData since data is no longer required
    freeEbcData(inputData);


    /*      OUTPUTTING BLOCKS IMAGE DATA TO FILE       */

    // get and open the output file in write mode
    char *outputFilename = argv[2];
    FILE *outputFile = loadOutputFileBinary(outputFilename);
    // validate that the file has been opened correctly
    if (badFile(outputFile, outputFilename))
    { // validate output file
        freeEbcBlockData(outputData);
        return BAD_FILE;
    } // validate output file

    // output to file
    errCode = outputFileDataCompressedBlockBinary(outputData, outputFile);
    // checking for any error codes
    if (errCode != 0)
    {
        // exit with the error code and free any data used in the program
        freeEbcBlockData(outputData);
        fclose(outputFile);
        return errCode;
    }

    // print final success message, free and return
    printf("BLOCKED\n");
    freeEbcBlockData(outputData);
    fclose(outputFile);
    return SUCCESS;
}