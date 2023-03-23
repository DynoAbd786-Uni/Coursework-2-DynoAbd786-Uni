// Header file for all global memory related variables

// defines the maximum amount of files that can be compared by any Comp file
#define MAX_FILE_COMPARISON 2

/*      EBF RELATED FUNCTIONS       */

ebfData *mallocEbf();

void freeEbfData(ebfData *data);

void freeEbfDataArray(ebfData *dataToCompare[]);

void freeEbfReadDataArrays(char *input, unsigned int *inputIntArray);


/*      EBU RELATED FUNCTIONS       */

ebuData *mallocEbu();

void freeEbuData(ebuData *data);

void freeEbuDataArray(ebuData *dataToCompare[]);
