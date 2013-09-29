#include "display.h"

unsigned int
randint(int n)
{
  return (unsigned int)((rand() % n));
}

STATE_T
init(STATE_T * state)
{
  printf("Screen dimensions as passed in: %dx%d\n", state->screen_width, state->screen_height);
  VGfloat clearcolour[] = {0.5f, 0.4f, 0.3f, 1.0f};/* grey-brownisch colour */
  VGfloat fillcolour[] = {1.0f, 1.0f, 1.0f, 1.0f};/* white colour */
  VGfloat blackcolour[] = {0.1f, 0.1f, 0.1f, 1.0f};/* black colour*/
  const VGubyte segments[4] = {VG_MOVE_TO_ABS,
    VG_SCCWARC_TO_ABS,
    VG_SCCWARC_TO_ABS,
    VG_CLOSE_PATH};

  /* bottom circle*/
  VGfloat data[12];
  /*
    const VGfloat cx = randint(state->screen_width),
    cy = randint(state->screen_height),
    width=BALL_WIDTH, height=BALL_HEIGHT;
  */
  const VGfloat cx = 0, cy=0, width=BALL_WIDTH, height=BALL_HEIGHT;
 // const VGfloat hw = width * 0.5f;
  //const VGfloat hh = height * 0.5f;

  state->entity = malloc(sizeof(GameEntity));
  if (state->entity == NULL) {
    fprintf(stderr, "Malloc had a boo-boo\n");
    return;
  }

  printf("Screen dimensions: %dx%d\n", state->screen_width, state->screen_height);
  /*
    state->entity->x_position = randint(state->screen_width);
    state->entity->y_position = randint(state->screen_height);
    state->entity->x_direction = 1;
    state->entity->y_direction = 1;
  */

  printf("X position: %d\n", state->entity->x_position);
  printf("Y position: %d\n", state->entity->y_position);

  /* TODO Read from configuration file */
  state->showfps = false;

  /* move to */
  //data[0] = cx + hw;
  //data[1] = cy;
  /*SCCWARC_TO_ABS*/
  //data[2] = hw;
  //data[3] = hh;
  //data[4] = 0;
  //data[5] = cx - hw;
  //data[6] = cy;
  /*SCCWARC_TO_ABS*/
  //data[7] = hw;
  //data[8] = hh;
  //data[9] = 0;
  //data[10] = data[0];
  //data[11] = cy;
  
  //Set the foreground colour
  vgSetfv(VG_CLEAR_COLOR , 4, clearcolour);
  /*vgSeti(VG_RENDERING_QUALITY, VG_RENDERING_QUALITY_NONANTIALIASED);*/
  vgSeti(VG_RENDERING_QUALITY, VG_RENDERING_QUALITY_BETTER);


  /*path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F,
      1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
  if (path == VG_INVALID_HANDLE) {
    return;
  }
  paint = vgCreatePaint();
  if (paint == VG_INVALID_HANDLE) {
    vgDestroyPath(path);
    return;
  }
  vgSetParameterfv(paint, VG_PAINT_COLOR, 4, fillcolour);
  vgSetParameteri(paint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);*/
  /* paint prepared*/

  /* now render bottom */
  //vgAppendPathData(path, 4, segments, data);

}

/* new window size or exposure */
void
reshape(int w, int h)
{
}

void
draw(STATE_T *state)
{
  static int frames = 0;
  static int start = 0;
  GameEntity *ball= state->entity;

  if(start ==0) {
    start = time(NULL);
  }
  int i;
  vgClear(0, 0, state->screen_width, state->screen_height);
  vgLoadIdentity();

  vgTranslate(ball->x_position, ball->y_position);
  
  VGPath path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);;
  vguEllipse(path, ball->x_position, ball->y_position, state->screen_width, state->screen_height);
  vgDrawPath(path, VG_FILL_PATH | VG_STROKE_PATH);
  vgDestroyPath(path);
  
  /* wind this value up to enable some reasonable benchmarking and fps watching stuff ...*/
  const int enable_loops = 48 /* 250 */;
  for (i=0;  i < enable_loops; i++) {
    /* vgTranslate(3,0);*/
    /* draw the snowman*/
    vgSetPaint(paint, VG_FILL_PATH);
    vgDrawPath(path,  VG_FILL_PATH);
  }
  vgFlush();
  /*swap buffers*/
  int vgerror = vgGetError();
  if (vgerror) {
    printf("%d %x vgerror was this\n", vgerror, vgerror);
  }
  assert(vgGetError() == VG_NO_ERROR);
  eglSwapBuffers(state->display, state->surface);
  assert(eglGetError() == EGL_SUCCESS);
  frames++;
  if(state->showfps) {
    if ((frames % 100) && (time(NULL) != start)) {
      printf("current fps is %.03f\n",  (((float) frames) / (float)(time(NULL) - start)));
    }
  }
}

