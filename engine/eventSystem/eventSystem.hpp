#pragma once

#include <memory>
#include <vector>
#include <functional>

#include "./events.hpp"

class EventQueue{
    public:
        // template<typename E>
        // void push(E e){
        //     events.emplace_back(std::make_unique<E>(std::forward<E>(e)));
        // }

        /**
         * The function only took:
         *      > emit(KeyPressEvent{key, scancode, action, mods});
         * now, the new one below can also take:
         *      > emit<KeyPressEvent>(key, scancode, action, mods);
         * which imo is nicer to use (and afaik also better in general).
         * 
         * Leaving the old function above commented out for now.
         */
        template<typename E, typename... Args>
        void push(Args&&... args) {
            events.emplace_back(
                std::make_unique<E>(std::forward<Args>(args)...)
            );
        }

        std::vector<std::unique_ptr<Event>> events;

    private:
};

using EventCallback = std::function<void(const Event&)>;

class EventBus{
    public:
        void subscribe(EventType type, EventCallback cb){
            listeners[type].push_back(cb);
        }

        void dispatch(const Event& e){
            auto it = listeners.find(e.type);
            if(it == listeners.end()) return;

            for(auto& cb : it->second){
                cb(e);
            }
        }

    private:
        std::unordered_map<EventType, std::vector<EventCallback>> listeners;
};

class EventSystem{
    public:
        void process(){
            for(auto& e : queue.events){
                bus.dispatch(*e);
            }
            queue.events.clear();
        }

        /**
         * constructor-style emit
         * Usage example:
         * > emit<KeyPressEvent>(key, scancode, action, mods);
         */
        template<typename E, typename... Args>
        void emit(Args&&... args) {
            queue.push<E>(std::forward<Args>(args)...);
        }

        /**
         * object-style emit
         * Usage example:
         * > emit(KeyPressEvent{key, scancode, action, mods});
         */
        template<typename E>
        void emit(E&& e) {
            queue.push<std::decay_t<E>>(std::forward<E>(e));
        }

        template<typename E>
        void subscribe(std::function<void(const E&)> cb){
            bus.subscribe(E::type, [cb = std::move(cb)](const Event& e){
                cb(static_cast<const E&>(e));
            });
        }

    private:
        EventQueue queue;
        EventBus bus;
};