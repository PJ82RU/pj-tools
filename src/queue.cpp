#include "queue.h"

namespace tools
{
    Queue::Queue(const UBaseType_t queue_length, const UBaseType_t item_size)
    {
        handle = xQueueCreate(queue_length, item_size);
    }

    Queue::~Queue()
    {
        vQueueDelete(handle);
    }

    unsigned int Queue::messages_waiting() const
    {
        return uxQueueMessagesWaiting(handle);
    }

    unsigned int Queue::spaces_available() const
    {
        return uxQueueSpacesAvailable(handle);
    }

    bool Queue::send(const void* p_item_to_queue, const TickType_t ticks_to_wait) const
    {
        return xQueueSend(handle, p_item_to_queue, ticks_to_wait) == pdTRUE;
    }

    bool Queue::overwrite(const void* p_item_to_queue) const
    {
        return xQueueOverwrite(handle, p_item_to_queue) == pdTRUE;
    }

    bool Queue::receive(void* p_buffer, const TickType_t ticks_to_wait) const
    {
        return xQueueReceive(handle, p_buffer, ticks_to_wait) == pdTRUE;
    }

    void Queue::reset() const
    {
        xQueueReset(handle);
    }
}
