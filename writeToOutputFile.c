#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "fileStructs.h"
#include "conversionFunctions.h"
#include "errorChecking.h"
#include "memoryManagement.h"
#include "writeToOutputFile.h"


// executes a series of functions to write to a file
// returns respective error codes if output has failed, otherwise 0 is successful
int outputFileData(ebfData *data, FILE *outputFile)
{
    // output header to file and validate for success (0 means success)
    if (outputHeader(data->magicNumber, data->height, data->width, outputFile) != 0)
    {
        return BAD_OUTPUT;
    }
    // output image data to file and validate for success (0 means success)
    else if (outputImageData(data->imageData, data->height, data->width, outputFile) != 0)
    {
        return BAD_OUTPUT;
    }

    return 0;
}


// writes the header name and length, width parameters to the output file
// returns error code if output has failed, 0 otherwise
int outputHeader(unsigned char *magicNumber, int height, int width, FILE *outputFile)
{
    // write the header data in one block
    // and use the return from fprintf to check that we wrote.
    if (badOutput(fprintf(outputFile, "%s\n%d %d\n", (char *) magicNumber, height, width)))
    { // check write
        return BAD_OUTPUT;
    } // check write
    return 0;
}

// writes all pixel values to the output file
// returns error code if output has failed, 0 if successful
int outputImageData(unsigned int **imageData, int height, int width, FILE *outputFile)
{
    // iterate though the array and print out pixel values
    for (int currentRow = 0; currentRow < height; currentRow++)
    { // writing out row
        for (int currentColumn = 0; currentColumn < width; currentColumn++)
        { // writing out column
            // output to file and validate for success
            if (badOutput(fprintf(outputFile, "%u", imageData[currentRow][currentColumn])))
            { // check write
                return BAD_OUTPUT;
            } // check write

            // if end of line has not been reached
            if ((currentColumn + 1) / width != 1)
            {
                // output whitespace to file
                // validate if output to file was sucessful
                if (badOutput(fprintf(outputFile, " ")))
                { // check write
                    return BAD_OUTPUT;
                } // check write
            }
            // else if end of line is reached, but end of row has not been reached
            else if ((currentColumn + 1) / width == 1 && currentRow != height - 1)
            {
                // output newline to file
                // validate if output to file was sucessful
                if (badOutput(fprintf(outputFile, "\n")))
                { // check write
                    return BAD_OUTPUT;
                } // check write
            }
        }
    }

    return 0;
}


// function takes all data for ebf file and outputs it to ebu file
// returns respective error codes if failed, 0 if otherwise
int outputFileDataEbfDirectEbu(ebfData *data, FILE *outputFile)
{
    // define the header that needs to be outputted to the file
    unsigned char *header = (unsigned char *) "eu";
    // output header to file and validate for success (0 means success)
    if (outputHeader(header, data->height, data->width, outputFile) != 0)
    {
        return BAD_OUTPUT;
    }
    // output image data to file and validate for success (0 means success)
    else if (outputImageDataEbfDirectEbu(data->imageData, data->height, data->width, outputFile) != 0)
    {
        return BAD_OUTPUT;
    }

    return 0;
}

// writes all pixel values from ebf to the ebu output file in binary format 
// returns error code if output has failed, 0 if successful
int outputImageDataEbfDirectEbu(unsigned int **imageData, int height, int width, FILE *outputFile)
{
    // iterate though the array and print out pixel values
    for (int currentRow = 0; currentRow < height; currentRow++)
    { // writing out row
        for (int currentColumn = 0; currentColumn < width; currentColumn++)
        { // writing out column
            // convert pixel value to binary format
            BYTE pixelValueByte = convertEbf2Ebu(imageData[currentRow][currentColumn]);
            // run and check if the output was successful
            if (badOutput((int) fwrite(&pixelValueByte, sizeof(BYTE), 1, outputFile)))
            { // check write
                return BAD_OUTPUT;
            } // check write
        }
    }

    return 0;
}


