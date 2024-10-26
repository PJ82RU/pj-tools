#include "thread.h"

Thread::Thread(const char *pc_name, uint32_t us_stack_depth, UBaseType_t ux_priority) {
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
    if (xTaskCreate(pv_task_code, name, stack_depth, pv_parameters, priority, &task) == pdPASS) {
        log_i("Task %s created", name);
        return true;
    }
    log_i("Task %s not created", name);
    return false;
}

bool Thread::start(TaskFunction_t pv_task_code, void *pv_parameters, BaseType_t xCoreID) {
    if (xTaskCreatePinnedToCore(pv_task_code, name, stack_depth, pv_parameters, priority, &task, xCoreID) == pdPASS) {
        log_i("Task %s created", name);
        return true;
    }
    log_i("Task %s not created", name);
    return false;
}

void Thread::stop() {
    if (task) {
        vTaskDelete(task);
        log_i("Task %s deleted", name);
    }
}

bool Thread::is_started() {
    return task_stack_depth() != 0;
}

uint32_t Thread::get_stack_depth() const {
    return stack_depth;
}

UBaseType_t Thread::task_stack_depth() {
    return task ? uxTaskGetStackHighWaterMark(task) : 0;
}

