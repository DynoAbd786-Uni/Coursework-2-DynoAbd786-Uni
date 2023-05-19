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

// runs ebcR... algorithms
int ebcCompressToRandomBlock(int argc, char **argv)
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

    // set up ebcRandomBlockData struct and check for any errors
    check = setEbcRandomBlockData(outputData, noParadigmBlocks);
    if (check != 0)
    {
        freeEbcBlockData(dataConversionHolder);
        freeEbcRandomBlockData(outputData);
        return check;
    }

    // calculate image data based on randomisation of blocks
    check = randomiseBlockData(dataConversionHolder, outputData, seedForRandomGenerator);
    if (check != 0)
    {
        freeEbcBlockData(dataConversionHolder);
        freeEbcRandomBlockData(outputData);
        return check;
    }

    // assign paradigm blocks to image blocks
    assignRandomBlocksToImage(outputData, dataConversionHolder);

    // compress image data and paradigm blocks
    compressDataToGivenBit(outputData->dataBlockUncompressed, outputData->dataBlockCompressed, outputData->numBlocksUncompressed, outputData->numBitsCompressed);
    convertEbu2Ebc(outputData->uncompressedParadigmBlocks, outputData->compressedParadigmBlocks, outputData->sizeOfParadigmBlockArrayCompressed);

    // free block data as this is no longer needed
    freeEbcBlockData(dataConversionHolder);

    /*      OUTPUTTING BLOCKS IMAGE DATA TO FILE       */

    // get and open the output file in write mode
    char *outputFilename = argv[2];
    FILE *outputFile = loadOutputFileBinary(outputFilename);
    // validate that the file has been opened correctly
    if (badFile(outputFile, outputFilename))
    { // validate output file
        freeEbcRandomBlockData(outputData);
        return BAD_FILE;
    } // validate output file

    // output to file
    errCode = outputFileDataCompressedRandomBlockBinary(outputData, outputFile);
    // checking for any error codes
    if (errCode != 0)
    {
        // exit with the error code and free any data used in the program
        freeEbcRandomBlockData(outputData);
        fclose(outputFile);
        return errCode;
    }

    // print final success message, free and return
    printf("BLOCKED\n");
    freeEbcRandomBlockData(outputData);
    fclose(outputFile);
    return SUCCESS;
}


// runs ebcU... algorithms
int ebcDecompressFromRandomBlock(int argc, char **argv)
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

    // find out which program is being used to find out how many paradigm blocks are required
    int noParadigmBlocks = calculateNoParadigmBlocks(argv[0]);

    /*      TAKING INPUT FROM FILE      */

    // malloc a struct of type ebfData to store data to
    ebcRandomBlockData *inputData = mallocEbcRandomBlock();
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
        freeEbcRandomBlockData(inputData);
        return BAD_FILE;
    } // check file pointer

    // checking if any error codes arise when getting data (0 means success)
    int errCode = getFileDataCompressedRandomBlockBinary(inputData, inputFilename, noParadigmBlocks, inputFile);
    if (errCode != 0)
    {
        // exit with the error code and free any data used in the program
        freeEbcRandomBlockData(inputData);
        fclose(inputFile);
        return errCode;
    }

    fclose(inputFile);

    /*      CONVERTING TO IMAGE DATA FROM BLOCK FORMAT       */

    // malloc a struct of type ebcData to store data to
    ebcBlockData *dataConversionHolder = mallocEbcBlock();
    // checking if struct has been malloc'd
    if (badMalloc(dataConversionHolder))
    {
        freeEbcRandomBlockData(inputData);
        return BAD_MALLOC;
    }

    // copy over image data and dimensions to the ebcBlockData struct
    copyEbcRandomBlockDataToEbcBlockData(inputData, dataConversionHolder);

    // set up ebcData for compressed data storage
    int check = setEbcBlockData(dataConversionHolder);
    if (check != 0)
    {
        freeEbcBlockData(dataConversionHolder);
        freeEbcRandomBlockData(inputData);
        return check;
    }

    // calculate image data in compressed block format
    check = calculateRandomBlockImageData(inputData, dataConversionHolder);
    if (check != 0)
    {
        freeEbcBlockData(dataConversionHolder);
        freeEbcRandomBlockData(inputData);
        return check;
    }

    // free data struct as it is no longer needed
    freeEbcRandomBlockData(inputData);

    // malloc a struct of type ebcData to store data to
    ebcData *outputData = mallocEbc();
    // checking if struct has been malloc'd
    if (badMalloc(outputData))
    {
        freeEbcBlockData(dataConversionHolder);
        return BAD_MALLOC;
    }

    // copy over image data and dimensions to the ebcData struct
    copyEbcBlockDataToEbcData(dataConversionHolder, outputData);

    // set up ebcData for compressed data storage
    check = setCompressedBinaryImageDataArrayEbc(outputData);
    if (check != 0)
    {
        freeEbcBlockData(dataConversionHolder);
        freeEbcData(outputData);
        return check;
    }

    // compress image data
    convertEbu2Ebc(outputData->dataBlockUncompressed, outputData->dataBlockCompressed, outputData->numBytesUncompressed);

    // free data struct as it is no longer needed
    freeEbcBlockData(dataConversionHolder);

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
