/*
 * Configuration parser for config files.
 *
 * Not actually used in current implementation, but the idea is that it
 * should read configuration from a conf file. An example is provided in
 * the examples document.
 *
 * Original configuration parser Copyright (C) 2003-2010 The Music
 * Player Daemon Project
 * http://www.musicpd.org
 *
 * GlibC dependencies removed by
 * John Uiterwyk, Virginia King, Stewart Johnston
 * Also de-static-ed.
 * 
 */

#include "conf.h"

bool
get_bool(const char *value, bool *value_r)
{
        const char *t[] = { "yes", "true", "1", NULL };
        const char *f[] = { "no", "false", "0", NULL };

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
        struct config_param *ret = safe_calloc(1,sizeof(struct config_param));

        if (!value)
                ret->value = NULL;
        else
                ret->value = strdup(value);

        ret->used = false;

        return ret;
}

void
config_param_free(struct config_param *param)
{
        free(param->value);

        free(param);
}

struct config_entry *
config_entry_get(const char *name)
{
        unsigned i;
        for (i = 0; i < CONFIG_ENTRY_COUNT; ++i) {
                struct config_entry *entry = &config_entries[i];
                if (strcmp(entry->name, name) == 0)
                        return entry;
        }

        return NULL;
}

void config_global_finish(void)
{
        unsigned i;
        for (i = 0; i < CONFIG_ENTRY_COUNT; ++i) {
                struct config_entry *entry = &config_entries[i];

                config_param_free(entry->params);
        }
}

void config_global_init(void)
{
}

bool
config_read_file(const char *file)
{
        FILE *fp;
        char *string;
        int count = 0;
        struct config_entry *entry;
        struct config_param *param;
        char *line;
        const char *name, *value;

        string = safe_calloc(MAX_STRING_SIZE + 1, sizeof(char));
        /*
           g_debug("loading file %s", file);
           */

        if (!(fp = fopen(file, "r"))) {
                fprintf(stderr, "Failed to open %s: %s\n", file,
                                strerror(errno));
                free(string);
                return false;
        }

        while (fgets(string, MAX_STRING_SIZE, fp)) {

                count++;

                strip_whitespace(string);
                line = string;
                if (*line == 0 || *line == CONF_COMMENT)
                        continue;

                /* the first token in each line is the name, followed
                   by either the value or '{' */

                name = tokenizer_next_word(line);
                if (name == NULL) {
                        assert(*line != 0);
                        fprintf(stderr, "Error reading line %i\n", count);
                        fclose(fp);
                        free(string);
                        return false;
                }

                /* get the definition of that option, and check the
                   void config_global_check(void);
                   "repeatable" flag */

                entry = config_entry_get(name);
                if (entry == NULL) {
                        fprintf(stderr, "Unrecognised parameter in config file at line %i: %s\n",
                                        count, name);
                        fclose(fp);
                        free(string);
                        return false;
                }

                if (entry->params != NULL) {
                        param = entry->params;
                        fprintf(stderr, "Config parameter \"%s\" is first defined on line %i and redefined on line %i\n",
                                        name, param->line, count);
                        fclose(fp);
                        free(string);
                        return false;
                }

                /* now parse the value */
                value = tokenizer_next_string(line + strlen(name));
                if (value == NULL) {
                        if (*line == 0)
                                fprintf(stderr, "Config error on line %i: Value missing\n",
                                                count);
                        else {
                                fprintf(stderr, "Config error on line %i: Something funny going on.\n",
                                                count);
                        }

                        fclose(fp);
                        free(string);
                        return false;
                }

                if (*line != 0 && *line != CONF_COMMENT) {
                        fprintf(stderr, "Config error on line %i: Unknown tokens after value.\n",
                                        count);
                        fclose(fp);
                        free(string);
                        return false;
                }

                param = config_new_param(value, count);

                entry->params = param;
        }
        fclose(fp);

        free(string);
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
                fprintf(stderr, "Not a positive number in line %i",
                                param->line);

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
                fprintf(stderr, "%s is not a boolean value (yes, true, 1) or (no, false, 0) on line %i\n",
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
