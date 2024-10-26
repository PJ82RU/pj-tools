#ifndef PJCAN_FIRMWARE_THREAD_H
#define PJCAN_FIRMWARE_THREAD_H

#include <Arduino.h>

#define THREAD_NAME_SIZE    32

class Thread {
public:
    Thread(const char *pc_name, uint32_t us_stack_depth, UBaseType_t ux_priority);

    ~Thread();

    /**
     * Запуск потока
     * @param pv_task_code
     * @param pv_parameters
     * @return
     */
    bool start(TaskFunction_t pv_task_code, void *pv_parameters);

    /**
     * Запуск потока
     * @param pv_task_code
     * @param pv_parameters
     * @param xCoreID
     * @return
     */
    bool start(TaskFunction_t pv_task_code, void *pv_parameters, BaseType_t xCoreID);

    /** Остановить поток */
    void stop();

    /** Поток запущен */
    bool is_started();

    /** Глубина стека */
    uint32_t get_stack_depth() const;

    /** Глубина используемого стека */
    UBaseType_t task_stack_depth();

protected:
    TaskHandle_t task{};
    char name[THREAD_NAME_SIZE]{};
    uint32_t stack_depth = 0;
    UBaseType_t priority = 0;
};

#endif //PJCAN_FIRMWARE_THREAD_H
