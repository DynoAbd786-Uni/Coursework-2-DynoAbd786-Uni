#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileStructs.h"
#include "blockHandling.h"
#include "errorChecking.h"


// takes the average value of block and sets the block data as that value
void setBlocksAsAverageValue(ebcBlockData *data)
{
    for (int blockNo = 0; blockNo < data->numBlocksUncompressed; blockNo++)
    {
        data->blocksUncompressed[blockNo] = data->blocksInImage[blockNo].averageValueOfBlock;
    }
}

// takes an image and calcualtes the 3x3 pixel blocks respective to that image 
void calculateBlockData(ebcBlockData *data)
{
    // variables for calculating position of top right and current pixels in block, as well as block number
    int pixelTrackerRow = 0;
    int pixelTrackerColumn = 0;
    int blockTracker = 0;

    // variables used for calculating block average
    int totalPixelValues = 0;
    int pixelCount = 0;
    int averageValueForBlock = 0;

    // looping through the rows and columns of the expected blocks
    for (int currentBlockRow = 0; currentBlockRow < data->noBlocksByHeight; currentBlockRow++)
    {
        for (int currentBlockColumn = 0; currentBlockColumn < data->noBlocksByWidth; currentBlockColumn++)
        {
            // calculate position of top left block according to the block position and store to struct
            data->blocksInImage[blockTracker].rowNum = currentBlockRow * 3;
            data->blocksInImage[blockTracker].columnNum = currentBlockColumn * 3;

            // reset values for calculating average of the block
            totalPixelValues = 0;
            pixelCount = 0;

            // looping inside the block row
            for (int pixelInBlockRow = 0; pixelInBlockRow < MAX_BLOCK_SIZE; pixelInBlockRow++)
            {
                // calculate position inside pixel respective to the image row
                pixelTrackerRow = pixelInBlockRow + data->blocksInImage[blockTracker].rowNum;
                
                // if the position exceeds the limits of the image, break
                if (pixelTrackerRow >= data->height)
                {
                    // set row size of block
                    data->blocksInImage[blockTracker].rowSize = pixelInBlockRow + 1;
                    break;
                }

                // looping inside the block column
                for (int pixelInBlockColumn = 0; pixelInBlockColumn < MAX_BLOCK_SIZE; pixelInBlockColumn++)
                {
                    // calculate position inside pixel respective to the image column
                    pixelTrackerColumn = pixelInBlockColumn + data->blocksInImage[blockTracker].columnNum;

                    // if the position exceeds the limits of the image, break
                    if (pixelTrackerColumn >= data->width)
                    {
                        // set column size of block
                        data->blocksInImage[blockTracker].columnSize = pixelInBlockColumn + 1;
                        break;
                    }

                    // add the value of the pixel to the total, and increment number of pixels by 1
                    totalPixelValues = totalPixelValues + data->imageDataUncompressed[pixelTrackerRow][pixelTrackerColumn];
                    pixelCount++;
                    
                    // set column size of block
                    data->blocksInImage[blockTracker].columnSize = pixelInBlockColumn + 1;
                }
                
                // set row size of block  
                data->blocksInImage[blockTracker].rowSize = pixelInBlockRow + 1;
            }

            // store total value and number of pixels to struct
            data->blocksInImage[blockTracker].numPixelsInBlock = pixelCount;
            data->blocksInImage[blockTracker].totalValueOfBlock = totalPixelValues;

            // calculate average, round, and store to respective block
            averageValueForBlock = (int) round((float) totalPixelValues / (float) pixelCount);
            data->blocksInImage[blockTracker].averageValueOfBlock = averageValueForBlock;

            // move to next block
            blockTracker++;
            
        }
    }
}



// decompresses the image from the ebcBlock data type
void calculateImageData(ebcBlockData *data)
{
    // variables for calculating position of top right and current pixels in block, as well as block number
    int pixelTrackerRow = 0;
    int pixelTrackerColumn = 0;
    int currentPixelRow = 0;
    int currentPixelColumn = 0;
    int blockTracker = 0;

    // looping through the rows and columns of the expected blocks
    for (int currentBlockRow = 0; currentBlockRow < data->noBlocksByHeight; currentBlockRow++)
    {
        for (int currentBlockColumn = 0; currentBlockColumn < data->noBlocksByWidth; currentBlockColumn++)
        {
            // calculate position of top right block according to the block position
            currentPixelRow = currentBlockRow * 3;
            currentPixelColumn = currentBlockColumn * 3;

            // looping inside the block row
            for (int pixelInBlockRow = 0; pixelInBlockRow < MAX_BLOCK_SIZE; pixelInBlockRow++)
            {
                // calculate position inside pixel respective to the image row
                pixelTrackerRow = pixelInBlockRow + currentPixelRow;
                
                // if the position exceeds the limits of the image, break
                if (pixelTrackerRow >= data->height)
                {
                    break;
                }

                // looping inside the block column
                for (int pixelInBlockColumn = 0; pixelInBlockColumn < MAX_BLOCK_SIZE; pixelInBlockColumn++)
                {
                    // calculate position inside pixel respective to the image column
                    pixelTrackerColumn = pixelInBlockColumn + currentPixelColumn;

                    // if the position exceeds the limits of the image, break
                    if (pixelTrackerColumn >= data->width)
                    {
                        break;
                    }

                    // output the value of the block to the imageDataUncompressed array
                    // repeats to fill in the MAX_BLOCK_SIZE x MAX_BLOCK_SIZE block
                    data->imageDataUncompressed[pixelTrackerRow][pixelTrackerColumn] = data->blocksUncompressed[blockTracker];
                }
            }
            
            // move to next block
            blockTracker++;
        }
    }
}

