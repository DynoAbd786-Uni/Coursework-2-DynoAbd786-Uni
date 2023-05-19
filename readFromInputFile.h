// contains function prototypes for reading from an input file

/*      FUNCTIONS ESSENTIAL TO ALL FILES        */

int getMagicNumber(FILE *inputFile, unsigned char *data);

unsigned short *getMagicNumberValue(unsigned char *magicNumber);

int getDimensions(int *height, int *width, FILE *inputFile);


/*      FOR EBF FILES       */

int getFileData(ebfData *inputData, char *filename, FILE *inputFile);

int setImageDataArray(ebfData *data);

int getImageDataArray(ebfData *data, FILE *inputFile, char *filename);


/*      FOR EBU FILES       */

int getFileDataBinary(ebuData *inputData, char* filename, FILE *inputFile);

int setBinaryImageDataArrayEbu(ebuData *data);

int getBinaryImageDataArray(ebuData *data, FILE *inputFile, char *filename);


/*      FOR EBC FILES       */

int getFileDataCompressedBinary(ebcData *inputData, char* filename, FILE *inputFile);

int setCompressedBinaryImageDataArrayEbc(ebcData *data);

int getCompressedBinaryImageDataArray(ebcData *data, FILE *inputFile, char *filename);


/*      FOR EBCC FILES      */

int getFileDataCompressedBlockBinary(ebcBlockData *inputData, char* filename, FILE *inputFile);

int setEbcBlockData(ebcBlockData *data);

int getCompressedBlockImageDataArray(BYTE *compressedBlockArray, BYTE *uncompressedBlockArray,  long sizeOfCompressedBlockArray, long sizeOfUncompressedBlockArray, int numBitsToDecompressFrom, FILE *inputFile, char *filename);

int getFileDataCompressedRandomBlockBinary(ebcRandomBlockData *inputData, char* filename, int numParadigmBlocksUncompressed, FILE *inputFile);

int getCompressedParadigmBlocksDataArray(ebcRandomBlockData *data, FILE *inputFile, char *filename);

int setEbcRandomBlockData(ebcRandomBlockData *data, int numParadigmBlocks);