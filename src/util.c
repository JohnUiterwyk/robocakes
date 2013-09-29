#include "util.h"

void
print_usage()
{
  /* TODO Calculate binary name dynamically */
  printf("USAGE: robocakes [options]\n");
  printf("  -f show frames per second\n");
  printf("  -c configuration file\n");
  printf("  -h display usage information\n");
}

bool
string_array_contains(const char *const* haystack, const char *needle)
{
  assert(haystack != NULL);
  assert(needle != NULL);

  for (; *haystack != NULL; ++haystack) {
    if (strcasecmp(*haystack, needle) == 0)
      return true;
  }
  return false;
}

char *
strip_leading_whitespace(char * string)
{
  char *start;
  assert(string != NULL);

  for (start = string; *start && isspace(*start); start++)
    ;

  memmove(string, start, strlen(start) + 1);
  return string;
}

void *
safe_calloc(int count, int size)
{
  return calloc(count, size);
}


/***************************************************************************
 * Following functions:
 *
 * read_rest_of_line
 * get_non_empty_string
 * get_string_from_stdin
 *
 * were adapted from sample code provided by Paul Miller,and Xiaodong Li,
 * as part of RMIT Programming Techniques SCOSC1283/1284
 * Semester 1 2013 Assignment #2
 *
 **************************************************************************/

/****************************************************************************
 * Function read_rest_of_line() is used for buffer clearing.
 ****************************************************************************/
void read_rest_of_line()
{
    int c;
    
    /* Read until the end of the line or end-of-file. */
    while ((c = fgetc(stdin)) != '\n' && c != EOF)
        ;
    
    /* Clear the error and end-of-file flags. */
    clearerr(stdin);
}

/****************************************************************************
 * Function get_string_auto_prompt() is used to get a string from the user
 * with a given prompt. The function returns success only if the string
 * is not an empty string
 *
 ****************************************************************************/
int get_string_auto_prompt(char* string, unsigned length, char * prompt)
{
    get_string_from_stdin(string, length, prompt);
    if(string[0] != '\0')
    {
        return EXIT_SUCCESS;
    }else
    {
        return EXIT_FAILURE;
    }
}


/****************************************************************************
 * Function get_string_from_stdin() is an interactive string input function 
 * with dynamic memory.
 ****************************************************************************/
int get_string_from_stdin(char* string, unsigned length, char* prompt)
{
    int finished = FALSE;
    char* tempString;
    
    /* Allocate temporary memory. */
    if ((tempString = malloc(sizeof(char) * (length+2))) == NULL)
    {
        fprintf(stderr, "Fatal error: malloc() failed in getString().\n");
        exit(EXIT_FAILURE);
    }
    
    /* Continue to interact with the user until the input is valid. */
    do
    {
        /* Provide a custom prompt. */
        printf("%s", prompt);
        
        /* Accept input. "+2" is for the \n and \0 characters. */
        fgets(tempString, length + 2, stdin);
        
        /* A string that doesn't have a newline character is too long. */
        if (tempString[strlen(tempString) - 1] != '\n')
        {
            printf("Input was too long.\n");
            read_rest_of_line();
        }
        else
        {
            finished = TRUE;
        }
        
    } while (finished == FALSE);
    
    /* Overwrite the \n character with \0. */
    tempString[strlen(tempString) - 1] = '\0';
    
    /* Make the result string available to calling function. */
    strcpy(string, tempString);
    
    /* Deallocate temporary memory. */
    free(tempString);
    
    return TRUE;
}
