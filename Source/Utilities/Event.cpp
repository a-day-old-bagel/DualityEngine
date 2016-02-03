//
// Created by adayoldbagel on 2/2/16.
//

#include "Event.h"

namespace DualityEngine {

    Action::Action() {

    }

    Event::Event() {

    }

    EventQueue::EventQueue() : queueToken(events), isDead(false){

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
                    currentEvent.action.voidVoid();
                    break;
                case VOIDCSTRING:
                    currentEvent.action.voidCstring("C-String Event");
                    break;
                case VOIDSTRING:
                    currentEvent.action.voidString("String Event");
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

