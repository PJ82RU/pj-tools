#include "queue.h"
#include <stdexcept>

namespace tools
{
    Queue::Queue(const UBaseType_t queue_length, const UBaseType_t item_size)
    {
        handle_ = xQueueCreate(queue_length, item_size);
        if (handle_ == nullptr)
        {
            throw std::runtime_error("Failed to create queue");
        }
    }

    Queue::~Queue() noexcept
    {
        cleanup();
    }

    Queue::Queue(Queue&& other) noexcept :
        handle_(other.handle_)
    {
    }

    Queue& Queue::operator=(Queue&& other) noexcept
    {
        if (this != &other)
        {
            cleanup();
            handle_ = other.handle_;
        }
        return *this;
    }

    void Queue::cleanup() noexcept
    {
        if (handle_)
        {
            vQueueDelete(handle_);
            handle_ = nullptr;
        }
    }

    UBaseType_t Queue::messages_waiting() const noexcept
    {
        return handle_ ? uxQueueMessagesWaiting(handle_) : 0;
    }

    UBaseType_t Queue::spaces_available() const noexcept
    {
        return handle_ ? uxQueueSpacesAvailable(handle_) : 0;
    }

    bool Queue::send(const void* item, const TickType_t ticks_to_wait) const noexcept
    {
        return handle_ && (xQueueSend(handle_, item, ticks_to_wait) == pdTRUE);
    }

    bool Queue::overwrite(const void* item) const noexcept
    {
        return handle_ && (xQueueOverwrite(handle_, item) == pdTRUE);
    }

    bool Queue::receive(void* buffer, const TickType_t ticks_to_wait) const noexcept
    {
        return handle_ && (xQueueReceive(handle_, buffer, ticks_to_wait) == pdTRUE);
    }

    void Queue::reset() const noexcept
    {
        if (handle_)
        {
            xQueueReset(handle_);
        }
    }
}
