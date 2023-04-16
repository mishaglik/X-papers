#ifndef X11_HANDLER_HPP
#define X11_HANDLER_HPP

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
/* WARNING: ORDER IS IMPORTANT */
#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xutil.h>

#include <cstdio>
#include <cstdlib>
#include <string>

#include "XEventHandler.hpp"

namespace winengine {

/* Is not supposed to be used in user-code */
class XStorage {
protected:
  Display *m_display{nullptr};
  int32_t m_screen{0};
  Window m_window;
  GC m_gc;
};

/* Stores important X variables */
class XHandler : XStorage {
public:
  constexpr XHandler() = default;

  /* non-copyable */
  explicit constexpr XHandler(const XHandler &) = delete;
  XHandler &operator=(const XHandler &) = delete;

  /* movable */
  XHandler(XHandler &&other) {
    m_display = other.m_display;
    other.m_display = nullptr;

    m_screen = other.m_screen;
    other.m_screen = 0; // TODO: make poisonous

    m_window = other.m_window;
    other.m_window = Window(); // default constructor of unsigned long

    m_gc = other.m_gc;
    // do not invalidate other.m_gc, it's not harmful
  }

  Display *getDisplay() { return m_display; }

  const int32_t getScreen() const { return m_screen; }

  Window getWindow() { return m_window; }

  GC getGC() { return m_gc; }

  //----

  Display *openDisplay(const char *display = nullptr) {
    m_display = XOpenDisplay(display);

    return m_display;
  }

  int32_t defaultScreen(const Display *display = nullptr) {
    if (display == nullptr) {
      m_screen = DefaultScreen(m_display);
    } else {
      m_screen = DefaultScreen(display);
    }

    return m_screen;
  }

  Window createWindow(int x, int y, unsigned int width, unsigned int height,
                      unsigned int border_width, unsigned long border,
                      unsigned long background, Display *display = nullptr,
                      Window win = 0) {
    if (display == nullptr) {
      display = m_display;
    }
    if (win == 0) {
      win = m_window;
    }

    return XCreateSimpleWindow(display, win, x, y, width, height, border_width,
                               border, background);
  }

  int nextEvent() {
    return XNextEvent(m_display, m_event_handler.getInstance());
  }

  int getEventType() const { return m_event_handler.getType(); }

  int lookupString(std::string &text) {
    // TODO: add xcompose status
    return XLookupString(&m_event_handler.getInstance()->xkey, text.data(), 255,
                         &m_key, 0);
  }

  int setForeground(unsigned long fg) {
    return XSetForeground(m_display, m_gc, fg);
  }

  int drawString(int x, int y, std::string &text) {
    return XDrawString(m_display, m_window, m_gc, x, y, text.data(),
                       text.size());
  }

  int setProperties(Display *display, Window w, _Xconst char *window_name,
                    _Xconst char *icon_name, Pixmap icon_pixmap, char **argv,
                    int argc, XSizeHints *hints) {
    return XSetStandardProperties(display, w, window_name, icon_name, icon_pixmap, argv, argc, hints);
  }

private:
  XEventHandler m_event_handler;
  KeySym m_key{0};
};

} // namespace winengine

#endif // !X11_HANDLER_HPP