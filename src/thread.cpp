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
    bool result;
    if (!task) {
        result = xTaskCreate(pv_task_code, name, stack_depth, pv_parameters, priority, &task) == pdPASS;
        if (result) {
            log_i("Task %s created", name);
        } else {
            log_i("Task %s not created", name);
        }
    } else {
        result = true;
    }
    return result;
}

bool Thread::start(TaskFunction_t pv_task_code, void *pv_parameters, BaseType_t xCoreID) {
    bool result;
    if (!task) {
        result = xTaskCreatePinnedToCore(pv_task_code, name, stack_depth, pv_parameters, priority, &task, xCoreID) ==
                  pdPASS;
        if (result) {
            log_i("Task %s created", name);
        } else {
            log_i("Task %s not created", name);
        }
    } else {
        result = true;
    }
    return result;
}

void Thread::stop() {
    if (task) {
        vTaskDelete(task);
        task = nullptr;
        log_i("Task %s deleted", name);
    }
}

//TaskStatus_t Thread::status() const {
//    TaskStatus_t details;
//    vTaskGetInfo(task, &details, pdTRUE, eInvalid);
//    return details;
//}

bool Thread::is_started() const {
    return task != nullptr;
}

void Thread::suspend() {
    if (task) {
        vTaskSuspend(task);
        log_i("Task %s is suspended", name);
    }
}

void Thread::resume() {
    if (task) {
        vTaskResume(task);
        log_i("Task %s has been resumed", name);
    }
}

uint32_t Thread::get_stack_depth() const {
    return stack_depth;
}

UBaseType_t Thread::task_stack_depth() {
    return task ? uxTaskGetStackHighWaterMark(task) : 0;
}

