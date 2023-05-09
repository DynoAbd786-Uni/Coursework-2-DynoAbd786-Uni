#include <math.h>

#include "fileStructs.h"
#include "blockHandling.h"

// takes an image and calcualtes the 3x3 pixel blocks respective to that image 
void calculateBlockData(ebcBlockData *data)
{
    int pixelTrackerRow = 0;
    int pixelTrackerColumn = 0;
    int currentPixelRow = 0;
    int currentPixelColumn = 0;
    int blockTracker = 0;

    int totalPixelValues = 0;
    int pixelCount = 0;
    float averageValueForBlock = 0;

    for (int currentBlockRow = 0; currentBlockRow < data->noBlocksByHeight; currentBlockRow++)
    {
        for (int currentBlockColumn = 0; currentBlockColumn < data->noBlocksByWidth; currentBlockColumn++)
        {
            currentPixelRow = currentBlockRow * 3;
            currentPixelColumn = currentBlockColumn * 3;

            for (int pixelInBlockRow = 0; pixelInBlockRow < MAX_BLOCK_SIZE; pixelInBlockRow++)
            {
                pixelTrackerRow = pixelInBlockRow + currentPixelRow;

                if (pixelTrackerRow >= data->height)
                {
                    break;
                }

                for (int pixelInBlockColumn = 0; pixelInBlockColumn < MAX_BLOCK_SIZE; pixelInBlockColumn++)
                {
                    pixelTrackerColumn = pixelInBlockColumn + currentPixelColumn;

                    if (pixelTrackerColumn >= data->width)
                    {
                        break;
                    }

                    totalPixelValues = totalPixelValues + data->imageDataUncompressed[pixelTrackerRow][pixelTrackerColumn];
                    pixelCount++;
                }
            }

            averageValueForBlock = (int) round((float) totalPixelValues / (float) pixelCount);
            data->blocksUncompressed[blockTracker] = (BYTE) averageValueForBlock;
            blockTracker++;
        }
    }
}