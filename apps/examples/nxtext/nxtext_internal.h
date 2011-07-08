/****************************************************************************
 * examples/nxtext/nxtext_internal.h
 *
 *   Copyright (C) 2011 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <spudmonkey@racsa.co.cr>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __EXAMPLES_NXTEXT_NXTEXT_INTERNAL_H
#define __EXAMPLES_NXTEXT_NXTEXT_INTERNAL_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <nuttx/nx.h>
#include <nuttx/nxtk.h>

/****************************************************************************
 * Definitions
 ****************************************************************************/

/* Configuration ************************************************************/

#ifndef CONFIG_NX
#  error "NX is not enabled (CONFIG_NX)"
#endif

#ifndef CONFIG_EXAMPLES_NXTEXT_VPLANE
#    define CONFIG_EXAMPLES_NXTEXT_VPLANE 0
#endif

#ifndef CONFIG_EXAMPLES_NXTEXT_BPP
#  define CONFIG_EXAMPLES_NXTEXT_BPP 32
#endif

#ifndef CONFIG_EXAMPLES_NXTEXT_BGCOLOR
#  if CONFIG_EXAMPLES_NXTEXT_BPP == 24 || CONFIG_EXAMPLES_NXTEXT_BPP == 32
#    define CONFIG_EXAMPLES_NXTEXT_BGCOLOR 0x007b68ee
#  elif CONFIG_EXAMPLES_NXTEXT_BPP == 16
#    define CONFIG_EXAMPLES_NXTEXT_BGCOLOR 0x7b5d
#  else
#    define CONFIG_EXAMPLES_NXTEXT_BGCOLOR ' '
# endif
#endif

#ifndef CONFIG_EXAMPLES_NXTEXT_PUCOLOR
#  if CONFIG_EXAMPLES_NXTEXT_BPP == 24 || CONFIG_EXAMPLES_NXTEXT_BPP == 32
#    define CONFIG_EXAMPLES_NXTEXT_PUCOLOR 0x00dcdcdc
#  elif CONFIG_EXAMPLES_NXTEXT_BPP == 16
#    define CONFIG_EXAMPLES_NXTEXT_PUCOLOR 0xdefb
#  else
#    define CONFIG_EXAMPLES_NXTEXT_PUCOLOR '2'
# endif
#endif

#ifndef CONFIG_EXAMPLES_NXTEXT_BGFONTCOLOR
#  if CONFIG_EXAMPLES_NXTEXT_BPP == 24 || CONFIG_EXAMPLES_NXTEXT_BPP == 32
#    define CONFIG_EXAMPLES_NXTEXT_BGFONTCOLOR 0x00000000
#  elif CONFIG_EXAMPLES_NXTEXT_BPP == 16
#    define CONFIG_EXAMPLES_NXTEXT_BGFONTCOLOR 0x0000
#  else
#    define CONFIG_EXAMPLES_NXTEXT_BGFONTCOLOR 'F'
#  endif
#endif

#ifndef CONFIG_EXAMPLES_NXTEXT_PUFONTCOLOR
#  if CONFIG_EXAMPLES_NXTEXT_BPP == 24 || CONFIG_EXAMPLES_NXTEXT_BPP == 32
#    define CONFIG_EXAMPLES_NXTEXT_PUFONTCOLOR 0x00000000
#  elif CONFIG_EXAMPLES_NXTEXT_BPP == 16
#    define CONFIG_EXAMPLES_NXTEXT_PUFONTCOLOR 0x0000
#  else
#    define CONFIG_EXAMPLES_NXTEXT_PUFONTCOLOR 'F'
#  endif
#endif

#ifndef CONFIG_EXAMPLES_NXTEXT_BMCACHE
#  define CONFIG_EXAMPLES_NXTEXT_BMCACHE 128
#endif

#ifndef CONFIG_EXAMPLES_NXTEXT_GLCACHE
#  if CONFIG_NXFONTS_CHARBITS < 8
#    define CONFIG_EXAMPLES_NXTEXT_GLCACHE MIN(CONFIG_EXAMPLES_NXTEXT_BMCACHE, 128)
#  else
#    define CONFIG_EXAMPLES_NXTEXT_GLCACHE MIN(CONFIG_EXAMPLES_NXTEXT_BMCACHE, 256)
#  endif
#endif

#ifdef CONFIG_NX_MULTIUSER
#  ifdef CONFIG_DISABLE_MQUEUE
#    error "The multi-threaded example requires MQ support (CONFIG_DISABLE_MQUEUE=n)"
#  endif
#  ifdef CONFIG_DISABLE_SIGNALS
#    error "This example requires signal support (CONFIG_DISABLE_SIGNALS=n)"
#  endif
#  ifdef CONFIG_DISABLE_PTHREAD
#    error "This example requires pthread support (CONFIG_DISABLE_PTHREAD=n)"
#  endif
#  ifndef CONFIG_NX_BLOCKING
#    error "This example depends on CONFIG_NX_BLOCKING"
#  endif
#  ifndef CONFIG_EXAMPLES_NXTEXT_STACKSIZE
#    define CONFIG_EXAMPLES_NXTEXT_STACKSIZE 2048
#  endif
#  ifndef CONFIG_EXAMPLES_NXTEXT_LISTENERPRIO
#    define CONFIG_EXAMPLES_NXTEXT_LISTENERPRIO 100
#  endif
#  ifndef CONFIG_EXAMPLES_NXTEXT_CLIENTPRIO
#    define CONFIG_EXAMPLES_NXTEXT_CLIENTPRIO 100
#  endif
#  ifndef CONFIG_EXAMPLES_NXTEXT_SERVERPRIO
#    define CONFIG_EXAMPLES_NXTEXT_SERVERPRIO 120
#  endif
#  ifndef CONFIG_EXAMPLES_NXTEXT_NOTIFYSIGNO
#    define CONFIG_EXAMPLES_NXTEXT_NOTIFYSIGNO 4
#  endif
#endif

