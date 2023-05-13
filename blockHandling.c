#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileStructs.h"
#include "blockHandling.h"


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
                        break;
                    }

                    // add the value of the pixel to the total, and increment number of pixels by 1
                    totalPixelValues = totalPixelValues + data->imageDataUncompressed[pixelTrackerRow][pixelTrackerColumn];
                    pixelCount++;
                }
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

// void calculateAveragePerBlock(ebcBlockData *data)
// {
    
// }


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

// // picks random blocks from struct ebcBlockData and stores it to ebcRandomBlockData
// void randomiseBlockData(ebcBlockData dataConversionHolder, ebcRandomBlockData outputData, int seed)
// {
//     for (int paradigmBlockNo = 0; paradigmBlockNo < outputData->numParadigmBlocksUncompressed; )
//     {

//     }
// }