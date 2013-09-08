#include "LibUtil.h"

void *
safe_malloc (size_t size)
{
    void * ptr;
    ptr = malloc(size);
    if (ptr == NULL)
	{
        perror("malloc encountered an error");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void *
safe_realloc (void *old_mem, size_t new_size)
{
    void * new_mem;
    new_mem = realloc(old_mem, new_size);
    if(new_mem == NULL)
    {
        perror("realloc encountered an error.");
        exit(EXIT_FAILURE);
    }
  return new_mem;
}

char *
safe_strdup (const char *str)
{
    char * new_string;
    if(str == NULL)
    {
        perror("Error! Can not duplicate a NULL string");
        exit(EXIT_FAILURE);
    }
    new_string = safe_malloc(strlen(str)+1);
    strcpy(new_string, str);
  return new_string;
}

int
cmp_int (const void *a, const void *b)
{
    if(a==NULL || b==NULL)
    {
        perror("Error! Can not compare a NULL value");
        exit(EXIT_FAILURE);
    }
    if(*(const int *)a == *(const int *)b) return (0);
    if(*(const int *)a < *(const int *)b) return (-1);
    if(*(const int *)a > *(const int *)b) return (1);
    return (-1);
}


int
string_to_int(char * string, int * result)
{
    int tempInt;
    char* endPtr;
    if(string == NULL || string[0] == '\n' || result == NULL)
    {
        fprintf(stderr,"ERROR string_to_int: Input invalid");
        return FALSE;
    }
    
    trimNewLine(string);
    
    /* Convert string to an integer. */
    tempInt = (int) strtol(string, &endPtr, 10);
    
    /* Validate integer result. */
    if (strcmp(endPtr, "") != 0)
    {
        fprintf(stderr,"ERROR string_to_int: Input was not numeric.\n");
        return FALSE;
    }
    *result = tempInt;
    return TRUE;
}

void trimNewLine(char * string)
{
    size_t length;
    length = strlen(string);
    if(length > 0 && string[length-1] == '\n' ) string[length-1] = 0;
}


FILE* safe_open_file(char * filename)
{
    FILE * filePtr;
	if ((filePtr = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "Error: Could not open file %s.\n", filename);
        exit(EXIT_FAILURE);
    }
    return filePtr;
}

int *
alloc_int (int val)
{
    int *rv = safe_malloc (sizeof (int));
    *rv = val;
    return (rv);
}

void
delete_int (void *val)
{
    if (val) {
        int *tmp = (int *)val;
        free (tmp);
    }
}



