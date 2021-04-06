#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <errno.h>
#include <math.h>
#include <pspctrl.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <psputils.h>
#include <psptypes.h>
#include <psppower.h>
#include <pspiofilemgr.h>
#include <psprtc.h>

#include <pspdisplay.h>
#include <pspgu.h>
#include <pspgum.h>
#include <jpeglib.h>
#include <png.h>
#include <zlib.h>

#include <pspaudiolib.h>
#include <pspaudio.h>
#include <mad.h>
#include <mikmod.h>
#include <tremor/ivorbiscodec.h>
#include <tremor/ivorbisfile.h>

//Some common definitions
#ifndef MAX_PATH
#define MAX_PATH 256
#endif

#define DATA_ZIP "data.zip"

#include "debug.h"
#include "fontloader.h"
#include "graphics.h"
#include "framebuffer.h"

#include "oggplayer.h"
#include "mikmodplayer.h"
#include "mp3.h"

#include "../zip/unzip.h"
#include "../zip/zfile.h"

#include "system.h"
#include "common.h"
#include "zip.h"
#include "controller.h"


int exit_callback();

int i;
char EBOOTPath[MAX_PATH];
