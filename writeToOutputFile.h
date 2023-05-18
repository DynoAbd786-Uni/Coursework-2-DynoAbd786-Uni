// contains function prototypes for anything to do with writing to the output file
// output header and image data functions take in individual variables rather than the whole struct to improve modularity (intentional design choice)

// used with all functions to output file header and dimensions
int outputHeader(unsigned char *magicNumber, int height, int width, FILE *outputFile);


// for ebf to ebf outputs
int outputFileData(ebfData *data, FILE *outputFile);

int outputImageData(unsigned int **imageData, int height, int width, FILE *outputFile);


// for ebf to ebu outputs
int outputFileDataEbfDirectEbu(ebfData *data, FILE *outputFile);

int outputImageDataEbfDirectEbu(unsigned int **imageData, int height, int width, FILE *outputFile);


// for ebu to ebf outputs
int outputFileDataEbuDirectEbf(ebuData *data, FILE *outputFile);

int outputImageDataEbuDirectEbf(BYTE **imageData, int height, int width, FILE *outputFile);


// for ebu to ebu outputs
int outputFileDataBinary(ebuData *data, FILE *outputFile);

int outputImageDataBinary(BYTE *dataBlock, long numBytes, FILE *outputFile);


// for ebu to ebc outputs
int outputFileDataEbuDirectEbc(ebuData *data, FILE *outputFile);

int outputImageDataEbuDirectEbc(ebuData *data, FILE *outputFile);


// for ebc to ebc outputs
int outputFileDataCompressedBinary(ebcData *data, FILE *outputFile);


// for ebc to ebu outputs
int outputFileDataEbcDirectEbu(ebcData *data, FILE *outputFile);

// for ebc to ebcBlock outputs
int outputFileDataCompressedBlockBinary(ebcBlockData *data, FILE *outputFile);

// for ebc to ebcRandomBlock outputs
int outputFileDataCompressedRandomBlockBinary(ebcRandomBlockData *data, FILE *outputFile);




