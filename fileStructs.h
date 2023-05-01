// Struct types for all files
#include <stdint.h>

// assigns type uint8_t to type BYTE for better readability
typedef uint8_t BYTE;

// struct for storing data from an ebf file
typedef struct 
{
    unsigned char magicNumber[2];
    int width, height;
    long numBytes;
    unsigned int **imageData;
    unsigned int *dataBlock;
} 
ebfData;

// struct for storing data from an ebu file
typedef struct
{
    unsigned char magicNumber[2];
    int width, height;
    long numBytes;
    BYTE **imageData;
    BYTE *dataBlock;
}
ebuData;

// struct for storing data from an ebc file
typedef struct
{
    unsigned char magicNumber[2];
    int width, height;
    long numBytesUncompressed;
    long numBytesCompressed;
    BYTE *dataBlockCompressed;
    BYTE *dataBlockUncompressed;
}
ebcData;


// struct for storing data from an ebcc file
typedef struct
{
    unsigned char magicNumber[2];
    int width, height;
    long numBlocksUncompressed;
    long numBlocksCompressed;
    BYTE *blocksCompressed;
    BYTE *blocksUncompressed;
    BYTE **imageDataUncompressed;
    BYTE *dataBlockUncompressed;
}
ebcBlockData;



