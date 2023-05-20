#ifndef ENGINE_WALLPAPERENGINE_EVENT_HPP
#define ENGINE_WALLPAPERENGINE_EVENT_HPP

#include <concepts>
#include <cstdint>
#include <Engine/WallpaperEngine/MetaTypes.hpp>
#include <typeinfo>
#include "Utilities/optional.hpp"

namespace xppr {

extern const meta::MetaType EventMetaType;

class EventBase : public meta::MetaObjectT<&EventMetaType> {
  public:
  EventBase() : MetaObjectT() {}
};

extern const meta::MetaType EventMemberDesciptorMetaType;

struct EventMemberDesciptorBase : meta::MetaObjectT<&EventMemberDesciptorMetaType> {
  explicit EventMemberDesciptorBase(size_t type_hash) : meta::MetaObjectT<&EventMemberDesciptorMetaType>(), m_type_hash(type_hash) {}
  size_t m_type_hash;
};

template<typename T>
struct EventMemberDesciptor : EventMemberDesciptorBase {
  enum Error : int {
    Ok,
    BadObject,
    BadType,
    Null,
  };

  EventMemberDesciptor(T EventBase::* member = nullptr) : EventMemberDesciptorBase(typeid(T).hash_code()), m_member(member) {}

  Optional<T, Error> Get(meta::MetaObject* object) {
    if(m_member == nullptr) {
      return Error::Null;
    }

    if(object->m_type != &EventMetaType) {
      return Error::BadObject;
    }
    return static_cast<EventBase*>(object)->*m_member;
  }

  static EventMemberDesciptor* FromObject(meta::MetaObject* object) {
    if(object->m_type != &EventMemberDesciptorMetaType) {
      return nullptr;
    }

    if(typeid(T).hash_code() != static_cast<EventMemberDesciptorBase*>(object)->m_type_hash) {
      return nullptr;
    }
    return static_cast<EventMemberDesciptor*>(object);
  }

  T EventBase::* m_member;
};
}

#endif /* ENGINE_WALLPAPERENGINE_EVENT_HPP */
