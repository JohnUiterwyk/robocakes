#include "sim_lib.h"
#include "display.h"
#include <stdlib.h>
int
main(int argv, char **argc)
{
  int i;
  sim_object_t * object;
  /*Create a sim_data object*/
  sim_data_t* test_sim = sim_new();
  sim_init(test_sim,1, 10, 192, 120);

  /*Create a display_state*/
  display_state_t * state = calloc(1,sizeof(display_state_t));
  /* memset(state, 0, sizeof(*state)); */
  bcm_egl_openvg_init(state);
  display_init(state, 0);

  /*
  for(i = 0; i < 1000; i++)
  */
  while(1)
  {
    sim_tick(test_sim);
    /*Pass to display and alter*/
    display_draw(state, test_sim);

    /*
    for(i = 0; i < test_sim->size; i++)
    {
      object = test_sim->objects[i];
      object->x += 1;
      object->y += 1;
    }
    */
  }
  return EXIT_SUCCESS;
}
