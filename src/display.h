#ifndef _DISPLAY_H_
#define _DISPAY_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#include <VG/openvg.h>
#include <VG/vgu.h>
#include <EGL/egl.h>
#include <GLES/gl.h>


/* const VGfloat white_colour[4] = {1.0, 1.0, 1.0, 1.0};
const VGfloat colour[4] = {0.0, 0.0, 0.0, 1.0};
*/

#define LEFT_BOUND 1
#define RIGHT_BOUND 2

#define BALL_WIDTH 100
#define BALL_HEIGHT 90

VGPath  path;
VGPaint paint;

typedef struct
{
   uint32_t x_position;
   uint32_t y_position;
} game_entity_t;

typedef struct
{
  uint32_t screen_width;
  uint32_t screen_height;
  /* OpenGL|ES objects*/
  EGLDisplay display;
  EGLSurface surface;
  EGLContext context;
  /* Stores any hard edges to the screen
   * No edges: 0
   * Left edge: 1
   * Right edge: 2
   * Left and right edge: 3
   */

  bool showfps;

  game_entity_t *entity;
} display_state_t;

/* Returns a random number bounded by n */
unsigned int randint(int n);

/* Returns a random float bounded by n */
VGfloat randf(int n);

void display_exit(void);

display_state_t
display_init(display_state_t *state);

/* new window size or exposure */
void display_reshape(int w, int h);

void display_draw(display_state_t *state);

void bcm_egl_openvg_init (display_state_t *state);

#endif