void
bcm_egl_openvg_init (STATE_T *state) {
  bcm_host_init();
  int32_t success = 0;
  EGLBoolean result;
  EGLint num_config;

  static EGL_DISPMANX_WINDOW_T nativewindow;

  DISPMANX_ELEMENT_HANDLE_T dispman_element;
  DISPMANX_DISPLAY_HANDLE_T dispman_display;
  DISPMANX_UPDATE_HANDLE_T dispman_update;
  VC_RECT_T dst_rect;
  VC_RECT_T src_rect;

static const 
EGLint attribute_list[] = {
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_ALPHA_SIZE, 8,
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_NONE
  };

  EGLConfig config;

  /* get an EGL display connection*/
  state->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  assert(state->display!=EGL_NO_DISPLAY);

  /* initialize the EGL display connection */
  result = eglInitialize(state->display, NULL, NULL);
  assert(EGL_FALSE != result);

  /* bind OpenVG API */
  eglBindAPI(EGL_OPENVG_API);

  /* get an appropriate EGL frame buffer configuration */
  result = eglChooseConfig(state->display, attribute_list, &config, 1, &num_config);
  assert(EGL_FALSE != result);

  /* create an EGL rendering context*/
  state->context = eglCreateContext(state->display, config, EGL_NO_CONTEXT, NULL);
  assert(state->context!=EGL_NO_CONTEXT);

  /* create an EGL window surface*/
  success = graphics_get_display_size(0 /* LCD */, &state->screen_width, &state->screen_height);
  printf("Width: %d   height: %d\n", &state->screen_width, &state->screen_height);
  assert( success >= 0 );

  dst_rect.x = 0;
  dst_rect.y = 0;
  dst_rect.width = state->screen_width;
  dst_rect.height = state->screen_height;

  src_rect.x = 0;
  src_rect.y = 0;
  src_rect.width = state->screen_width << 16;
  src_rect.height = state->screen_height << 16;

  dispman_display = vc_dispmanx_display_open( 0 /* LCD */);
  dispman_update = vc_dispmanx_update_start( 0 );

  dispman_element = vc_dispmanx_element_add ( dispman_update, dispman_display,
      0/*layer*/, &dst_rect, 0/*src*/,
      &src_rect, DISPMANX_PROTECTION_NONE, 0 /*alpha*/, 0/*clamp*/, 0/*transform*/);

  nativewindow.element = dispman_element;
  nativewindow.width = state->screen_width;
  nativewindow.height = state->screen_height;
  vc_dispmanx_update_submit_sync( dispman_update );

  state->surface = eglCreateWindowSurface( state->display, config, &nativewindow, NULL );
  assert(state->surface != EGL_NO_SURFACE);

  /* connect the context to the surface*/
  result = eglMakeCurrent(state->display, state->surface, state->surface, state->context);
  assert(EGL_FALSE != result);

  /*Set up screen ratio*/
  glViewport(0, 0, (GLsizei)state->screen_width, (GLsizei)state->screen_height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  float ratio = (float)state->screen_width / (float)state->screen_height;
  glFrustumf(-ratio, ratio, -1.0f, 1.0f, 1.0f, 10.0f);
  printf("Screen all set up \n");
}

/*int
main(int argc, char **argv)
{
    STATE_T *state = malloc(sizeof(state));
    memset( state, 0, sizeof( *state ) ); // clear application state
    bcm_egl_openvg_init(state);
    init(state);
    while(1) {
      draw(state);
    }
}*/