// writes all pixel values from ebu binary format to the ebf output file in readable format
// returns error code if output has failed, 0 if successful
int outputFileDataEbuDirectEbf(ebuData *data, FILE *outputFile)
{
    // define the header that needs to be outputted to the file
    unsigned char *header = (unsigned char *) "eb";
    // output header to file and validate for success (0 means success)
    if (outputHeader(header, data->height, data->width, outputFile) != 0)
    {
        return BAD_OUTPUT;
    }
    // output image data to file and validate for success (0 means success)
    else if (outputImageDataEbuDirectEbf(data->imageData, data->height, data->width, outputFile) != 0)
    {
        return BAD_OUTPUT;
    }

    return 0;
}

// outputs image data from binary format to readable format
// returns error code if output has failed, 0 if successful
int outputImageDataEbuDirectEbf(BYTE **imageData, int height, int width, FILE *outputFile)
{
    // iterate though the array and print out pixel values
    for (int currentRow = 0; currentRow < height; currentRow++)
    { // writing out row
        for (int currentColumn = 0; currentColumn < width; currentColumn++)
        { // writing out column
            // convert pixel value to binary format
            unsigned int pixelValue = convertEbu2Ebf(imageData[currentRow][currentColumn]);
            // run and check if the output was successful
            if (badOutput(fprintf(outputFile, "%u", pixelValue)))
            { // check write
                return BAD_OUTPUT;
            } // check write

            // if end of line has not been reached
            if ((currentColumn + 1) / width != 1)
            {
                // output whitespace to file
                // validate if output to file was sucessful
                if (badOutput(fprintf(outputFile, " ")))
                { // check write
                    return BAD_OUTPUT;
                } // check write
            }
            // else if end of line is reached, but end of row has not been reached
            else if ((currentColumn + 1) / width == 1 && currentRow != height - 1)
            {
                // output newline to file
                // validate if output to file was sucessful
                if (badOutput(fprintf(outputFile, "\n")))
                { // check write
                    return BAD_OUTPUT;
                } // check write
            }
        }
    }

    return 0;
}



// outputs all data to a file, where the pixel values are in binary
// returns error code if output has failed, 0 if successful
int outputFileDataBinary(ebuData *data, FILE *outputFile)
{
    // output header to file and validate for success (0 means success)
    if (outputHeader(data->magicNumber, data->height, data->width, outputFile) != 0)
    {
        return BAD_OUTPUT;
    }
    // output image data to file and validate for success (0 means success)
    else if (outputImageDataBinary(data->dataBlock, data->numBytes, outputFile) != 0)
    {
        return BAD_OUTPUT;
    }

    return 0;
}

// outputs the binary pixel data directly to a file
// returns error code if output has failed, 0 if successful
int outputImageDataBinary(BYTE *dataBlock, long numBytes, FILE *outputFile)
{
    // write the entire dataBlock to the file and validate for success
    if (badOutput(fwrite(dataBlock, numBytes, 1, outputFile)))
    {
        return 1;
    }

    return 0;
}


// function takes all data for ebu file and outputs it to ebc file
// returns respective error codes if failed, 0 if otherwise
int outputFileDataEbuDirectEbc(ebuData *data, FILE *outputFile)
{
    // define the header that needs to be outputted to the file
    unsigned char *header = (unsigned char *) "ec";
    // output header to file and validate for success (0 means success)
    if (outputHeader(header, data->height, data->width, outputFile) != 0)
    {
        return BAD_OUTPUT;
    }
    // output image data to file and validate for success (0 means success)
    int errCode = outputImageDataEbuDirectEbc(data, outputFile);
    if (errCode != 0)
    {
        return errCode;
    }

    return 0;
}

