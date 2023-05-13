#include <stdio.h>
#include <string.h>

#include "fileStructs.h"
#include "errorChecking.h"

// all functions within this file return 1 for any error detected, 0 otherwise

/*      NO ARGUEMENTS       */
// Provide the user with correct usage if no arguements are provided
// Returns 1 if no arguements are provided
int noArguements(int argc, char **argv)
{
    if (argc == 1)
    {
        // obtaining the executable name without the ./ at the beginning. 
        // this was adapted from a query asked to ChatGPT, asking "how to i take out the ./ from ./ebfComp"
        // overwrites the beginning of the string using memmove, omitting the "./"
        char *executableName = argv[0];
        int length = strlen(executableName);
        memmove(executableName, executableName + 2, length - 1);

        // output error message with respected executable name
        printf("Usage: %s file1 file2\n", executableName);
        return 1;
    }

    return 0;
}


/*      BAD ARGUEMENTS      */
// validate that user has enter 2 arguments (plus the executable name)
// Returns 1 if the wrong amount of arguements are provided
int badArguements(int argc)
{
    // check arg count
    if (argc != 3)
    { 
        printf("ERROR: Bad Arguments\n");
        return 1;
    } 

    return 0;
}

// error code for the random block compression programs
// validate that user has enter 2 arguments (plus the executable name)
// Returns 1 if the wrong amount of arguements are provided
int badArguementsRandom(int argc)
{
    // check arg count
    if (argc != 4)
    { 
        printf("ERROR: Bad Arguments\n");
        return 1;
    } 

    return 0;
}


/*      BAD FILE      */
// check file opened successfully
// returns 1 if bad file is given
int badFile(FILE *file, char *filename)
{
    // check file pointer
    if (!file)
    {
        printf("ERROR: Bad File Name (%s)\n", filename);
        return 1;
    }

    return 0;
}


/*      BAD MAGIC NUMBER        */

// checking against known ebf magic number
// returns 1 if magic number doesnt match known value
int badMagicNumberEbf(unsigned short *magicNumberValue, char *filename)
{
    // check magic number
    if (*magicNumberValue != MAGIC_NUMBER_EBF)
    {
        printf("ERROR: Bad Magic Number (%s)\n", filename);
        return 1;
    }

    return 0;
}

// checking against known ebu magic number
// returns 1 if magic number doesnt match known value
int badMagicNumberEbu(unsigned short *magicNumberValue, char *filename)
{
    // check magic number
    if (*magicNumberValue != MAGIC_NUMBER_EBU)
    {
        printf("ERROR: Bad Magic Number (%s)\n", filename);
        return 1;
    }

    return 0;
}

// checking against known ebc magic number
// returns 1 if magic number doesnt match known value
int badMagicNumberEbc(unsigned short *magicNumberValue, char *filename)
{
    // check magic number
    if (*magicNumberValue != MAGIC_NUMBER_EBC)
    {
        printf("ERROR: Bad Magic Number (%s)\n", filename);
        return 1;
    }

    return 0;
}

// checking against known ebc magic number
// returns 1 if magic number doesnt match known value
int badMagicNumberEbcBlock(unsigned short *magicNumberValue, char *filename)
{
    // check magic number
    if (*magicNumberValue != MAGIC_NUMBER_EBC_BLOCK)
    {
        printf("ERROR: Bad Magic Number (%s)\n", filename);
        return 1;
    }

    return 0;
}


/*      BAD DIMENSIONS      */
// check dimensions to see if 2 values have been captured, and if dimensions are within acceptable range
// returns 1 if requirements arent met
int badDimensions(int height, int width, int checkValue, char *filename)
{
    // check dimensions
    if (checkValue != 2 || height < MIN_DIMENSION || width < MIN_DIMENSION || height > MAX_DIMENSION || width > MAX_DIMENSION)
    {
        // print appropriate error message and return
        printf("ERROR: Bad Dimensions (%s)\n", filename);
        return 1;
    }

    return 0;
}


/*      BAD MALLOC      */
// checks if malloc has sucessfully allocated memory
// returns 1 if failed
int badMalloc(void *mallocData)
{
    // check malloc
    if (mallocData == NULL)
    { 
        printf("ERROR: Image Malloc Failed\n");
        return 1;
    }

    return 0;
}


/*      BAD DATA        */

// checks if end of file has been reached
// returns 1 if end of file has not been reached
int endOfFile(FILE *file, char *filename)
{
    // checking end of file
    if (feof(file))
    {
        printf("ERROR: Bad Data (%s)\n", filename);
        return 1;
    }

    return 0;
}

// checks if end of file has been reached
// returns 1 if end of file has not been reached
int notEndOfFile(FILE *file, char *filename)
{
    // checking for not end of file
    if (!feof(file))
    {
        printf("ERROR: Bad Data (%s)\n", filename);
        return 1;
    }

    return 0;
}

/*      SPECIALISED FOR EBF REALTED FILES       */

// check if pixel value is out of bounds
// returns 1 if pixel value is out of bounds
int badPixelValue(int pixel, char *filename)
{
    // checking pixel value
    if (pixel < MIN_PIXEL_VALUE || pixel > MAX_PIXEL_VALUE)
    {
        printf("ERROR: Bad Data (%s)\n", filename);
        return 1;
    }

    return 0;
}

// checks if length of array is the same as the width given in file
// returns 1 if array size does not match the stated width from the file
int wrongArraySize(int width, int arraySize, char *filename)
{
    // checking array size 
    if (arraySize != width)
    {
        printf("ERROR: Bad Data (%s)\n", filename);
        return 1;
    }

    return 0;
}

// checks if there are no more lines to read from the file (height given is lower than actual height of file)
// returns 1 if there are not enough lines in the file
int noMoreLines(void *array, char *filename)
{
    // chekcing for no more lines read
    if (array == NULL)
    {
        printf("ERROR: Bad Data (%s)\n", filename);
        return 1;
    }

    return 0;
}


/*      SPECIALISED FOR UNCOMPRESSED BINARY RELATED FILES        */
// checks if the correct number of bytes has been read
// returns 1 if no bytes has been read to the array
int badByteRead(int count, char *filename)
{
    if (!count)
    {
        printf("ERROR: Bad Data (%s)\n", filename);
        return 1;
    }

    return 0;
}

/*      SPECIALISED FOR COMPRESSED BINARY RELATED FILES        */
// compares the amount of decompressed bytes returned by the conversion algorithm against the expected numBytes
// returns 1 if the count does not match the expected numBytes
int badNumBytes(long count, long numBytesUncompressed, char *filename)
{
    if (count != numBytesUncompressed)
    {
        printf("ERROR: Bad Data (%s)\n", filename);
        return 1;
    }

    return 0;
}


/*      BAD OUTPUT     */
// checks to see if the data is being outputted correctly
// returns 1 if the fprintf statement has had an error occur, or if nothing has been printed to the file
int badOutput(int check)
{
    if (check == 0 || check == -1)
    {
        printf("ERROR: Bad Output\n");
        return 1;
    }

    return 0;
}


/*      MISCELLANEOUS ERRORS    */

/*      BAD FILE FORMAT     */
// functions in this header checks if the format of the input file is correct
// information derived from the pgm document provided by github README document
int noWhitespaceOrNull(char character)
{
    int asciiValue = (int) character;
    if (!(asciiValue == 10 || asciiValue == 9 || asciiValue == 32 || asciiValue == 0))
    {
        printf("ERROR: Miscellaneous (file format is incorrect (no whitespace char where there is meant to be a whitespace char))");
        return 1;
    } 

    return 0;
}




