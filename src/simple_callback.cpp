#include "simple_callback.h"

namespace pj_tools
{
    SimpleCallback::SimpleCallback(const CallbackFunction callback, void* params) noexcept
        : mCallback(callback),
          mParams(params)
    {
    }

    void SimpleCallback::set(const CallbackFunction callback, void* params) noexcept
    {
        mCallback = callback;
        mParams = params;
    }

    void SimpleCallback::reset() noexcept
    {
        mCallback = nullptr;
        mParams = nullptr;
    }

    void SimpleCallback::invoke(void* value) const noexcept
    {
        if (mCallback != nullptr)
        {
            mCallback(value, mParams);
        }
    }

    bool SimpleCallback::isSet() const noexcept
    {
        return mCallback != nullptr;
    }
} // namespace pj_tools
