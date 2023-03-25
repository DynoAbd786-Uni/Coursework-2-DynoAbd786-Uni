#define MAX_BITS_IN_BYTE 8
#define MAX_BITS_IN_UNCOMPRESSED_BYTE 5 // defines the max amount of bits to read from duing compression from ebu to ebc, since MAX_PIXEL_VALUE = 31 = 2^5 bytes needed



// contains all header files for converting data between data types ebf, ebu, and ebc

BYTE convertEbf2Ebu(unsigned int pixelValue);

unsigned int convertEbu2Ebf(BYTE pixelValueBinary);

BYTE *convertEbu2Ebc(BYTE *uncompressedPixelValues, BYTE *compressedBinaryArray, int numBytes);

int convertEbc2Ebu(ebcData data);

