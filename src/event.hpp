#pragma once

namespace pubsub {
    enum class EventType{
        Created,
        Deleted,
        Updated,
    };

    template <typename T>

    struct Event{
        EventType type;
        T payload;
    };
}