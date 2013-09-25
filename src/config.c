/*
 * Copyright (C) 2003-2010 The Music Player Daemon Project
 * http://www.musicpd.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "config.h"

static bool
get_bool(const char *value, bool *value_r)
{
	static const char *t[] = { "yes", "true", "1", NULL };
	static const char *f[] = { "no", "false", "0", NULL };

	if (string_array_contains(t, value)) {
		*value_r = true;
		return true;
	}

	if (string_array_contains(f, value)) {
		*value_r = false;
		return true;
	}

	return false;
}

struct config_param *
config_new_param(const char *value, int line)
{
	struct config_param *ret = g_new(struct config_param, 1);

	if (!value)
		ret->value = NULL;
	else
		ret->value = g_strdup(value);

	ret->used = false;

	return ret;
}

static void
config_param_free(struct config_param *param)
{
	g_free(param->value);

	g_free(param);
}

static struct config_entry *
config_entry_get(const char *name)
{
  unsigned i;
	for (i = 0; i < G_N_ELEMENTS(config_entries); ++i) {
		struct config_entry *entry = &config_entries[i];
		if (strcmp(entry->name, name) == 0)
			return entry;
	}

	return NULL;
}

void config_global_finish(void)
{
  unsigned i;
	for (i = 0; i < G_N_ELEMENTS(config_entries); ++i) {
		struct config_entry *entry = &config_entries[i];

    config_param_free(entry->params);
	}
}

void config_global_init(void)
{
}

bool
config_read_file(const char *file, GError **error_r)
{
	FILE *fp;
	char string[MAX_STRING_SIZE + 1];
	int count = 0;
	struct config_entry *entry;
	struct config_param *param;
  char *line;
  const char *name, *value;
  GError *error = NULL;

	g_debug("loading file %s", file);

	if (!(fp = fopen(file, "r"))) {
		g_set_error(error_r, config_quark(), errno,
			    "Failed to open %s: %s",
			    file, strerror(errno));
		return false;
	}

  while (fgets(string, MAX_STRING_SIZE, fp)) {
    error = NULL;

    count++;

    line = g_strchug(string);
    if (*line == 0 || *line == CONF_COMMENT)
      continue;

    /* the first token in each line is the name, followed
       by either the value or '{' */

    name = tokenizer_next_word(&line, &error);
    if (name == NULL) {
      assert(*line != 0);
      g_propagate_prefixed_error(error_r, error,
          "line %i: ", count);
      fclose(fp);
      return false;
    }

    /* get the definition of that option, and check the
void config_global_check(void);
       "repeatable" flag */

    entry = config_entry_get(name);
    if (entry == NULL) {
      g_set_error(error_r, config_quark(), 0,
          "unrecognized parameter in config file at "
          "line %i: %s\n", count, name);
      fclose(fp);
      return false;
    }

    if (entry->params != NULL) {
      param = entry->params;
      g_set_error(error_r, config_quark(), 0,
          "config parameter \"%s\" is first defined "
          "on line %i and redefined on line %i\n",
          name, param->line, count);
      fclose(fp);
      return false;
    }

    /* now parse the value */
    value = tokenizer_next_string(&line, &error);
    if (value == NULL) {
      if (*line == 0)
        g_set_error(error_r, config_quark(), 0,
            "line %i: Value missing",
            count);
      else {
        g_set_error(error_r, config_quark(), 0,
            "line %i: %s", count,
            error->message);
        g_error_free(error);
      }

      fclose(fp);
      return false;
    }

    if (*line != 0 && *line != CONF_COMMENT) {
      g_set_error(error_r, config_quark(), 0,
          "line %i: Unknown tokens after value",
          count);
      fclose(fp);
      return false;
    }

    param = config_new_param(value, count);

    entry->params = param;
  }
	fclose(fp);

	return true;
}

const char *
config_get_string(const char *name, const char *default_value)
{
	const struct config_param *param = config_get_param(name);

	if (param == NULL)
		return default_value;

	return param->value;
}

unsigned
config_get_unsigned(const char *name, unsigned default_value)
{
	const struct config_param *param = config_get_param(name);
	long value;
	char *endptr;

	if (param == NULL)
		return default_value;

	value = strtol(param->value, &endptr, 0);
	if (*endptr != 0 || value < 0)
		fprintf(stderr, "Not a valid non-negative number in line %i",
			  param->line);

	return (unsigned)value;
}

unsigned
config_get_positive(const char *name, unsigned default_value)
{
	const struct config_param *param = config_get_param(name);
	long value;
	char *endptr;

	if (param == NULL)
		return default_value;

	value = strtol(param->value, &endptr, 0);
	if (*endptr != 0)
		fprintf(stderr, "Not a valid number in line %i", param->line);

	if (value <= 0)
		fprintf(stderr, "Not a positive number in line %i", param->line);

	return (unsigned)value;
}

bool config_get_bool(const char *name, bool default_value)
{
	const struct config_param *param = config_get_param(name);
	bool success, value;

	if (param == NULL)
		return default_value;

	success = get_bool(param->value, &value);
	if (!success)
		fprintf(stderr, "%s is not a boolean value (yes, true, 1) or "
			  "(no, false, 0) on line %i\n",
			  name, param->line);

	return value;
}

struct config_param *
config_get_param(const char *name)
{
  struct config_entry *entry;
  struct config_param *param;

  entry = config_entry_get(name);
  if (entry == NULL)
    return NULL;

  param = entry->params;
  param->used = true;

  return param;
}

static inline GQuark
config_quark(void)
{
  return g_quark_from_static_string("config");
}
