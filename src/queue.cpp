#include "queue.h"

namespace tools {
    Queue::Queue(UBaseType_t queue_length, UBaseType_t item_size) {
        handle = xQueueCreate(queue_length, item_size);
    }

    Queue::~Queue() {
        vQueueDelete(handle);
    }

    unsigned int Queue::messages_waiting() {
        return uxQueueMessagesWaiting(handle);
    }

    unsigned int Queue::spaces_available() {
        return uxQueueSpacesAvailable(handle);
    }

    bool Queue::send(const void *p_item_to_queue, TickType_t ticks_to_wait) {
        return xQueueSend(handle, p_item_to_queue, ticks_to_wait) == pdTRUE;
    }

    bool Queue::overwrite(const void *p_item_to_queue) {
        return xQueueOverwrite(handle, p_item_to_queue) == pdTRUE;
    }

    bool Queue::receive(void *p_buffer, TickType_t ticks_to_wait) {
        return xQueueReceive(handle, p_buffer, ticks_to_wait) == pdTRUE;
    }

    void Queue::reset() {
        xQueueReset(handle);
    }
}