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
safe_malloc(int size, int count)
{
  return calloc(count, size);
}
