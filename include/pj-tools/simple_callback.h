#ifndef PJ_TOOLS_SIMPLE_CALLBACK_H
#define PJ_TOOLS_SIMPLE_CALLBACK_H

namespace pj_tools
{
    /**
     * @brief Класс для работы с простыми callback-функциями C-стиля
     *
     * @details Предоставляет механизм для регистрации и вызова функций обратного вызова
     *          с возможностью передачи пользовательских параметров
     */
    class SimpleCallback
    {
    public:
        /**
         * @brief Тип функции обратного вызова
         * @param value Указатель на передаваемые данные
         * @param params Указатель на пользовательские параметры
         */
        using CallbackFunction = void (*)(void* value, void* params);

        /**
         * @brief Конструктор callback-объекта
         * @param callback Функция обратного вызова (может быть nullptr)
         * @param params Пользовательские параметры для callback (может быть nullptr)
         */
        explicit SimpleCallback(CallbackFunction callback = nullptr, void* params = nullptr) noexcept;

        /**
         * @brief Установка callback функции и параметров
         * @param callback Функция обратного вызова (может быть nullptr)
         * @param params Пользовательские параметры (может быть nullptr)
         */
        void set(CallbackFunction callback, void* params) noexcept;

        /**
         * @brief Сброс зарегистрированного callback
         * @note После вызова isSet() будет возвращать false
         */
        void reset() noexcept;

        /**
         * @brief Вызов зарегистрированного callback
         * @param value Указатель на данные для передачи в callback
         * @note Ничего не происходит если callback не установлен
         */
        void invoke(void* value) const noexcept;

        /**
         * @brief Проверка наличия зарегистрированного callback
         * @return true если callback был установлен и не был сброшен
         */
        [[nodiscard]] bool isSet() const noexcept;

    private:
        /// Указатель на функцию обратного вызова
        CallbackFunction mCallback = nullptr;

        /// Пользовательские параметры для callback
        void* mParams = nullptr;
    };
} // namespace pj_tools

#endif // PJ_TOOLS_SIMPLE_CALLBACK_H