// finds how many paradigm blocks are required according to the program name
int calculateNoParadigmBlocks(char *programName)
{
    // finding the number within the executable name
    int length = strlen(programName);
    memmove(programName, programName + 6, length - 5);

    // convert number to int and return
    int noParadigmBlocks = atoi(programName);
    return noParadigmBlocks;
}

// picks random blocks from struct ebcBlockData and stores it to ebcRandomBlockData
int randomiseBlockData(ebcBlockData *dataConversionHolder, ebcRandomBlockData *outputData, int seed)
{
    // if the amount of blocks in the image is less than the programs required blocks, print out message
    if (dataConversionHolder->numBlocksUncompressed < outputData->numParadigmBlocksUncompressed)
    {
        outputData->numParadigmBlocksUncompressed = dataConversionHolder->numBlocksUncompressed;
        printf("Do be advised that running this program may result in a file larger in size than the input file. The program will still run, but for storage reasons you are better off just using the normal .ebc file\n");
    }
    // set up an array of indexes to block numbers so that it can be cross referenced for repeated index values
    int *blockIndexStorage = (int *) malloc(outputData->numParadigmBlocksUncompressed * sizeof(BYTE));

    // if malloc is unsuccessful, it will return a null pointer
    if (badMalloc(blockIndexStorage))
    { // check malloc
        return BAD_MALLOC;
    } // check malloc

    // set up the seed for the randomiser
    srand(seed);

    // initialise random block variable 
    int randomBlock = 0;

    // set variables to keep track of amount of blocks selected  
    int totalParadigmBlocksSelected = 0;

    int continueFlag = 0;

    // repeat until number of blocks selected matches the number of paradigm blocks required
    while (totalParadigmBlocksSelected < outputData->numParadigmBlocksUncompressed)
    {
        // pick a random block using rand
        randomBlock = rand() % dataConversionHolder->numBlocksUncompressed;

        // loop through the array and check if the randomised block has been selected
        for (int blockNumber = 0; blockNumber < totalParadigmBlocksSelected; blockNumber++)
        {
            // if there is an identical block, run the randomiser again
            if (blockIndexStorage[blockNumber] == randomBlock)
            {
                continueFlag = 1;
                break;
            }
            else
            {
                // checking for the unlikely case of there being 2 same blocks, but from different index points
                int pixelCounter = 0;

                for (int rowNum = 0; rowNum < MAX_BLOCK_SIZE; rowNum++)
                {
                    for (int columnNum = 0; columnNum < MAX_BLOCK_SIZE; columnNum++)
                    {
                        // save block of image data
                        outputData->uncompressedParadigmBlocks[(totalParadigmBlocksSelected * MAX_BLOCK_SIZE * MAX_BLOCK_SIZE) + (rowNum + columnNum)] = dataConversionHolder->blocksInImage[randomBlock].blockImage[rowNum][columnNum];
                    }
                }
            }
        }

        if (continueFlag)
        {
            continue
        }

        // save the index of the block
        blockIndexStorage[totalParadigmBlocksSelected] = randomBlock;
        
        // grab 3x3 image
        getBlockImage(dataConversionHolder->imageDataUncompressed, dataConversionHolder->blocksInImage[randomBlock]);

        for (int rowNum = 0; rowNum < MAX_BLOCK_SIZE; rowNum++)
        {
            for (int columnNum = 0; columnNum < MAX_BLOCK_SIZE; columnNum++)
            {
                // save block of image data
                outputData->uncompressedParadigmBlocks[(totalParadigmBlocksSelected * MAX_BLOCK_SIZE * MAX_BLOCK_SIZE) + (rowNum + columnNum)] = dataConversionHolder->blocksInImage[randomBlock].blockImage[rowNum][columnNum];
            }
        }
        
        // increment counter
        totalParadigmBlocksSelected++;
    }

    // free the temporary array
    free(blockIndexStorage);
    // exit function with success
    return 0;
}

// grabs a 3x3 image according to the position of the indexed block's row and column, and writes it to the block

void getBlockImage(BYTE **image, block blocks)
{
    // set up block tracker
    int blockTracker = 0;

    // loop through block dimensions
    for (int rowInBlock = 0; rowInBlock < MAX_BLOCK_SIZE; rowInBlock++)
    {
        for (int columnInBlock = 0; columnInBlock < MAX_BLOCK_SIZE; columnInBlock++)
        {
            // write the value of the pixel in the image to the respective block position
            blocks.blockImage[rowInBlock][columnInBlock] = image[blocks.rowNum + rowInBlock][blocks.columnNum + columnInBlock];

            // check if pixel value is within block limits. if not, use average to set pixel to while or black value
            if (rowInBlock >= blocks.rowSize || columnInBlock + 1 >= blocks.columnSize)
            {
                if (blocks.averageValueOfBlock < MIDDLE_VALUE_OF_PIXEL)
                {
                    blocks.blockImage[rowInBlock][columnInBlock] = MIN_PIXEL_VALUE;
                }
                else
                {
                    blocks.blockImage[rowInBlock][columnInBlock] = MAX_PIXEL_VALUE;
                }
            }

            // set block image to the pixel value at the image dimensions
            blocks.dataBlock[blockTracker] = image[blocks.rowNum + rowInBlock][blocks.columnNum + columnInBlock];

            // increment block counter
            blockTracker++;
        }
    }
}
