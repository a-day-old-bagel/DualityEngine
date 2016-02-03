//
// Created by adayoldbagel on 2/2/16.
//

#include "Event.h"

namespace DualityEngine {

    Event::Event() {

    }

    EventQueue::EventQueue() : queueToken(events) {

    }

    moodycamel::ProducerToken EventQueue::requestProducerToken() {
        return moodycamel::ProducerToken(events);
    }

    void EventQueue::newEvent(moodycamel::ProducerToken& token, Event& event) {
        events.enqueue(token, event);
    }

    bool EventQueue::getNext(Event &event) {
        return events.try_dequeue(queueToken, event);
    }
}

