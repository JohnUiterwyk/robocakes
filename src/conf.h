/*
 * Configuration parser for config files.
 *
 * Original configuration parser Copyright (C) 2003-2010 The Music
 * Player Daemon Project
 * http://www.musicpd.org
 *
 */
#ifndef _CONFIG_H_
#define _CONFIG_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

#include "tokenizer.h"
#include "util.h"

#undef G_LOG_DOMAIN
#define G_LOG_DOMAIN "config"

#define CONF_COMMENT '#'

#define CONF_SERVER_IP "server_ip"
#define CONF_SERVER_PORT "server_port"
#define CONF_ROLE "role"
#define CONF_MAP_WIDTH "map_width"
#define CONF_NET_METHOD "net_method"
#define CONF_LEFT_EDGE "left_edge"
#define CONF_RIGHT_EDGE "right_edge"
#define CONF_POSITION "position"


#define CONFIG_ENTRY_COUNT 8

struct config_param {
   char *value;

   unsigned int line;
   /* Flag is false if option has not been queried yet. */
   bool used;
};

struct config_entry {
   const char *const name;

   struct config_param *params;
};

static struct config_entry config_entries[] = {
   { .name = CONF_SERVER_IP },
   { .name = CONF_SERVER_PORT },
   { .name = CONF_ROLE },
   { .name = CONF_MAP_WIDTH },
   { .name = CONF_NET_METHOD },
   { .name = CONF_LEFT_EDGE },
   { .name = CONF_RIGHT_EDGE },
   { .name = CONF_POSITION },
};


static bool get_bool(const char *value, bool *value_r);
struct config_param * config_new_param(const char *value, int line);
static void config_param_free(struct config_param *param);
static struct config_entry * config_entry_get(const char *name);
void config_global_finish(void);
void config_global_init(void);
bool config_read_file(const char *file);
struct config_param * config_get_param(const char *name);
const char * config_get_string(const char *name, const char *default_value);
unsigned config_get_unsigned(const char *name, unsigned default_value);
unsigned config_get_positive(const char *name, unsigned default_value);
bool config_get_bool(const char *name, bool default_value);

#endif
