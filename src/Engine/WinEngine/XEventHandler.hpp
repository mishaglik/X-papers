#ifndef X11_EVENT_HANDLER_HPP
#define X11_EVENT_HANDLER_HPP

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <cstdio>
#include <cstdlib>

namespace winengine {

class XEventHandler {
   public:
    explicit constexpr XEventHandler() {
        m_event = getInstance();
    }

    ~XEventHandler() {
        if (m_event == nullptr) {
            delete m_event;
        }
    }

    /* non-copyable */
    explicit constexpr XEventHandler(const XEventHandler&) = delete;
    XEventHandler& operator=(const XEventHandler&) = delete;

    /* non-movable */
    XEventHandler(XEventHandler&& other) = delete;

    // TODO: return type must be std::optional<T, error_type>
    XEvent* getInstance() {
        if (m_event == nullptr) {
            m_event = new XEvent;
        }

        return m_event;
    }

    int32_t getType() const {
        return m_event->type;
    }



   private:
    XEvent* m_event;
};

}  // namespace winengine

#endif  // !X11_EVENT_HANDLER_HPP