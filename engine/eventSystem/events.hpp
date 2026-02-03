#include "./../scene/sceneStruts.hpp"

enum class EventType{
    MouseButton,
    MouseMove,
    MouseScroll,
    Keypress,
    SpawnEntity
};

struct Event{
    EventType type;
};

//!? vvvvvvvvvvvvvvvvvvvv MOUSE EVENTS vvvvvvvvvvvvvvvvvvvv
// vvvvvvvvvvvvvvvvvvvv Mouse button vvvvvvvvvvvvvvvvvvvv
struct MouseButtonEvent : public Event{
    static constexpr EventType type = EventType::MouseButton;

    int button;
    int action;
    int mods;

    MouseButtonEvent(int b, int a, int m) : Event{EventType::MouseButton}, button(b), action(a), mods(m) {};
};

// vvvvvvvvvvvvvvvvvvvv Mouse move vvvvvvvvvvvvvvvvvvvv
struct MouseMoveEvent : public Event{
    static constexpr EventType type = EventType::MouseMove;

    double xpos;
    double ypos;

    MouseMoveEvent(double x, double y) : Event{EventType::MouseMove}, xpos(x), ypos(y) {};
};

// vvvvvvvvvvvvvvvvvvvv Mouse scroll vvvvvvvvvvvvvvvvvvvv
struct MouseScrollEvent : public Event{
    static constexpr EventType type = EventType::MouseScroll;

    double xoffset;
    double yoffset;

    MouseScrollEvent(double x, double y) : Event{EventType::MouseScroll}, xoffset(x), yoffset(y) {};
};

//!? vvvvvvvvvvvvvvvvvvvv KEY EVENTS vvvvvvvvvvvvvvvvvvvv
struct KeyPressEvent : public Event{
    static constexpr EventType type = EventType::Keypress;

    int key;
    int scancode;
    int action;
    int mods;

    KeyPressEvent(int k, int s, int a, int m) : Event{EventType::Keypress}, key(k), scancode(s), action(a), mods(m) {};
};

//!? vvvvvvvvvvvvvvvvvvvv OBJECT RELATED EVENTS vvvvvvvvvvvvvvvvvvvv
// vvvvvvvvvvvvvvvvvvvv Object spawn vvvvvvvvvvvvvvvvvvvv

struct SpawnEntityEvent : public Event{
    static constexpr EventType type = EventType::SpawnEntity;

    EntityDesc entityDesc;

    SpawnEntityEvent(EntityDesc ed) : Event{EventType::SpawnEntity}, entityDesc(std::move(ed)) {};
};