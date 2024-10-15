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
    bool result = xTaskCreate(pv_task_code, name, stack_depth, pv_parameters, priority, &task) == pdPASS;
    if (result)
        log_i("Task %s created", name);
    else
        log_i("Task %s not created", name);
    return result;
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

UBaseType_t Thread::task_stack_depth() {
    return task ? uxTaskGetStackHighWaterMark(task) : 0;
}

