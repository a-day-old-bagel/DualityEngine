//
// Created by adayoldbagel on 2/1/16.
//

#ifndef DUALITYENGINE_EVENT_H
#define DUALITYENGINE_EVENT_H

#include "concurrentqueue.h"
#include "Delegate.h"

namespace DualityEngine {
    enum ActionType {
        VOIDVOID,
        VOIDCSTRING,
        VOIDSTRING,
    };
    struct Action {
        ActionType type;
        void* arguments;
        union {
            Delegate<void()> voidVoid;
            Delegate<void(const char*)> voidCstring;
            Delegate<void(const std::string)> voidString;
        };
        Action();
    };
    struct Event {
        Action action;
        Event();
    };
    class EventQueue {
        moodycamel::ConcurrentQueue<Event> events;
        moodycamel::ConsumerToken queueToken;
        bool isDead;
    public:
        EventQueue();
        void newEvent(moodycamel::ProducerToken token, Event event);
        void handleEvents();
        bool isAlive();
        void kill();
    };
}

#endif //DUALITYENGINE_EVENT_H
