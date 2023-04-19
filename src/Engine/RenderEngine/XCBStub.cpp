#include "Utilities/log.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <xcb/xcb.h>
#include "Color.hpp"

xcb_connection_t *c;
xcb_screen_t     *screen;
xcb_window_t      win;

static int xcbmain ()
{
  /* Open the connection to the X server */
  c = xcb_connect (NULL, NULL);

  /* Get the first screen */
  screen = xcb_setup_roots_iterator (xcb_get_setup (c)).data;

  /* Ask for our window's Id */
  win = xcb_generate_id(c);

  /* Create the window */
  xcb_create_window (c,                             /* Connection          */
                     XCB_COPY_FROM_PARENT,          /* depth (same as root)*/
                     win,                           /* window Id           */
                     screen->root,                  /* parent window       */
                     0, 0,                          /* x, y                */
                     150, 150,                      /* width, height       */
                     10,                            /* border_width        */
                     XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
                     screen->root_visual,           /* visual              */
                     0, NULL);                      /* masks, not used yet */

  /* Map the window on the screen */
  xcb_map_window (c, win);

  /* Make sure commands are sent before we pause, so window is shown */
  xcb_flush (c);

  return 0;
}

int main() {
    xppr::log::init_logger();
    xcbmain();

    while(1) {
        xcb_flush(c);
    }
}