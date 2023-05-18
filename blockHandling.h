// maximum value of pixel value is 32, middle is 16

#define MIDDLE_VALUE_OF_PIXEL 15

void setBlocksAsAverageValue(ebcBlockData *data);

void calculateBlockData(ebcBlockData *data);

void calculateImageData(ebcBlockData *data);

int calculateNoParadigmBlocks(char *programName);

int randomiseBlockData(ebcBlockData *dataConversionHolder, ebcRandomBlockData *outputData, int seed);

block getBlockImage(BYTE **image, block blocks);