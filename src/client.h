#ifndef _CLIENT_H_
#define _CLIENT_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "UdpLib.h"
#include "InputUtil.h"
#include "SimLib.h"

#ifdef _HAS_EGL
#include "display.h"
#endif

void main_client(void);

#endif
