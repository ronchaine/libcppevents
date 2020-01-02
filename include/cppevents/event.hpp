#ifndef CPPEVENTS_EVENT_HPP
#define CPPEVENTS_EVENT_HPP

#include <cstdlib>
#include <cassert>
#include <cstring>
#include <atomic>
#include <memory>

#include "common.hpp"

namespace cppevents
{
    class event;

    namespace detail
    {
        using data_buffer_type = std::aligned_storage<2 * sizeof(void*), std::alignment_of<void*>::value>;

        inline std::atomic<event_typeid> event_typeid_counter = 0;

        template <typename T> struct internal_event_handler;
        template <typename T> struct external_event_handler;

        enum class handler_action
        {
            destroy,
            move,
            get
        };

        union event_storage
        {
            void* ptr = nullptr;
            data_buffer_type data;
        };

        using handler_ptr = void* (*)(handler_action, event const*, event*);
    }

    template <typename T, typename... Ts>
    event_typeid get_event_id_for()
    {
        static_assert(std::is_same_v<std::decay_t<T>, T>);
        assert(detail::event_typeid_counter < 64);
        static event_typeid ids = 1ul << detail::event_typeid_counter++;

        if constexpr(sizeof...(Ts))
            return ids | get_event_id_for<Ts...>();

        return ids;
    }

    class event
    {
        public:
            template <typename ValueType, typename T = std::decay_t<ValueType>>
            event(ValueType&&);

            event(const event&) = delete;
            event(event&& other) noexcept
            {
                if (other.handler != nullptr)
                    other.handler(detail::handler_action::move, &other, this);
                id = other.id;
            }

            ~event()
            {
                if (handler != nullptr)
                    handler(detail::handler_action::destroy, this, nullptr);
            }

            event_typeid type() const noexcept { return id; }

        private:
            template <typename T> using use_small_object_optimisation =
                std::bool_constant<(sizeof(T) <= sizeof(detail::event_storage)) && (alignof(T) <= alignof(detail::event_storage))>;

            template <typename T> friend struct detail::internal_event_handler;
            template <typename T> friend struct detail::external_event_handler;

            template <typename T> friend T event_cast(event& ev);

            template <typename T>
            using preferred_handler = typename std::conditional<use_small_object_optimisation<T>::value,
                                                                detail::internal_event_handler<T>,
                                                                detail::external_event_handler<T>>::type;

            event_typeid id = 0;

            detail::handler_ptr handler = nullptr;
            detail::event_storage storage;
    };

    template <typename ValueType, typename T>
    event::event(ValueType&& value)
    {
        preferred_handler<T>::create(*this, std::forward<T>(value));
    }

    template <typename T>
    struct detail::internal_event_handler
    {
        template <typename... Arguments>
        static T& create(event& dest, Arguments&&... args)
        {
            T* rval = new (&dest.storage.data) T(std::forward<Arguments>(args)...);
            dest.handler = &internal_event_handler::handle;
            dest.id = get_event_id_for<T>();
            return *rval;
        }

        static void destroy(event& self)
        {
            T& val = *static_cast<T*>(static_cast<void*>(&self.storage.data));
            val.~T();
            self.handler = nullptr;
        }

        static void move(event& self, event& dest)
        {
            create(dest, std::move(*static_cast<T*>(static_cast<void*>(&self.storage.data))));
            destroy(self);
        }

        static void* get(event& self)
        {
            if (self.id == get_event_id_for<T>())
                return static_cast<void*>(&self.storage.data);
            return nullptr;
        }

        static void* handle(handler_action act, event const* self, event* other)
        {
            switch(act)
            {
                case handler_action::destroy:
                    destroy(const_cast<event&>(*self));
                    return nullptr;
                case handler_action::move:
                    move(const_cast<event&>(*self), *other);
                    return nullptr;
                case handler_action::get:
                    return get(const_cast<event&>(*self));
            }
            return nullptr;
        }
    };

    template <typename T>
    struct detail::external_event_handler
    {
        static void* handle(handler_action act, event const* self, event* other)
        {
            switch(act)
            {
                case handler_action::destroy:
                    destroy(const_cast<event&>(*self));
                    return nullptr;
                case handler_action::move:
                    move(const_cast<event&>(*self), *other);
                    return nullptr;
                case handler_action::get:
                    return get(const_cast<event&>(*self));
            }
        }

        template <typename... Arguments>
        static T& create(event& dest, Arguments&&... args)
        {
            dest.storage.ptr = ::new T(std::forward<Arguments>(args)...);
            dest.handler = &external_event_handler::handle;
            dest.id = get_event_id_for<T>();
            return *static_cast<T*>(dest.storage.ptr);
        }

        static void destroy(event& self)
        {
            delete static_cast<T*>(self.storage.ptr);
            self.handler = nullptr;
        }

        static void move(event& self, event& dest)
        {
            dest.storage.ptr = self.storage.ptr;
            dest.handler = &external_event_handler::handle;
            dest.id = self.id;
            self.handler = nullptr;
        }

        static void* get(event& self)
        {
            if (self.id == get_event_id_for<T>())
                return self.storage.ptr;
            return nullptr;
        }
    };

    template <typename T>
    T event_cast(event& ev)
    {
        using raw_type = typename std::remove_cvref<T>::type;

        raw_type* ptr = reinterpret_cast<raw_type*>(event::preferred_handler<raw_type>::handle(detail::handler_action::get, &ev, nullptr));
        assert(ptr != nullptr);

        return *ptr;
    }
}

#endif
