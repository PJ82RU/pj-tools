#include "callback.h"
#include <esp_log.h>
#include <cstring>

namespace tools
{
    Callback::Callback(const uint8_t buffer_size, const size_t item_size, const char* name, const uint32_t stack_depth,
                       const UBaseType_t priority) :
        thread_(name, stack_depth, priority),
        queue_(buffer_size, sizeof(BufferItem)),
        semaphore_(false)
    {
        if (buffer_size > 0 && item_size > 0)
        {
            buffer_size_ = buffer_size;
            item_size_ = item_size;
            buffer_ = new(std::nothrow) uint8_t[buffer_size_ * item_size_];

            if (buffer_)
            {
                ESP_LOGI("Callback", "Initialized with buffer %dx%d", buffer_size_, item_size_);
                return;
            }
        }
        ESP_LOGE("Callback", "Invalid initialization parameters");
    }

    Callback::~Callback()
    {
        thread_.stop();
        delete[] items_;
        delete[] buffer_;
    }

    bool Callback::init(const uint8_t num_callbacks) noexcept
    {
        if (buffer_ && num_items_ == 0 && num_callbacks > 0)
        {
            items_ = new(std::nothrow) Item[num_callbacks];
            if (items_)
            {
                num_items_ = num_callbacks;
                free();
                return thread_.start(&Callback::callback_task, this);
            }
        }
        return false;
    }

    bool Callback::is_initialized() const noexcept
    {
        return buffer_ && items_;
    }

    int16_t Callback::add_callback(const EventSendFunc func, void* params, const bool only_index) const noexcept
    {
        if (!is_initialized() || !func) return -1;

        if (semaphore_.take())
        {
            for (int16_t i = 0; i < num_items_; ++i)
            {
                if (!items_[i].func)
                {
                    items_[i] = {only_index, func, params};
                    (void)semaphore_.give();
                    ESP_LOGD("Callback", "Added callback at index %d", i);
                    return i;
                }
            }
            (void)semaphore_.give();
        }

        ESP_LOGW("Callback", "No free slots for callback");
        return -1;
    }

    void Callback::free() const noexcept
    {
        if (is_initialized() && semaphore_.take())
        {
            std::memset(items_, 0, sizeof(Item) * num_items_);
            (void)semaphore_.give();
            ESP_LOGD("Callback", "Cleared all callbacks");
        }
    }

    void Callback::invoke(const void* value, const int16_t index) noexcept
    {
        if (!buffer_ || !value || !semaphore_.take()) return;

        std::memcpy(&buffer_[current_buffer_index_ * item_size_], value, item_size_);

        const BufferItem item{index, current_buffer_index_};
        current_buffer_index_ = (current_buffer_index_ + 1) % buffer_size_;

        (void)queue_.send(&item);
        (void)semaphore_.give();
    }

    bool Callback::read(void* value) const noexcept
    {
        if (!buffer_ || !value || !semaphore_.take()) return false;

        BufferItem item{};
        const bool result = queue_.receive(&item, 0);
        if (result)
        {
            std::memcpy(value, &buffer_[item.buffer_index * item_size_], item_size_);
        }

        (void)semaphore_.give();
        return result;
    }

    void Callback::callback_task(void* arg) noexcept
    {
        const auto* cb = static_cast<Callback*>(arg);
        if (cb)
        {
            cb->run();
        }
    }

    void Callback::process_items(const BufferItem& item) const noexcept
    {
        const size_t pos = item.buffer_index * item_size_;

        if (semaphore_.take())
        {
            for (uint8_t i = 0; i < num_items_; ++i)
            {
                const Item& it = items_[i];
                if (it.func && (!it.only_index || it.only_index && item.item_index == i))
                {
                    if (it.func(&buffer_[pos], it.params))
                    {
                        parent_callback.invoke(&buffer_[pos]);
                    }
                }
            }
            (void)semaphore_.give();
        }
    }

    void Callback::run() const noexcept
    {
        BufferItem item{};
        while (queue_.receive(&item, portMAX_DELAY))
        {
            process_items(item);
        }
    }
}
