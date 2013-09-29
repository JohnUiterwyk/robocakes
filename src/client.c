#include "client.h"

void main_client() {
  int boundaries = 0;
  udp_conn_data_t connData;
  char message[MAX_BUFFER_LEN];
  Sim_Data * simData;

  char ip[MAX_STRING_SIZE];
  char port[MAX_STRING_SIZE];

  struct timespec sleepTime, sleepTimeResult;

  sleepTime.tv_sec = 0;
  sleepTime.tv_nsec = 0.001 * 1000000000;
  /* TODO Change to calloc */
  STATE_T _state, *state=&_state;
  /* STATE_T *state = malloc(sizeof(state)); */

  memset(state, 0, sizeof(*state)); // clear application state
  bcm_egl_openvg_init(state);
  if (config_get_bool(CONF_LEFT_EDGE, false)) {
    boundaries += 1;
  }
  if (config_get_bool(CONF_RIGHT_EDGE, false)) {
    boundaries += 2;
  }

  connData.socketType = SOCKET_TYPE_LISTEN;

  /*
  strcpy(connData.ipAddress, config_get_string(CONF_SERVER_IP,
        DEFAULT_SERVER_IP));
  strcpy(connData.port, config_get_string(CONF_SERVER_PORT,
        DEFAULT_SERVER_PORT));
        */

  strcpy(ip, config_get_string(CONF_SERVER_IP, DEFAULT_SERVER_IP));
  strcpy(port, config_get_string(CONF_SERVER_PORT, DEFAULT_SERVER_PORT));
  /*
  strncpy(ip, config_get_string(CONF_SERVER_IP, DEFAULT_SERVER_IP));
  connData.ipAddress = ip;
  strncpy(port, config_get_string(
  connData.port = port;

  */

  printf("IP address is: %s\n", ip);
  printf("Port is: %s\n", port);
  connData.ipAddress = config_get_string(CONF_SERVER_IP, DEFAULT_SERVER_IP);
  connData.port = config_get_string(CONF_SERVER_PORT, DEFAULT_SERVER_PORT);
  /*
  connData.ipAddress = ip;
  connData.port = port;
  */
  udp_create_socket(&connData);

  init(&state);

  while(true) {
    udp_receive_message(&connData, message);
    /* Sim_UnserializeData(simData, message); */
    draw(state);
    nanosleep(&sleepTime, &sleepTimeResult);
  }
}
