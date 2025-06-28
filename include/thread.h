#ifndef PJ_FIRMWARE_THREAD_H
#define PJ_FIRMWARE_THREAD_H

#include <Arduino.h>

/// @brief Максимальная длина имени потока (включая нуль-терминатор)
constexpr size_t THREAD_NAME_SIZE = 32;

/// @brief Класс-обертка для работы с задачами FreeRTOS
///
/// Предоставляет удобный интерфейс для создания и управления задачами,
/// включая контроль стека, приоритетов и привязку к ядрам процессора
class Thread
{
public:
    /**
     * @brief Конструктор задачи FreeRTOS
     * @param name Имя задачи (максимум 31 символ + нуль-терминатор)
     * @param stack_depth Размер стека в словах (4 байта на слово)
     * @param priority Приоритет задачи (0 - самый низкий)
     */
    Thread(const char* name, uint32_t stack_depth, UBaseType_t priority) noexcept;

    /// @brief Деструктор - автоматически останавливает задачу
    ~Thread() noexcept;

    // Запрещаем копирование
    Thread(const Thread&) = delete; ///< Запрет конструктора копирования
    Thread& operator=(const Thread&) = delete; ///< Запрет оператора присваивания

    /**
     * @brief Запуск задачи на любом доступном ядре
     * @param task_func Функция-задача (бесконечный цикл)
     * @param params Параметры для передачи в задачу
     * @return true если задача успешно создана
     */
    bool start(TaskFunction_t task_func, void* params) noexcept;

    /**
     * @brief Запуск задачи с привязкой к конкретному ядру
     * @param task_func Функция-задача (бесконечный цикл)
     * @param params Параметры для передачи в задачу
     * @param core_id Номер ядра (0 или 1)
     * @return true если задача успешно создана
     */
    bool start(TaskFunction_t task_func, void* params, BaseType_t core_id) noexcept;

    /**
     * @brief Остановка и удаление задачи
     * @note Безопасно вызывать даже если задача не запущена
     */
    void stop() noexcept;

    /**
     * @brief Проверка активности задачи
     * @return true если задача существует и не завершена
     */
    bool is_running() const noexcept;

    /**
     * @brief Приостановка выполнения задачи
     * @note Задачу можно возобновить методом resume()
     */
    void suspend() const noexcept;

    /**
     * @brief Возобновление приостановленной задачи
     */
    void resume() const noexcept;

    /**
     * @brief Получение размера стека задачи
     * @return Размер стека в словах (4 байта на слово)
     */
    uint32_t stack_size() const noexcept;

    /**
     * @brief Получение минимального свободного места в стеке
     * @return Минимальное количество оставшихся слов стека
     * @note Полезно для анализа использования стека
     */
    UBaseType_t stack_high_water_mark() const noexcept;

private:
    TaskHandle_t handle_ = nullptr; ///< Хэндл задачи FreeRTOS
    char name_[THREAD_NAME_SIZE] = {}; ///< Имя задачи (для отладки)
    uint32_t stack_depth_ = 0; ///< Запрошенный размер стека
    UBaseType_t priority_ = 0; ///< Приоритет задачи
};

#endif // PJ_FIRMWARE_THREAD_H
