#include "client.h"

void main_client()
{
  udp_conn_data_t connData;
  char message[MAX_BUFFER_LEN];
  Sim_Data * simData;


  struct timespec sleepTime, sleepTimeResult;

  sleepTime.tv_sec = 0;
  sleepTime.tv_nsec = 0.001 * 1000000000;
    
  /* 
   * begin display stuff
   * make state a member of the client data struct
   */
#ifdef _HAS_EGL
  STATE_T * state = safe_malloc(sizeof(STATE_T),1); //swap order of paa
  memset(state, 0, sizeof(*state)); // clear application state
  bcm_egl_openvg_init(state);
  init(&state);
#endif

    /*
     * end display stuff
     */
    
    

  connData.socketType = SOCKET_TYPE_LISTEN;

  connData.ipAddress = config_get_string(CONF_SERVER_IP, DEFAULT_SERVER_IP);
  connData.port = config_get_string(CONF_SERVER_PORT, DEFAULT_SERVER_PORT);

  udp_create_socket(&connData);


  while(true) {
    udp_receive_message(&connData, message);
    /* Sim_UnserializeData(simData, message); */
      
#ifdef _HAS_EGL
      draw(state);
#endif
    nanosleep(&sleepTime, &sleepTimeResult);
  }
}
