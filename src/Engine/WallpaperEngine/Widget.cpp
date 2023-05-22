#include "Widget.hpp"

xppr::WidgetBase::~WidgetBase() {} // Here to make vtable strong. (-Wweak-vtables)

const char* xppr::WidgetBase::getName() const { return nullptr; }
