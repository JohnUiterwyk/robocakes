#include "client.h"

void main_client() {
  int boundaries = 0;
  UDP_ConnectionData connData;
  char message[MAX_BUFFER_LEN];
  Sim_Data * simData;

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
  printf("Server still at %s\n", config_get_string(CONF_SERVER_IP, "localhost"));
  connData.ipAddress = &(config_get_string(CONF_SERVER_IP, DEFAULT_SERVER_IP));
  connData.port = &(config_get_string(CONF_SERVER_PORT, DEFAULT_SERVER_PORT));
  UDP_CreateSocket(&connData);

  init(state, boundaries);

  while(true) {
    UDP_ReceiveMessage(&connData, message);
    Sim_UnserializeData(simData, message);
    draw(state);
    nanosleep(&sleepTime, &sleepTimeResult);
  }
}
