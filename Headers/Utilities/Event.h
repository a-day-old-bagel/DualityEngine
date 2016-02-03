//
// Created by adayoldbagel on 2/1/16.
//

#ifndef DUALITYENGINE_EVENT_H
#define DUALITYENGINE_EVENT_H

#include "concurrentqueue.h"
#include "Delegate.h"

namespace DualityEngine {
    typedef moodycamel::ProducerToken Token;
    struct Event {
        enum {
            VOIDVOID,
            VOIDCSTRING,
            VOIDSTRING,
        } type;
        union {
            Delegate<void()> voidVoid;
            Delegate<void(const char*)> voidCstring;
            Delegate<void(const std::string)> voidString;
        };
        Event();
    };
    class EventQueue {
        moodycamel::ConcurrentQueue<Event> events;
        moodycamel::ConsumerToken queueToken;
    public:
        EventQueue();
        moodycamel::ProducerToken requestProducerToken();
        void newEvent(moodycamel::ProducerToken& token, Event& event);
        bool getNext(Event& event);
    };
}

#endif //DUALITYENGINE_EVENT_H
