/**
 * \mainpage Hydra - A multi display simulator
 * \author John Uiterwyk
 * \author Virginia King
 * \author Stewart Johnston
 * \copyright GNU Public License.
 *
 * \image html hydra-logo.png
 * \image latex hydra-logo.eps
 *
 * \section intro_sec Introduction
 * Hydra currently simulates a series of balls bouncing across multiple
 * screens, each controlled by a seperate device.
 *
 * \section install_sec Installation
 * \subsection requirements_subsec Requirements
 *
 * The requirements for Hydra depend somewhat on the platform. For
 * headless operation (eg, a server, or if you are implementing your own
 * display) the requirements are simply those required to actually
 * compile the package:
 *      * gcc
 *      * make
 *
 * However, building on a Raspberry pi requires a good deal more work,
 * since Hydra requires OpenGLES and OpenVG to actually draw exciting
 * things to the screen. Additionally, the raspberrypi.org debian
 * archives (which are not enabled by default on Raspbian) are needed
 * for some libraries (libraspberrypi-dev)
 *
 * Raspbian has an additional repository section specifically for the pi
 * which is needed.
 *
 * Change your source in /etc/apt/sources.list from
 *
 *     deb http://archive.raspbian.org/raspbian wheezy main
 *
 * to
 *
 *     deb http://archive.raspbian.org/raspbian wheezy main rpi
 *
 * To add the raspberrypi repository, create a new file
 * /etc/apt/sources.list.d/raspi.list and copy the following into it:
 *
 *     deb http://archive.raspberrypi.org/debian/ wheezy main
 *     deb-src http://archive.raspberrypi.org/debian/ wheezy main
 *
 * You'll also need to add the gpg key for raspberrypi.org (you should
 * check it first)
 *
 *     wget http://archive.raspbian.org/raspbian.public.key -O - | sudo apt-key add -
 *     wget http://archive.raspberrypi.org/debian/raspberrypi.gpg.key -O - | sudo apt-key add -
 *
 * Remember to run apt-get update, and then you are ready for the
 * dependencies!
 *      * libegl1-mesa
 *      * libjpeg8-dev
 *      * libegl1-mesa                                                                    
 *      * libjpeg8-dev                                                                    
 *      * git-all                                                                         
 *      * build-essential                                                                 
 *      * mesa-utils                                                                      
 *      * libegl1-mesa-dev                                                                
 *      * libgles1-mesa                                                                   
 *      * libgles1-mesa-dev                                                               
 *      * libopenvg1-mesa                                                                 
 *      * libopenvg1-mesa-dev                                                             
 *      * libraspberrypi-dev                                                              
 *      * libfreetype6-dev 
 *
 * \subsection install_subsec Install
 *
 * Finally, follow the usual procedure for installing
 *     ./configure
 *     make
 *      make install
 *
 * Phew. Or you could wait for the .deb
 *
 * \section autotools_sec Autotools
 *
 * The Hydra project uses GNU Autotools. This makes it (relatively) easy
 * to develop the project on multiple different systems and
 * conditionally exclude sections that are unavailable on that platform.
 * For instance, the display module relies on OpenVG and OpenGLES.
 * Additionally, the OpenVG implementation relies on specific hardware
 * being present (the Broadcom VideoCore IV) in order to operate.
 *
 * Autotools allowed us to make the entire display library an optional
 * component that is only included if the project is compiled on
 * compatible systems.
 *
 * \subsection autoconf_subsec Autoconf configure.ac
 *
 * \includelineno configure.ac
 *
 * \subsection automake_subsec Automake Makefile.am
 *
 * \includelineno Makefile.am
 */
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
