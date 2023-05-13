#include <stdio.h>
#include <stdlib.h>

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
    if (badArguementsRandom(argc))
    { // check arg count
        return BAD_ARGS;
    } // check arg count

    // find out which program is being used to find out how many paradigm blocks are required
    int noParadigmBlocks = calculateNoParadigmBlocks(argv[0]);

    // save the seed from command line
    int seedForRandomGenerator = atoi(argv[3]);


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
    ebcBlockData *dataConversionHolder = mallocEbcBlock();
    // checking if struct has been malloc'd
    if (badMalloc(dataConversionHolder))
    {
        freeEbcData(inputData);
        return BAD_MALLOC;
    }

    // copy over data from ebcData struct to ebcBlockData struct for easier data handling
    copyEbcDataToEbcBlockData(inputData, dataConversionHolder);

    // set up the rest of ebcBlockData
    // checks for any error codes that may have been returned
    int check = setEbcBlockData(dataConversionHolder);
    if (check != 0)
    {
        freeEbcData(inputData);
        freeEbcBlockData(dataConversionHolder);
        return check;
    }

    // find block data and store to data struct
    calculateBlockData(dataConversionHolder);

    // free ebcData since data is no longer required
    freeEbcData(inputData);


    /*      RANDOMISING BLOCKS      */

    // malloc a struct of type ebcBlockData to store data to
    ebcRandomBlockData *outputData = mallocEbcRandomBlock();
    // checking if struct has been malloc'd
    if (badMalloc(outputData))
    {
        freeEbcBlockData(dataConversionHolder);
        return BAD_MALLOC;
    }

    copyEbcBlockDataToEbcRandomBlockData(dataConversionHolder, outputData);

    // set up ebcRandomBlockData struct
    check = setEbcRandomBlockData(outputData, noParadigmBlocks);
    if (check != 0)
    {
        freeEbcBlockData(dataConversionHolder);
        freeEbcRandomBlockData(outputData);
        return check;
    }

    // calculate image data based on randomisation of blocks
    randomiseBlockData(dataConversionHolder, outputData, seedForRandomGenerator);

}


    

    

