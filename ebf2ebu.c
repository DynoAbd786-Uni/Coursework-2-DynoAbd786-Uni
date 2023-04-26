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
    { // check arg count
        return BAD_ARGS;
    } // check arg count


    /*      TAKING INPUT FROM FILE      */

    // malloc a struct of type ebfData to store data to
    ebfData *inputData = mallocEbf();
    // checking if struct has been malloc'd
    if (badMalloc(inputData))
    {
        return BAD_MALLOC;
    }

    // get and open the input file in read mode
    char *inputFilename = argv[1];
    FILE *inputFile = loadInputFile(inputFilename);
    // check file opened successfully
    if (badFile(inputFile, inputFilename))
    { // check file pointer
        // free data and exit
        freeEbfData(inputData);
        return BAD_FILE;
    } // check file pointer

    // checking if any error codes arise when getting data (0 means success)
    int errCode = getFileData(inputData, inputFilename, inputFile);
    if (errCode != 0)
    {
        // exit with the error code and free any data used in the program
        freeEbfData(inputData);
        fclose(inputFile);
        return errCode;
    }

    fclose(inputFile);


    /*      OUTPUTTING AS AN EBU FILE       */

    // get and open the output file in write mode
    char *outputFilename = argv[2];
    FILE *outputFile = loadOutputFileBinary(outputFilename);
    // validate that the file has been opened correctly
    if (badFile(outputFile, outputFilename))
    { // validate output file
        return BAD_FILE;
    } // validate output file

    
    // output to file
    errCode = outputFileDataEbfDirectEbu(inputData, outputFile);
    // checking for any error codes
    if (errCode != 0)
    {
        // exit with the error code and free any data used in the program
        freeEbfData(inputData);
        fclose(outputFile);
        return errCode;
    }

    // print final success message, free and return
    printf("CONVERTED\n");
    freeEbfData(inputData);
    fclose(outputFile);
    return SUCCESS;
}