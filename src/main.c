#include "util.h"
#include "client.h"
#include "server.h"
#include "config.h"
#include "conf.h"

int
main (int argc, char **argv)
{
  /* Getopt variables */
  int c;
  bool ret;

  char *config_file = NULL;

  struct configuration * conf;

  static const char *optstring = "fc:n:r:p:i:t:b:P:W:H:h?";

  conf = new_config();
  init_config(conf);

  if (argc < REQUIRED_ARGS) {
    print_usage();
    return EXIT_SUCCESS;
  }

  /* Process options */
  while ((c = getopt(argc, argv, optstring)) != GETOPT_FINISHED) {
    switch (c) {
      case 'f':
      case 'c':
        config_file = optarg;
        break;
      case 'n':
        conf->clients = atoi(optarg);
        break;
      case 'r':
        conf->role = optarg;
      case 'p':
        conf->position = atoi(optarg);
        break;
      case 'i':
        conf->ipaddress = optarg;
        break;
      case 'P':
        conf->port = optarg;
        break;
      case 'W':
        conf->width = atoi(optarg);
        break;
      case 'H':
        conf->height = atoi(optarg);
        break;
      case 'b':
        conf->balls = atoi(optarg);
        break;
      case 't':
        conf->interval = atoi(optarg);
        break;
      case ':':
        fprintf(stderr, "Option %c requires an argument.\n", optopt);
        return EXIT_FAILURE;
      case 'h':
      case '?':
        print_usage();
        return EXIT_SUCCESS;
      default:
        return EXIT_FAILURE;
    }
  }

  /* TODO Fix configuration reader and tokenizer */
  /*
  if (config_file) {
    ret = config_read_file(config_file);
  }
  else {
    if (access(USER_CONFIG_FILE_LOCATION, F_OK) != -1) {
      ret = config_read_file(USER_CONFIG_FILE_LOCATION);
    }
    else if (access(SYSTEM_CONFIG_FILE_LOCATION, F_OK) != -1) {
      ret = config_read_file(SYSTEM_CONFIG_FILE_LOCATION);
    }
    else {
      fprintf(stderr, "Could not find configuration file.\n");
      return EXIT_FAILURE;
    }
  }
  */

  /*
  printf("Starting in %s mode.\n",
    config_get_string(CONF_ROLE, CLIENT_ROLE));

  if (DEBUG) {
    printf("Server at %s\n", config_get_string(CONF_SERVER_IP, "localhost"));
    printf("Server port: %s\n", config_get_string(CONF_SERVER_PORT, "80"));
    printf("Net method: %s\n", config_get_string(CONF_NET_METHOD, "Unknown"));
    printf("Map width: %s\n", config_get_string(CONF_MAP_WIDTH, "100"));
    printf("Left edge: %s\n", config_get_string(CONF_LEFT_EDGE, "foo"));
    printf("Right edge: %s\n", config_get_string(CONF_RIGHT_EDGE, "bar"));
    printf("Position: %s\n", config_get_string(CONF_POSITION, position_str));
  }

  position = atoi(position_str);
  */


  if (strcmp(conf->role, CLIENT_ROLE) == 0)
  {
    client_start(conf);
  }
  else if (strcmp(conf->role, SERVER_ROLE) == 0)
  {
    server_start(conf);
  }

  return EXIT_SUCCESS;

}
