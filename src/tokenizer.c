#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_
#include "tokenizer.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>


static inline bool
valid_word_first_char(char ch)
{
  return isalpha(ch);
}

static inline bool
valid_word_char(char ch)
{
  return isalnum(ch) || ch == '_';
}

static inline bool
valid_numeral_char(char ch)
{
  return isdigit((int)ch) || ch == ',';
}

static inline bool
valid_numeral_first_char(char ch)
{
  return isdigit((int)ch);
}

char *
tokenizer_next_word(char **input_p)
{
	char *word, *input;

	assert(input_p != NULL);
	assert(*input_p != NULL);

	word = input = *input_p;

	if (*input == 0)
		return NULL;

	/* check the first character */

	if (!valid_word_first_char(*input)) {
    fprintf(stderr, "Letter expected\n");
		return NULL;
	}

	/* now iterate over the other characters until we find a
	   whitespace or end-of-string */

	while (*++input != 0) {
		if (isspace(*input)) {
			/* a whitespace: the word ends here */
			*input = 0;
			/* skip all following spaces, too */
			input = strip_leading_whitespace(input + 1);
			break;
		}

		if (!valid_word_char(*input)) {
			*input_p = input;
      fprintf(stderr, "Invalid word character\n");
			return NULL;
		}
	}

	/* end of string: the string is already null-terminated
	   here */

	*input_p = input;
	return word;
}

char *
tokenizer_next_udp_message(char **input_p)
{
  char *word, *input;

  assert(input_p != NULL);
  assert(*input_p != NULL);

  word = input = *input_p;

  if (*input == 0)
    return NULL;

  if (!valid_numeral_first_char(*input)) {
    fprintf(stderr, "Number expected\n");
    return NULL;
  }

  /* Now iterate over the other characters until we find a semicolon or
   * end-of-string */
  while (*++input != 0) {
    if (*input == ';') {
      /* A semi-colon. The block ends here. */
      *input = 0;
      /* skip all the following spaces, too */
      input = strip_leading_whitespace(input + 1);
      break;
    }

    if (!valid_numeral_char(*input)) {
      fprintf(stderr, "Invalid character\n");
      return NULL;
    }
  }
  /* End of string: the string is already null-terminated here */
  *input_p = input;
  return word;
}

int
tokenizer_next_udp_number(char **input_p)
{
  int number;
  char *word, *input;

  assert(input_p != NULL);
  assert(*input_p != NULL);

  word = input = *input_p;

  if (*input == 0)
    return -1;

  if (!valid_numeral_first_char(*input)) {
    fprintf(stderr, "Number expected\n");
    return -1;
  }

  /* Now iterate over the other characters until we find a semicolon or
   * comma */
  while (*++input != 0) {
    if (*input == ';' || *input == ',') {
      /* A semi-colon or comma. The number ends here. */
      *input = 0;
      /* skip all the following spaces, too */
      input = strip_leading_whitespace(input + 1);
      break;
    }

    if (!valid_numeral_first_char(*input)) {
      fprintf(stderr, "Invalid character\n");
      return -1;
    }
  }
  /* End of string: the string is already null-terminated here */
  number = (int)strtol(*input_p, &input, 10);
  *input_p = input;
  return number;
}

static inline bool
valid_unquoted_char(char ch)
{
	return (unsigned char)ch > 0x20 && ch != '"' && ch != '\'';
}

char *
tokenizer_next_unquoted(char **input_p)
{
	char *word, *input;

	assert(input_p != NULL);
	assert(*input_p != NULL);

	word = input = *input_p;

	if (*input == 0)
		return NULL;

	/* check the first character */

	if (!valid_unquoted_char(*input)) {
    fprintf(stderr, "Invalid unquoted character\n");
		return NULL;
	}

	/* now iterate over the other characters until we find a
	   whitespace or end-of-string */

	while (*++input != 0) {
		if (isspace(*input)) {
			/* a whitespace: the word ends here */
			*input = 0;
			/* skip all following spaces, too */
			input = strip_leading_whitespace(input + 1);
			break;
		}

		if (!valid_unquoted_char(*input)) {
			*input_p = input;
      fprintf(stderr, "Invalid unquoted character\n");
			return NULL;
		}
	}

	/* end of string: the string is already null-terminated
	   here */

	*input_p = input;
	return word;
}

char *
tokenizer_next_string(char **input_p)
{
	char *word, *dest, *input;

	assert(input_p != NULL);
	assert(*input_p != NULL);

	word = dest = input = *input_p;

	if (*input == 0)
		/* end of line */
		return NULL;

	/* check for the opening " */

	if (*input != '"') {
    fprintf(stderr, "'\"' expected.\n");
		return NULL;
	}

	++input;

	/* copy all characters */

	while (*input != '"') {
		if (*input == '\\')
			/* the backslash escapes the following
			   character */
			++input;

		if (*input == 0) {
			/* return input-1 so the caller can see the
			   difference between "end of line" and
			   "error" */
			*input_p = input - 1;
      fprintf(stderr, "Missing closing '\"'\n");
			return NULL;
		}

		/* copy one character */
		*dest++ = *input++;
	}

	/* the following character must be a whitespace (or end of
	   line) */

	++input;
	if (*input != 0 && !isspace(*input)) {
		*input_p = input;
    fprintf(stderr, "Space expected after closing '\"'\n");
		return NULL;
	}

	/* finish the string and return it */

	*dest = 0;
	*input_p = strip_leading_whitespace(input);
	return word;
}

char *
tokenizer_next_param(char **input_p)
{
	assert(input_p != NULL);
	assert(*input_p != NULL);

	if (**input_p == '"')
		return tokenizer_next_string(input_p);
	else
		return tokenizer_next_unquoted(input_p);
}

#endif