// outputs image data from binary format to compressed binary format
// returns error code if output has failed, 0 if successful
int outputImageDataEbuDirectEbc(ebuData *data, FILE *outputFile)
{
    // calculate size and allocate memory for an array that will store the compressed binary format once converted
    int sizeOfCompressedBinaryArray;
    
    // if there is a remainder from numBytes, there is an extra byte needs to be allocated to store infomation
    if (fmod(data->numBytes, COMPRESSION_FACTOR) != 0.0)
    {
        sizeOfCompressedBinaryArray = (((data->numBytes) * (COMPRESSION_FACTOR)) + 1) * sizeof(BYTE);
    }
    else
    {
        sizeOfCompressedBinaryArray = ((data->numBytes) * (COMPRESSION_FACTOR)) * sizeof(BYTE);
    }

    // allocate array for storing compressed bytes
    BYTE *compressedBinaryArray = (BYTE *) malloc(sizeOfCompressedBinaryArray);
    if (badMalloc(compressedBinaryArray))
    {
        return BAD_MALLOC;
    }

    // convert uncompressed binary format to compressed binary
    convertEbu2Ebc(data->dataBlock, compressedBinaryArray, data->numBytes);

    // write the entire dataBlock to the file
    if (badOutput(fwrite(compressedBinaryArray, sizeOfCompressedBinaryArray, 1, outputFile)))
    {
        return BAD_OUTPUT;
    }

    // free malloc'd compressed binary array
    free(compressedBinaryArray);

    return 0;
}


// outputs all data to a file, where the pixel values are in compressed binary
// returns error code if output has failed, 0 if successful
int outputFileDataCompressedBinary(ebcData *data, FILE *outputFile)
{
    // output header to file and validate for success (0 means success)
    
    // define the header that needs to be outputted to the file
    unsigned char *header = (unsigned char *) "ec";

    if (outputHeader(header, data->height, data->width, outputFile) != 0)
    {
        return BAD_OUTPUT;
    }
    // output image data to file and validate for success (0 means success)
    int errCode = outputImageDataBinary(data->dataBlockCompressed, data->numBytesCompressed, outputFile);
    if (errCode != 0)
    {
        return errCode;
    }

    return 0;
}


// function takes all data for ebc file and outputs it to ebu file
// returns respective error codes if failed, 0 if otherwise
int outputFileDataEbcDirectEbu(ebcData *data, FILE *outputFile)
{
    // define the header that needs to be outputted to the file
    unsigned char *header = (unsigned char *) "eu";
    // output header to file and validate for success (0 means success)
    if (outputHeader(header, data->height, data->width, outputFile) != 0)
    {
        return BAD_OUTPUT;
    }
    // output image data to file and validate for success (0 means success)
    int errCode = outputImageDataBinary(data->dataBlockUncompressed, data->numBytesUncompressed, outputFile);
    if (errCode != 0)
    {
        return errCode;
    }

    return 0;
}


// outputs all data to a file, where the pixel values are in compressed block binary
// returns error code if output has failed, 0 if successful
int outputFileDataCompressedBlockBinary(ebcBlockData *data, FILE *outputFile)
{
    // define the header that needs to be outputted to the file
    unsigned char *header = (unsigned char *) "EC";
    // output header to file and validate for success (0 means success)
    if (outputHeader(header, data->height, data->width, outputFile) != 0)
    {
        return BAD_OUTPUT;
    }
    // output image data to file and validate for success (0 means success)
    int errCode = outputImageDataBinary(data->blocksCompressed, data->numBlocksCompressed, outputFile);
    if (errCode != 0)
    {
        return errCode;
    }

    return 0;
}

// outputs all data to a file, where the pixel values are in compressed random block binary
// returns error code if output has failed, 0 if successful
int outputFileDataCompressedRandomBlockBinary(ebcRandomBlockData *data, FILE *outputFile)
{
    // define the header that needs to be outputted to the file
    unsigned char header[2];
    header[0] = 'E';
    header[1] = (unsigned char) data->numBitsCompressed;
    // output header to file and validate for success (0 means success)
    if (outputHeader(header, data->height, data->width, outputFile) != 0)
    {
        return BAD_OUTPUT;
    }
    // output image data to file and validate for success (0 means success)
    int errCode = outputImageDataBinary(data->blocksCompressed, data->numBlocksCompressed, outputFile);
    if (errCode != 0)
    {
        return errCode;
    }

    return 0;
}



