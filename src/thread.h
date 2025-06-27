#ifndef PJ_FIRMWARE_THREAD_H
#define PJ_FIRMWARE_THREAD_H

#include <Arduino.h>
#include <cstring>

constexpr size_t THREAD_NAME_SIZE = 32;

class Thread
{
public:
    /**
     * @brief Конструктор потока
     * @param name Имя задачи (макс 31 символ)
     * @param stack_depth Размер стека в словах
     * @param priority Приоритет задачи
     */
    Thread(const char* name, uint32_t stack_depth, UBaseType_t priority) noexcept;
    ~Thread() noexcept;

    // Запрещаем копирование
    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;

    /**
     * @brief Запуск задачи на любом ядре
     * @param task_func Функция задачи
     * @param params Параметры задачи
     * @return true если задача успешно создана
     */
    bool start(TaskFunction_t task_func, void* params) noexcept;

    /**
     * @brief Запуск задачи на указанном ядре
     * @param task_func Функция задачи
     * @param params Параметры задачи
     * @param core_id ID ядра (0 или 1)
     * @return true если задача успешно создана
     */
    bool start(TaskFunction_t task_func, void* params, BaseType_t core_id) noexcept;

    /**
     * @brief Остановка задачи
     */
    void stop() noexcept;

    /**
     * @brief Проверка активности задачи
     * @return true если задача запущена
     */
    bool is_running() const noexcept;

    /**
     * @brief Приостановка задачи
     */
    void suspend() const noexcept;

    /**
     * @brief Возобновление задачи
     */
    void resume() const noexcept;

    /**
     * @brief Получить размер стека
     */
    uint32_t stack_size() const noexcept;

    /**
     * @brief Получить минимальный остаток стека
     */
    UBaseType_t stack_high_water_mark() const noexcept;

private:
    TaskHandle_t handle_ = nullptr;
    char name_[THREAD_NAME_SIZE] = {};
    uint32_t stack_depth_ = 0;
    UBaseType_t priority_ = 0;
};

#endif // PJ_FIRMWARE_THREAD_H
