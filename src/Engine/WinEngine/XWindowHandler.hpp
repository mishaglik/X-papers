#ifndef X11_WINDOW_HANDLER_HPP
#define X11_WINDOW_HANDLER_HPP

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include "proto.hpp"
namespace winengine {

class XDisplayHandler;

class XWindowHandler {
    friend class XDisplayHandler;

   private:
    XWindowHandler(XDisplayHandler* manager,
                   display_t* display,
                   pair_t coords,
                   pair_t size,
                   int border_width = 0,
                   class_t class_type = class_t::Parent,
                   visual_t* visual = nullptr,
                   uint32_t mask = 0,
                   win_attr_t* attributes = nullptr);

    XWindowHandler& operator=(const XWindowHandler&) = delete;
    XWindowHandler& operator=(XWindowHandler&&) = delete;

    ~XWindowHandler() = default;

   public:
    int show();

    int hide();

    int changeProperty(atom_t property,
                       atom_t type,
                       int format,
                       int mode,
                       const uint8_t* data,
                       int nelements);

    int setBackground(uint64_t color);

    void removeTakeFocusProtocol();

    int clear();

    int setHints(wm_hints_t* hints);

    const win_t& getInternalHandler() const { return m_win_id; }

   private:
    win_t m_win_id = 0;
    XDisplayHandler* m_manager = nullptr;
};

}  // namespace winengine

#endif  // !X11_WINDOW_HANDLER_HPP
