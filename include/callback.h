#ifndef PJ_TOOLS_CALLBACK_H
#define PJ_TOOLS_CALLBACK_H

#include "thread.h"
#include "semaphore.h"
#include "queue.h"
#include "simple_callback.h"

namespace tools
{
    /**
     * @brief Класс для управления callback-функциями с буферизацией данных
     */
    class Callback
    {
    public:
        /**
         * @brief Тип функции callback
         * @param arg1 Указатель на данные
         * @param arg2 Указатель на параметры
         * @return Результат выполнения (true/false)
         */
        using EventSendFunc = bool (*)(void*, void*);

        /**
         * @brief Структура элемента callback
         */
        struct Item
        {
            bool only_index; ///< Флаг вызова только по индексу
            EventSendFunc func; ///< Указатель на callback-функцию
            void* params; ///< Параметры для callback-функции
        };

        /**
         * @brief Структура элемента буфера
         */
        struct BufferItem
        {
            int16_t item_index; ///< Индекс callback-функции
            uint8_t buffer_index; ///< Индекс в буфере данных
        };

        /**
         * @brief Конструктор менеджера callback-функций
         * @param buffer_size Количество элементов в буфере
         * @param item_size Размер одного элемента буфера (в байтах)
         * @param name Имя задачи для отладки
         * @param stack_depth Размер стека задачи (по умолчанию 3072)
         * @param priority Приоритет задачи (по умолчанию 18)
         */
        Callback(uint8_t buffer_size,
                 size_t item_size,
                 const char* name,
                 uint32_t stack_depth = 3072,
                 UBaseType_t priority = 18);

        /**
         * @brief Деструктор (освобождает ресурсы)
         */
        ~Callback();

        // Запрещаем копирование объектов
        Callback(const Callback&) = delete;
        Callback& operator=(const Callback&) = delete;

        /**
         * @brief Инициализация менеджера callback-функций
         * @param num_callbacks Количество callback-функций
         * @return true если инициализация прошла успешно
         */
        bool init(uint8_t num_callbacks) noexcept;

        /**
         * @brief Проверка состояния инициализации
         * @return true если объект инициализирован
         */
        [[nodiscard]] bool is_initialized() const noexcept;

        /**
         * @brief Добавление новой callback-функции
         * @param func Указатель на callback-функцию
         * @param params Параметры для функции (опционально)
         * @param only_index Флаг вызова только по индексу (опционально)
         * @return Индекс добавленной функции или -1 при ошибке
         */
        int16_t add_callback(EventSendFunc func, void* params = nullptr, bool only_index = false) const noexcept;

        /**
         * @brief Очистка всех зарегистрированных callback-функций
         */
        void free() const noexcept;

        /**
         * @brief Вызов callback-функции/функций
         * @param value Указатель на данные для передачи
         * @param index Индекс callback (-1 для вызова всех)
         */
        void invoke(const void* value, int16_t index = -1) noexcept;

        /**
         * @brief Чтение данных из буфера
         * @param value Указатель на буфер для данных
         * @return true если данные успешно прочитаны
         */
        bool read(void* value) const noexcept;

        /**
         * @brief Родительский callback для обработки событий
         */
        SimpleCallback parent_callback;

    private:
        /**
         * @brief Задача для обработки callback-функций
         * @param arg Указатель на объект Callback
         */
        static void callback_task(void* arg) noexcept;

        /**
         * @brief Обработка элементов callback
         * @param item Элемент буфера для обработки
         */
        void process_items(const BufferItem& item) const noexcept;

        /**
         * @brief Основной цикл обработки
         */
        void run() const noexcept;

        Thread thread_; ///< Поток для обработки callback
        Queue queue_; ///< Очередь для хранения заданий
        Semaphore semaphore_; ///< Семафор для синхронизации

        uint8_t num_items_ = 0; ///< Количество зарегистрированных callback-функций
        Item* items_ = nullptr; ///< Массив callback-функций

        uint8_t buffer_size_ = 0; ///< Размер буфера данных
        size_t item_size_ = 0; ///< Размер элемента данных
        uint8_t current_buffer_index_ = 0; ///< Текущий индекс в буфере
        uint8_t* buffer_ = nullptr; ///< Буфер для хранения данных
    };
}

#endif // PJ_TOOLS_CALLBACK_H
