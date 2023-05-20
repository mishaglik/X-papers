#ifndef PLUGINS_WIDGET_BASEWIDGETS_SPRITEWIDGET_HPP
#define PLUGINS_WIDGET_BASEWIDGETS_SPRITEWIDGET_HPP

#include <Engine/WallpaperEngine/Widget.hpp>
#include "Engine/WallpaperEngine/MetaTypes.hpp"

template<class T>
concept SpriteWidget = std::derived_from<T, xppr::meta::MetaObject> && 
requires (T t, const T& tc, xppr::RenderTexture& texture, uint64_t time)
{
    tc.drawOn(texture);
    t.update(time);
    T::m_meths;
};

template <SpriteWidget T, typename C>
auto operator|(T f, C c) {
  return c.Pipe(std::move(f));
}

#endif /* PLUGINS_WIDGET_BASEWIDGETS_SPRITEWIDGET_HPP */
