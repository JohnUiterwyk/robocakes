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

#ifndef INPUT_UTILITY_H
#define INPUT_UTILITY_H



/* Function prototypes. */
void readRestOfLine();
int getNonEmptyString(char* string, unsigned length, char * prompt);
int getStringAutoPrompt(char* string, unsigned length, char * contentType);
int getString(char* string, unsigned length, char* prompt);
void trimNewLine(char * string);
void stringToUpper(char * string);


#endif
