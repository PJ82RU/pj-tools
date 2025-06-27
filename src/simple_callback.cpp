#include "simple_callback.h"

namespace tools
{
    SimpleCallback::SimpleCallback(const CallbackFunction callback, void* params) noexcept
        : callback_(callback), params_(params)
    {
    }

    void SimpleCallback::set(const CallbackFunction callback, void* params) noexcept
    {
        callback_ = callback;
        params_ = params;
    }

    void SimpleCallback::reset() noexcept
    {
        callback_ = nullptr;
        params_ = nullptr;
    }

    void SimpleCallback::invoke(void* value) const noexcept
    {
        if (callback_ != nullptr)
        {
            callback_(value, params_);
        }
    }

    bool SimpleCallback::is_set() const noexcept
    {
        return callback_ != nullptr;
    }
}
