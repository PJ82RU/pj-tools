#ifndef PJCAN_FIRMWARE_THREAD_H
#define PJCAN_FIRMWARE_THREAD_H

#include <Arduino.h>

#define THREAD_NAME_SIZE    32

class Thread {
public:
    /**
     * Поток. Обвертка Задачи.
     * @param pc_name Описательное имя задачи
     * @param us_stack_depth Количество слов (не байтов!) для использования в качестве стека задачи
     * @param ux_priority Приоритет, с которым будет выполняться созданная задача
     */
    Thread(const char *pc_name, uint32_t us_stack_depth, UBaseType_t ux_priority);
    ~Thread();

    /**
     * Запуск потока
     * @param pv_task_code Указатель на функцию ввода задачи
     * @param pv_parameters Значение, которое передается в качестве параметра созданной задачи
     * @return Результат выполнения
     */
    bool start(TaskFunction_t pv_task_code, void *pv_parameters);

    /**
     * Запуск потока
     * @param pv_task_code Указатель на функцию ввода задачи
     * @param pv_parameters Значение, которое передается в качестве параметра созданной задачи
     * @param xCoreID ID ядра
     * @return Результат выполнения
     */
    bool start(TaskFunction_t pv_task_code, void *pv_parameters, BaseType_t xCoreID);

    /** Остановить поток */
    void stop();

//    Не поддерживается Arduino
//    /** Статус задачи */
//    TaskStatus_t status() const;

    /** Поток запущен */
    bool is_started() const;

    /** Приостановить поток */
    void suspend();

    /** Возобновить приостановленный поток */
    void resume();

    /** Глубина стека */
    uint32_t get_stack_depth() const;

    /** Глубина используемого стека */
    UBaseType_t task_stack_depth();

protected:
    TaskHandle_t task = nullptr;
    char name[THREAD_NAME_SIZE]{};
    uint32_t stack_depth = 0;
    UBaseType_t priority = 0;
};

#endif //PJCAN_FIRMWARE_THREAD_H
