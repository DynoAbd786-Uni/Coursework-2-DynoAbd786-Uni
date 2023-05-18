#define COMPRESSION_FACTOR (5.0 / 8.0)
#define COMPRESSION_FACTOR_R128 (7.0 / 8.0)
#define MAX_BITS_IN_BYTE 8
#define MAX_BITS_IN_UNCOMPRESSED_BYTE 5     // defines the max amount of bits to read from duing compression from ebu to ebc, since MAX_PIXEL_VALUE = 31 = 2^5 bytes needed


// contains all header files for converting data between data types ebf, ebu, ebc, and ebcc

BYTE convertEbf2Ebu(unsigned int pixelValue);

unsigned int convertEbu2Ebf(BYTE pixelValueBinary);

void convertEbu2Ebc(BYTE *uncompressedPixelValues, BYTE *compressedBinaryArray, int numBytesUncompressed);

long convertEbc2Ebu(BYTE *compressedPixelValues, BYTE *uncompressedBinaryArray, int numBytesCompressed, int numBytesUncompressed);

void compressDataToGivenBit(BYTE *uncompressedPixelValues, BYTE *compressedBinaryArray, int numBytesUncompressed, int numBitsToCompressTo);

long decompressDataFromGivenBit(BYTE *compressedPixelValues, BYTE *uncompressedBinaryArray, int numBytesCompressed, int numBytesUncompressed, int numBitsToDecompressFrom);

