#include "thread.h"

Thread::Thread(const char *pc_name, uint32_t us_stack_depth, UBaseType_t ux_priority) :
        semaphore(false) {
    size_t len = strlen(pc_name);
    if (len >= sizeof(name)) {
        len = sizeof(name) - 1;
        name[len] = 0;
    }
    memcpy(name, pc_name, len);

    stack_depth = us_stack_depth;
    priority = ux_priority;
}

Thread::~Thread() {
    stop();
}

bool Thread::start(TaskFunction_t pv_task_code, void *pv_parameters) {
    if (semaphore.take()) {
        if (!started) {
            started = xTaskCreate(pv_task_code, name, stack_depth, pv_parameters, priority, &task) == pdPASS;
            if (started) {
                log_i("Task %s created", name);
            } else {
                log_i("Task %s not created", name);
            }
        }
        semaphore.give();
    }
    return started;
}

bool Thread::start(TaskFunction_t pv_task_code, void *pv_parameters, BaseType_t xCoreID) {
    if (semaphore.take()) {
        if (!started) {
            started =
                    xTaskCreatePinnedToCore(pv_task_code, name, stack_depth, pv_parameters, priority, &task, xCoreID) ==
                    pdPASS;
            if (started) {
                log_i("Task %s created", name);
            } else {
                log_i("Task %s not created", name);
            }
        }
        semaphore.give();
    }
    return started;
}

void Thread::stop() {
    if (semaphore.take()) {
        if (started) {
            started = false;
            vTaskDelete(task);
            log_i("Task %s deleted", name);
        }
        semaphore.give();
    }
}

bool Thread::is_started() const {
    return started && task;
}

void Thread::suspend() {
    if (semaphore.take()) {
        if (is_started()) {
            vTaskSuspend(task);
            log_i("Task %s is suspended", name);
        }
        semaphore.give();
    }
}

void Thread::resume() {
    if (semaphore.take()) {
        if (is_started()) {
            vTaskResume(task);
            log_i("Task %s has been resumed", name);
        }
        semaphore.give();
    }
}

uint32_t Thread::get_stack_depth() const {
    return stack_depth;
}

UBaseType_t Thread::task_stack_depth() {
    return is_started() ? uxTaskGetStackHighWaterMark(task) : 0;
}

