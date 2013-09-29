#include "util.h"
#include "client.h"
#include "server.h"


int
main (int argc, char **argv)
{
  /* Getopt variables */
  int c;
  char *config_file = NULL;
  bool ret;

  static const char *optstring = "fc:h?";

  if (argc < REQUIRED_ARGS) {
    print_usage();
    return EXIT_SUCCESS;
  }

  /* Process options */
  while ((c = getopt(argc, argv, optstring)) != GETOPT_FINISHED) {
    switch (c) {
      case 'c':
        config_file = optarg;
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

  /* TODO Remove this when we read in configs from /etc */
  if (!config_file) {
    fprintf(stderr, "FIXME: Configuration file required at the moment.\n");
    return EXIT_FAILURE;
  }

  ret = config_read_file(config_file);

  printf("Starting in %s mode.\n",
    config_get_string(CONF_ROLE, CLIENT_ROLE));

  if (DEBUG) {
    printf("Server at %s\n", config_get_string(CONF_SERVER_IP, "localhost"));
    printf("Server port: %s\n", config_get_string(CONF_SERVER_PORT, "80"));
    printf("Net method: %s\n", config_get_string(CONF_NET_METHOD, "Unknown"));
    printf("Map width: %s\n", config_get_string(CONF_MAP_WIDTH, "100"));
    printf("Left edge: %s\n", config_get_string(CONF_LEFT_EDGE, "foo"));
    printf("Right edge: %s\n", config_get_string(CONF_RIGHT_EDGE, "bar"));
    printf("Position: %s\n", config_get_string(CONF_POSITION, "100"));
  }

  if (strcmp(config_get_string(CONF_ROLE, CLIENT_ROLE), CLIENT_ROLE) == 0)
    main_client();
  else if (strcmp(config_get_string(CONF_ROLE, CLIENT_ROLE), SERVER_ROLE) == 0)
    //main_server();

  return EXIT_SUCCESS;

}
