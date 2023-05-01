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

int setEbcBlockData(ebcBlockData *data);