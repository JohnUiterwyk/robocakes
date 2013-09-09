/***************************************************************************
 * COSC1283/1284 - Programming Techniques
 * Semester 1 2013 Assignment #2 - SCSIT Program Management System
 * Full Name        : JOHN UITERWYK
 * Student Number   : s3406031
 * Yallara Username : s3406031
 * Course Code      : COSC1284
 * Program Code     : BP094
 * Start up code provided by Paul Miller,and Xiaodong Li
 **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

/* Function prototypes. */
void readRestOfLine();
int getNonEmptyString(char* string, unsigned length, char * prompt);
int getStringAutoPrompt(char* string, unsigned length, char * contentType);
int getString(char* string, unsigned length, char* prompt);
void trimNewLine(char * string);
void stringToUpper(char * string);

/****************************************************************************
* Function readRestOfLine() is used for buffer clearing. 
* Please refer to "test_fgets.c" on Blackboard: 
* "Course Documents"->"Function Examples"->"Input Validation Examples" 
****************************************************************************/
void readRestOfLine()
{
   int c;

   /* Read until the end of the line or end-of-file. */   
   while ((c = fgetc(stdin)) != '\n' && c != EOF)
      ;

   /* Clear the error and end-of-file flags. */
   clearerr(stdin);
}

/****************************************************************************
* Function getNonEmptyString() is used to get a string from the user
* with a given prompt. The function returns success only if the string
* is not an empty string
*
****************************************************************************/
int getNonEmptyString(char* string, unsigned length, char * prompt)
{
  getString(string, length, prompt);
  if(string[0] != '\0')
  {
    return EXIT_SUCCESS;
  }else
  {
    return EXIT_FAILURE;
  }
}


/****************************************************************************
* Function getString() is used to read in a string. 
* Please refer to "getString-advanced.c" on Blackboard: 
* "Course Documents"->"Function Examples"->"Input Validation Examples" 
*****************************************************************************
* getString(): An interactive string input function with dynamic memory.
* This function prompts the user for a string using a custom prompt. A line
* of text is accepted from the user using fgets() and stored in a temporary
* string. When the function detects that the user has entered too much text,
* an error message is given and the user is forced to reenter the input. The
* function also clears the extra text (if any) with the readRestOfLine()
* function.
* When a valid string has been accepted, the unnecessary newline character
* at the end of the string is overwritten. Finally, the temporary string is
* copied to the string variable that is returned to the calling function.
****************************************************************************/
int getString(char* string, unsigned length, char* prompt)
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
         readRestOfLine();
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



