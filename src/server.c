#include "server.h"

void
main_server()
{

  UDP_ConnectionData connData;
  Sim_Data * simData;
  char message[MAX_BUFFER_LEN];
  int numOfObjects, i;

  simData = Sim_New();
  Sim_Init(simData, 1, config_get_unsigned(CONF_MAP_WIDTH, DEFAULT_MAP_WIDTH),
      1200);

  struct timespec sleepTime, sleepTimeResult;
  sleepTime.tv_sec = 0;
  sleepTime.tv_nsec = 0.001 *1000000000;

  connData.socketType = SOCKET_TYPE_BROADCAST;
  connData.ipAddress = config_get_string(CONF_SERVER_IP, "localhost");
  connData.port = config_get_string(CONF_SERVER_PORT, "8000");

  UDP_CreateSocket(&connData);

  while(strcmp(message, "exit") != 0) {

    getString(message, MAX_BUFFER_LEN, ">");

    if(strcmp(message, "start") == 0) {
      for (i = 0; i < 5000; i++) {
        Sim_Tick(simData);
        Sim_SerializeState(simData, message, MAX_BUFFER_LEN);
        UDP_SendMessage(&connData, message);
        nanosleep(&sleepTime, &sleepTimeResult);
      }
      printf("Done\n");
    }
  }
}
