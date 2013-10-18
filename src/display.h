/**
 * @file display.h
 */
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

#include "sim_lib.h"

/* const VGfloat white_colour[4] = {1.0, 1.0, 1.0, 1.0};
const VGfloat colour[4] = {0.0, 0.0, 0.0, 1.0};
*/

#define LEFT_BOUND 1
#define RIGHT_BOUND 2

#define BALL_WIDTH 100
#define BALL_HEIGHT 90

VGPath  path;
VGPaint paint;

/**
 * The OpenVG display context
 */
typedef struct
{
  uint32_t screen_width;
  uint32_t screen_height;

  /* Position on map */
  uint32_t left_edge;
  uint32_t right_edge;
  uint32_t position;
  /* OpenGL|ES objects*/
  /** The display device to draw to. */
  EGLDisplay display;
  /** The display surface to draw on. */
  EGLSurface surface;
  /** The EGL rendering context. */
  EGLContext context;
  /* Stores any hard edges to the screen
   * No edges: 0
   * Left edge: 1
   * Right edge: 2
   * Left and right edge: 3
   */

  /** Flag to display or hide frames per second information. */
  bool showfps;

} display_state_t;

/** Create a random number bounded by n. */
unsigned int randint(int n);

/** Create a random float bounded by n */
VGfloat randf(int n);

void display_exit(void);

/** Sets up the cosmetics of the display.
 *
 * @param state The EGL state to use for display.
 * @param position The position of the pi in the map.
 * Each pi is assigned a position from left to right, starting with
 * 0 for the left-most pi.
 */
display_state_t
display_init(display_state_t *state, uint32_t position);

/* new window size or exposure */
void display_reshape(int w, int h);

/**
 * Draws the simulation data to the screen.
 *
 * Loops over each object ("ball") in the sim_data struct and draws
 * it at it's new position on the screen. Only objects located in the
 * client's zone of control are drawn.
 *
 * @param state The EGL state to use for display.
 * @param sim_data The current state of the simulation.
 */
void display_draw(display_state_t *state, sim_data_t *sim_data);

/**
 * Creates and configures EGL for drawing lots of balls.
 *
 * Calculates screen size, aspect ratio and projection information.
 */
void bcm_egl_openvg_init (display_state_t *state);

/**
 * Helper function to create a new path, ready to turn into a ball.
 */
VGPath newpath();

#endif
