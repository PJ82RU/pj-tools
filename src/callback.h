#ifndef PJ_TOOLS_CALLBACK_H
#define PJ_TOOLS_CALLBACK_H

#include "thread.h"
#include "semaphore.h"
#include "queue.h"
#include "simple_callback.h"

namespace tools
{
    class Callback
    {
    public:
        using EventSendFunc = bool (*)(void*, void*);

        struct Item
        {
            bool only_index;
            EventSendFunc func;
            void* params;
        };

        struct BufferItem
        {
            int16_t item_index;
            uint8_t buffer_index;
        };

        SimpleCallback parent_callback;

        /**
         * @brief Конструктор callback менеджера
         * @param buffer_size Количество элементов буфера
         * @param item_size Размер элемента буфера
         * @param name Имя задачи
         * @param stack_depth Глубина стека (по умолчанию 3072)
         * @param priority Приоритет задачи (по умолчанию 18)
         */
        Callback(uint8_t buffer_size, size_t item_size, const char* name, uint32_t stack_depth = 3072,
                 UBaseType_t priority = 18);

        ~Callback();

        // Запрещаем копирование
        Callback(const Callback&) = delete;
        Callback& operator=(const Callback&) = delete;

        /**
         * @brief Инициализация callback менеджера
         * @param num_callbacks Количество callback функций
         * @return true если инициализация успешна
         */
        bool init(uint8_t num_callbacks) noexcept;

        /**
         * @brief Проверка инициализации
         */
        [[nodiscard]] bool is_initialized() const noexcept;

        /**
         * @brief Добавить callback функцию
         * @param func Функция callback
         * @param params Параметры функции
         * @param only_index Вызывать только по индексу
         * @return Индекс callback или -1 при ошибке
         */
        int16_t add_callback(EventSendFunc func, void* params = nullptr, bool only_index = false) const noexcept;

        /**
         * @brief Очистить все callback функции
         */
        void clear() const noexcept;

        /**
         * @brief Вызвать callback
         * @param value Данные для передачи
         * @param index Индекс callback (-1 для всех)
         */
        void invoke(const void* value, int16_t index = -1) noexcept;

        /**
         * @brief Прочитать данные из буфера
         * @param value Буфер для данных
         * @return true если данные прочитаны
         */
        bool read_data(void* value) const noexcept;

    private:
        static void callback_task(void* arg) noexcept;
        void process_items(const BufferItem& item) const noexcept;
        void run() const noexcept;

        Thread thread_;
        Queue queue_;
        Semaphore semaphore_;

        uint8_t num_items_ = 0;
        Item* items_ = nullptr;

        uint8_t buffer_size_ = 0;
        size_t item_size_ = 0;
        uint8_t current_buffer_index_ = 0;
        uint8_t* buffer_ = nullptr;
    };
}

#endif // PJ_TOOLS_CALLBACK_H
