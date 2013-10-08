#include "client.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void client_start(struct configuration * conf)
{
  client_data_t * data;
  pthread_t draw_thread;

  /* if not on os x, init display */
    /* in future change to ifdef egl */
    
    data = client_data_init();
#ifndef __APPLE__
#ifdef HAVE_GLES
  /* swap order of paa */
  bcm_egl_openvg_init(data->display_state);
  display_init(data->display_state, conf->position);
#endif
#endif
    
    sim_data_t * test;
    test = sim_new();
    sim_init(test, 3, 10, 1200,1200);
    while(1)
    {
        sim_tick(test);
        display_draw(data->display_state, test);
    }
    
  data->time_data->interval = .033;
  strcpy(data->tcp_client->dest_ip_address, conf->ipaddress);
  strcpy(data->tcp_client->port, conf->port);
  tcp_client_start(data->tcp_client);

  pthread_create(&draw_thread,NULL,&draw_thread_func,data);
  pthread_join(draw_thread,NULL);

}
client_data_t * client_data_init()
{
  client_data_t * client_data;
  client_data = calloc(1, sizeof(client_data_t));
  if(client_data == NULL)
  {
    perror("piss off!");
    exit(EXIT_FAILURE);
  }

  client_data->tcp_client = tcp_client_data_new();
  client_data->time_data = timeloop_new();
  client_data->draw_message = calloc(MAX_BUFFER_LEN, sizeof(char));
  client_data->sim_data = sim_new();
  client_data->display_state = safe_calloc(1,sizeof(display_state_t));
  return client_data;
}

void * draw_thread_func(void * data)
{
  client_data_t * client_data = (client_data_t *) data;
  timeloop_start(client_data->time_data,&client_timer_tick,client_data);

  return NULL;
}


void * client_timer_tick(void * data)
{
  client_data_t * client_data;
  client_data = (client_data_t *) data;
  int redraw = 0;

  redraw = thread_copy_from_buffer(client_data->tcp_client->recv_buffer,
      client_data->draw_message);

  if(redraw)
  {
    sim_deserialize_state(client_data->sim_data, client_data->draw_message);
    
    //
#ifndef __APPLE__
#ifdef HAVE_GLES
    display_draw(client_data->display_state, client_data->sim_data);
#endif
#endif
  }

  return NULL;
}
