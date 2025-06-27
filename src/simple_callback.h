#ifndef PJ_TOOLS_SIMPLE_CALLBACK_H
#define PJ_TOOLS_SIMPLE_CALLBACK_H

namespace tools
{
    class SimpleCallback
    {
    public:
        using CallbackFunction = void (*)(void* value, void* params);

        /**
         * @brief Конструктор с инициализацией callback
         * @param callback Функция обратного вызова
         * @param params Параметры для callback (может быть nullptr)
         */
        explicit SimpleCallback(CallbackFunction callback = nullptr, void* params = nullptr) noexcept;

        /**
         * @brief Установка callback функции и параметров
         * @param callback Функция обратного вызова (может быть nullptr)
         * @param params Параметры для callback (может быть nullptr)
         */
        void set(CallbackFunction callback, void* params) noexcept;

        /**
         * @brief Сброс callback функции и параметров
         */
        void reset() noexcept;

        /**
         * @brief Вызов callback функции
         * @param value Значение для передачи в callback
         */
        void invoke(void* value) const noexcept;

        /**
         * @brief Проверка наличия установленного callback
         * @return true если callback установлен
         */
        [[nodiscard]] bool is_set() const noexcept;

    private:
        CallbackFunction callback_ = nullptr;
        void* params_ = nullptr;
    };
}

#endif // PJ_TOOLS_SIMPLE_CALLBACK_H
