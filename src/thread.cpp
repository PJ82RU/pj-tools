#include "thread.h"
#include <esp_log.h>

Thread::Thread(const char* name, const uint32_t stack_depth, const UBaseType_t priority) noexcept
    : stack_depth_(stack_depth), priority_(priority)
{
    const size_t len = std::min(strlen(name), THREAD_NAME_SIZE - 1);
    memcpy(name_, name, len);
    name_[len] = '\0';
}

Thread::~Thread() noexcept
{
    stop();
}

bool Thread::start(const TaskFunction_t task_func, void* params) noexcept
{
    if (handle_) return false;

    if (xTaskCreate(task_func, name_, stack_depth_, params, priority_, &handle_) == pdPASS)
    {
        ESP_LOGI("Thread", "Task %s created", name_);
        return true;
    }

    ESP_LOGE("Thread", "Failed to create task %s", name_);
    return false;
}

bool Thread::start(const TaskFunction_t task_func, void* params, const BaseType_t core_id) noexcept
{
    if (handle_) return false;

    if (xTaskCreatePinnedToCore(task_func, name_, stack_depth_, params, priority_, &handle_, core_id) == pdPASS)
    {
        ESP_LOGI("Thread", "Task %s created on core %d", name_, core_id);
        return true;
    }

    ESP_LOGE("Thread", "Failed to create task %s on core %d", name_, core_id);
    return false;
}

void Thread::stop() noexcept
{
    if (handle_)
    {
        vTaskDelete(handle_);
        handle_ = nullptr;
        ESP_LOGI("Thread", "Task %s deleted", name_);
    }
}

bool Thread::is_running() const noexcept
{
    return handle_ != nullptr;
}

void Thread::suspend() const noexcept
{
    if (handle_)
    {
        vTaskSuspend(handle_);
        ESP_LOGI("Thread", "Task %s suspended", name_);
    }
}

void Thread::resume() const noexcept
{
    if (handle_)
    {
        vTaskResume(handle_);
        ESP_LOGI("Thread", "Task %s resumed", name_);
    }
}

uint32_t Thread::stack_size() const noexcept
{
    return stack_depth_;
}

UBaseType_t Thread::stack_high_water_mark() const noexcept
{
    return handle_ ? uxTaskGetStackHighWaterMark(handle_) : 0;
}
