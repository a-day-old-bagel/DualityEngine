//
// Created by adayoldbagel on 2/1/16.
//

#ifndef DUALITYENGINE_EVENT_H
#define DUALITYENGINE_EVENT_H

#include "concurrentqueue.h"
//#include "Delegate.h"

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
//            Delegate<void()> voidVoid;
//            Delegate<void(const char*)> voidCstring;
//            Delegate<void(const std::string)> voidString;
        };
        Action() {}
    };
    struct Event {
        Action action;
        Event() {}
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

    EventQueue::EventQueue() : isDead(false), queueToken(events){

    }

    void EventQueue::newEvent(moodycamel::ProducerToken token, Event event) {
        events.enqueue(token, event);
    }

    void EventQueue::handleEvents() {
        Event currentEvent;
        bool thereAreMoreEvents = events.try_dequeue(queueToken, currentEvent);
        while (thereAreMoreEvents) {
            switch(currentEvent.action.type) {
                case VOIDVOID:
//                    currentEvent.action.voidVoid();
                    break;
                case VOIDCSTRING:
//                    currentEvent.action.voidCstring("C-String Event");
                    break;
                case VOIDSTRING:
//                    currentEvent.action.voidString("String Event");
                    break;

                default:
                    break;
            }
            thereAreMoreEvents = events.try_dequeue(queueToken, currentEvent);
        }
    }

    bool EventQueue::isAlive() {
        return !isDead;
    }

    void EventQueue::kill() {
        isDead = true;
        handleEvents();
    }
}

#endif //DUALITYENGINE_EVENT_H
