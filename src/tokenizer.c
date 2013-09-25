
#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_
#include "config.h"
#include "tokenizer.h"

#include <stdbool.h>
#include <assert.h>
#include <string.h>

G_GNUC_CONST
static GQuark
tokenizer_quark(void)
{
	return g_quark_from_static_string("tokenizer");
}

static inline bool
valid_word_first_char(char ch)
{
	return g_ascii_isalpha(ch);
}

static inline bool
valid_word_char(char ch)
{
	return g_ascii_isalnum(ch) || ch == '_';
}

static inline bool
valid_numeral_char(char ch)
{
  return g_ascii_isdigit(ch) || ch == ',';
}

static inline bool
valid_numeral_first_char(char ch)
{
  return g_ascii_isdigit(ch);
}

char *
tokenizer_next_word(char **input_p, GError **error_r)
{
	char *word, *input;

	assert(input_p != NULL);
	assert(*input_p != NULL);

	word = input = *input_p;

	if (*input == 0)
		return NULL;

	/* check the first character */

	if (!valid_word_first_char(*input)) {
		g_set_error(error_r, tokenizer_quark(), 0,
			    "Letter expected");
		return NULL;
	}

	/* now iterate over the other characters until we find a
	   whitespace or end-of-string */

	while (*++input != 0) {
		if (g_ascii_isspace(*input)) {
			/* a whitespace: the word ends here */
			*input = 0;
			/* skip all following spaces, too */
			input = g_strchug(input + 1);
			break;
		}

		if (!valid_word_char(*input)) {
			*input_p = input;
			g_set_error(error_r, tokenizer_quark(), 0,
				    "Invalid word character");
			return NULL;
		}
	}

	/* end of string: the string is already null-terminated
	   here */

	*input_p = input;
	return word;
}

char *
tokenizer_next_udp_message(char **input_p, GError **error_r)
{
  char *word, *input;

  assert(input_p != NULL);
  assert(*input_p != NULL);

  word = input = *input_p;

  if (*input == 0)
    return NULL;

  if (!valid_numeral_first_char(*input)) {
    g_set_error(error_r, tokenizer_quark(), 0,
        "Number expected");
    return NULL;
  }

  /* Now iterate over the other characters until we find a semicolon or
   * end-of-string */
  while (*++input != 0) {
    if (*input == ';') {
      /* A semi-colon. The block ends here. */
      *input = 0;
      /* skip all the following spaces, too */
      input = g_strchug(input + 1);
      break;
    }

    if (!valid_numeral_char(*input)) {
      g_set_error(error_r, tokenizer_quark(), 0,
          "Invalid character");
      return NULL;
    }
  }
  /* End of string: the string is already null-terminated here */
  *input_p = input;
  return word;
}

int
tokenizer_next_udp_number(char **input_p, GError **error_r)
{
  int number;
  char *word, *input;

  assert(input_p != NULL);
  assert(*input_p != NULL);

  word = input = *input_p;

  if (*input == 0)
    return -1;

  if (!valid_numeral_first_char(*input)) {
    g_set_error(error_r, tokenizer_quark(), 0,
        "Number expected");
    return -1;
  }

  /* Now iterate over the other characters until we find a semicolon or
   * comma */
  while (*++input != 0) {
    if (*input == ';' || *input == ',') {
      /* A semi-colon or comma. The number ends here. */
      *input = 0;
      /* skip all the following spaces, too */
      input = g_strchug(input + 1);
      break;
    }

    if (!valid_numeral_first_char(*input)) {
      g_set_error(error_r, tokenizer_quark(), 0,
          "Invalid character");
      return -1;
    }
  }
  /* End of string: the string is already null-terminated here */
  number = strtol(*input_p, &input, 10);
  *input_p = input;
  return number;
}

static inline bool
valid_unquoted_char(char ch)
{
	return (unsigned char)ch > 0x20 && ch != '"' && ch != '\'';
}

char *
tokenizer_next_unquoted(char **input_p, GError **error_r)
{
	char *word, *input;

	assert(input_p != NULL);
	assert(*input_p != NULL);

	word = input = *input_p;

	if (*input == 0)
		return NULL;

	/* check the first character */

	if (!valid_unquoted_char(*input)) {
		g_set_error(error_r, tokenizer_quark(), 0,
			    "Invalid unquoted character");
		return NULL;
	}

	/* now iterate over the other characters until we find a
	   whitespace or end-of-string */

	while (*++input != 0) {
		if (g_ascii_isspace(*input)) {
			/* a whitespace: the word ends here */
			*input = 0;
			/* skip all following spaces, too */
			input = g_strchug(input + 1);
			break;
		}

		if (!valid_unquoted_char(*input)) {
			*input_p = input;
			g_set_error(error_r, tokenizer_quark(), 0,
				    "Invalid unquoted character");
			return NULL;
		}
	}

	/* end of string: the string is already null-terminated
	   here */

	*input_p = input;
	return word;
}

char *
tokenizer_next_string(char **input_p, GError **error_r)
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
		g_set_error(error_r, tokenizer_quark(), 0,
			    "'\"' expected");
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
			g_set_error(error_r, tokenizer_quark(), 0,
				    "Missing closing '\"'");
			return NULL;
		}

		/* copy one character */
		*dest++ = *input++;
	}

	/* the following character must be a whitespace (or end of
	   line) */

	++input;
	if (*input != 0 && !g_ascii_isspace(*input)) {
		*input_p = input;
		g_set_error(error_r, tokenizer_quark(), 0,
			    "Space expected after closing '\"'");
		return NULL;
	}

	/* finish the string and return it */

	*dest = 0;
	*input_p = g_strchug(input);
	return word;
}

char *
tokenizer_next_param(char **input_p, GError **error_r)
{
	assert(input_p != NULL);
	assert(*input_p != NULL);

	if (**input_p == '"')
		return tokenizer_next_string(input_p, error_r);
	else
		return tokenizer_next_unquoted(input_p, error_r);
}

#endif