/* Debug ********************************************************************/

#ifdef CONFIG_CPP_HAVE_VARARGS
#  ifdef CONFIG_DEBUG
#    define message(...) lib_lowprintf(__VA_ARGS__)
#    define msgflush()
#  else
#    define message(...) printf(__VA_ARGS__)
#    define msgflush() fflush(stdout)
#  endif
#else
#  ifdef CONFIG_DEBUG
#    define message lib_lowprintf
#    define msgflush()
#  else
#    define message printf
#    define msgflush() fflush(stdout)
#  endif
#endif

/****************************************************************************
 * Public Types
 ****************************************************************************/

enum exitcode_e
{
  NXEXIT_SUCCESS = 0,
  NXEXIT_SCHEDSETPARAM,
  NXEXIT_TASKCREATE,
  NXEXIT_PTHREADCREATE,
  NXEXIT_EXTINITIALIZE,
  NXEXIT_FBINITIALIZE,
  NXEXIT_FBGETVPLANE,
  NXEXIT_LCDINITIALIZE,
  NXEXIT_LCDGETDEV,
  NXEXIT_NXOPEN,
  NXEXIT_NXREQUESTBKGD,
  NXEXIT_NXCONNECT,
  NXEXIT_NXSETBGCOLOR,
  NXEXIT_NXOPENWINDOW,
  NXEXIT_NXSETSIZE,
  NXEXIT_NXSETPOSITION,
  NXEXIT_NXCLOSEWINDOW,
  NXEXIT_LOSTSERVERCONN
};

/* Describes one cached glyph bitmap */

struct nxtext_glyph_s
{
  uint8_t code;                        /* Character code */
  uint8_t height;                      /* Height of this glyph (in rows) */
  uint8_t width;                       /* Width of this glyph (in pixels) */
  uint8_t stride;                      /* Width of the glyph row (in bytes) */
  FAR uint8_t *bitmap;                 /* Allocated bitmap memory */
};

/* Describes on character on the display */

struct nxtext_bitmap_s
{
  struct nxgl_rect_s bounds;              /* Size/position of bitmap */
  FAR const struct nxtext_glyph_s *glyph; /* The cached glyph */
};

/* Describes the state of one text display */

struct nxtext_state_s
{
  /* The following describe the window */

  nxgl_mxpixel_t wcolor[CONFIG_NX_NPLANES]; /* Window color */
  struct nxgl_size_s wsize;                 /* Window size */

  /* These characterize the font in use */

  nxgl_mxpixel_t fcolor[CONFIG_NX_NPLANES]; /* Font color */
  uint8_t fheight;                          /* Max height of a font in pixels */
  uint8_t fwidth;                           /* Max width of a font in pixels */
  uint8_t spwidth;                          /* The width of a space */

  /* This is the next display position */

  struct nxgl_point_s pos;                  /* Next display position */

  /* These describe all text already added to the display */

  uint16_t maxchars;                        /* Size of the mb array */
  uint8_t maxglyphs;                        /* Size of the glyph array */
  uint8_t nchars;                           /* Number of chars already displayed */
  uint8_t nglyphs;                          /* Number of glyphs cached */

  FAR struct nxtext_bitmap_s *bm;           /* List of characters on the display */
  FAR struct nxtext_glyph_s  *glyph;        /* Cache of rendered fonts in use */
};

/****************************************************************************
 * Public Variables
 ****************************************************************************/

/* The connecton handler */

extern NXHANDLE g_hnx;

/* NX callback vtables */

extern const struct nx_callback_s g_bgcb;

/* The screen resolution */

nxgl_coord_t g_xres;
nxgl_coord_t g_yres;

extern bool b_haveresolution;
#ifdef CONFIG_NX_MULTIUSER
extern bool g_connected;
#endif
extern sem_t g_semevent;

extern int g_exitcode;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef CONFIG_EXAMPLES_NXTEXT_EXTERNINIT
extern FAR NX_DRIVERTYPE *up_nxdrvinit(unsigned int devno);
#endif
#if defined(CONFIG_NX) && defined(CONFIG_NX_MULTIUSER)
extern int nxtext_server(int argc, char *argv[]);
extern FAR void *nxtext_listener(FAR void *arg);
#endif

/* Background window interfaces */

extern FAR struct nxtext_state_s *nxbg_getstate(void);
extern void nxbg_write(NXWINDOW hwnd, FAR const uint8_t *buffer, size_t buflen);

/* Pop-up window interfaces */

extern NXWINDOW nxpu_open(void);
extern int nxpu_close(NXWINDOW hwnd);

/* Generic text helpers */

extern void nxtext_home(FAR struct nxtext_state_s *st);
extern void nxtext_newline(FAR struct nxtext_state_s *st);
extern void nxtext_putc(NXWINDOW hwnd, FAR struct nxtext_state_s *st,
                        uint8_t ch);
extern void nxtext_fillchar(NXWINDOW hwnd, FAR const struct nxgl_rect_s *rect,
                            FAR const struct nxtext_bitmap_s *bm);

#endif /* __EXAMPLES_NXTEXT_NXTEXT_INTERNAL_H */