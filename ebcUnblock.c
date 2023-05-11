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
    ebcBlockData *inputData = mallocEbcBlock();
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
        freeEbcBlockData(inputData);
        return BAD_FILE;
    } // check file pointer

    // checking if any error codes arise when getting data (0 means success)
    int errCode = getFileDataCompressedBlockBinary(inputData, inputFilename, inputFile);
    if (errCode != 0)
    {
        // exit with the error code and free any data used in the program
        freeEbcBlockData(inputData);
        fclose(inputFile);
        return errCode;
    }

    fclose(inputFile);


    /*      CONVERTING TO IMAGE DATA FROM BLOCK FORMAT       */

    // malloc a struct of type ebcData to store data to
    ebcData *outputData = mallocEbc();
    // checking if struct has been malloc'd
    if (badMalloc(outputData))
    {
        freeEbcBlockData(inputData);
        return BAD_MALLOC;
    }

    // calculate image data in compressed block format
    calculateImageData(inputData);

    // copy over image data and dimensions to the ebcData struct
    copyEbcBlockDataToEbcData(inputData, outputData);

    // set up ebcData for compressed data storage
    setCompressedBinaryImageDataArrayEbc(outputData);

    // compress image data
    convertEbu2Ebc(outputData->dataBlockUncompressed, outputData->dataBlockCompressed, outputData->numBytesUncompressed);

    // free data struct as it is no longer needed
    freeEbcBlockData(inputData);

    


    /*      OUTPUTTING DATA AS EBC FILE     */

    // get and open the output file in write mode
    char *outputFilename = argv[2];
    FILE *outputFile = loadOutputFileBinary(outputFilename);
    // validate that the file has been opened correctly
    if (badFile(outputFile, outputFilename))
    { // validate output file
        freeEbcData(outputData);
        return BAD_FILE;
    } // validate output file

    
    // output to file
    errCode = outputFileDataCompressedBinary(outputData, outputFile);
    // checking for any error codes
    if (errCode != 0)
    {
        // exit with the error code and free any data used in the program
        freeEbcData(outputData);
        fclose(outputFile);
        return errCode;
    }

    // print final success message, free and return
    printf("UNBLOCKED\n");
    freeEbcData(outputData);
    fclose(outputFile);
    return SUCCESS;

}