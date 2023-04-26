#include <stdio.h>

#include "fileStructs.h"
#include "memoryManagement.h"
#include "errorChecking.h"
#include "loadFiles.h"
#include "readFromInputFile.h"
#include "writeToOutputFile.h"


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
    { 
        return BAD_ARGS;
    }


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
    { 
        freeEbcData(inputData);
        return BAD_FILE;
    }

    // checking if any error codes arise when getting data (0 means success)
    int errCode = getFileDataCompressedBinary(inputData, inputFilename, inputFile);
    if (errCode != 0)
    {
        // exit with the error code and free any data used in the program
        freeEbcData(inputData);
        fclose(inputFile);
        return errCode;
    }

    // close the file
    fclose(inputFile);


    /*      OUTPUTTING AS AN EBC FILE     */

    // get and open the output file in write mode
    char *outputFilename = argv[2];
    FILE *outputFile = loadOutputFileBinary(outputFilename);
    // validate that the file has been opened correctly
    if (badFile(outputFile, outputFilename))
    {
        return BAD_FILE;
    }

    // output to ebu file
    errCode = outputFileDataEbcDirectEbu(inputData, outputFile);
    // checking for any error codes
    if (errCode != 0)
    {
        // exit with the error code and free any data used in the program
        freeEbcData(inputData);
        fclose(outputFile);
        return errCode;
    }

    // print final success message, free and return
    printf("CONVERTED\n");
    freeEbcData(inputData);
    fclose(outputFile);
    return SUCCESS;
}