#include "callback.h"
#include "esp32-hal-log.h"

namespace tools {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

    void task_callback(void *pv_parameters) {
        auto *callback = (Callback *) pv_parameters;
        tools::Callback::buffer_item_t b_item;

        for (;;) {
            if (xQueueReceive(callback->queue, &b_item, portMAX_DELAY) == pdTRUE) {
                callback->call_items(b_item);
            }
        }
    }

#pragma clang diagnostic pop

    Callback::Callback(uint8_t num, size_t size, const char *name, uint32_t stack_depth, UBaseType_t priority) {
        if (num > 0 && size > 0) {
            num_buffer = num;
            size_buffer = size;
            buffer = new uint8_t[num_buffer * size_buffer];
        }

        mutex = xSemaphoreCreateMutex();

        queue = xQueueCreate(num, sizeof(buffer_item_t));
        log_i("Queue callback created");

        xTaskCreate(&task_callback, name, stack_depth, this, priority, &task_callback_call);
        log_i("Task callback created");
    }

    Callback::~Callback() {
        vTaskDelete(task_callback_call);
        log_i("Task callback deleted");
        vQueueDelete(queue);
        log_i("Queue callback deleted");

        delete[] items;
        delete[] buffer;
    }

    bool Callback::init(uint8_t num) {
        if (num_items > 0 || num == 0) return false;

        num_items = num;
        items = new item_t[num];
        clear();

        log_i("Callback initialized");
        return true;
    }

    bool Callback::is_init() {
        return buffer && items;
    }

    int16_t Callback::set(event_send_t item, void *p_parameters, bool only_index) {
        if (is_init() && xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
            item_t *_item;
            for (int16_t i = 0; i < num_items; i++) {
                _item = &items[i];
                if (!_item->p_item) {
                    _item->p_item = item;
                    _item->p_parameters = p_parameters;
                    _item->only_index = only_index;
                    xSemaphoreGive(mutex);

                    log_d("A callback with index %d was recorded", i);
                    return i;
                }
            }
            xSemaphoreGive(mutex);
            log_d("There is no free cell to record the callback");
        } else
            log_d("The object is not initialized");

        return -1;
    }

    void Callback::clear() {
        if (is_init() && xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
            item_t *_item;
            for (int16_t i = 0; i < num_items; i++) {
                _item = &items[i];
                _item->p_item = nullptr;
                _item->p_parameters = nullptr;
                _item->only_index = false;
            }
            xSemaphoreGive(mutex);
            log_d("Clearing all cells");
        } else
            log_d("The object is not initialized");
    }

    void Callback::call(void *value, int16_t index) {
        if (buffer && value && xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
            memcpy(&buffer[index_buffer * size_buffer], value, size_buffer);
            buffer_item_t b_item{};
            b_item.index_item = index;
            b_item.index_buffer = index_buffer;

            index_buffer++;
            if (index_buffer >= num_buffer) index_buffer = 0;

            xQueueSend(queue, &b_item, 0);
            xSemaphoreGive(mutex);
            log_d("Calling the callback function");
        }
    }

    bool Callback::read(void *value) {
        if (!buffer || !value) return false;

        buffer_item_t buf{};
        bool result = xQueueReceive(queue, &buf, 0) == pdTRUE;
        if (result) memcpy(value, &buffer[buf.index_buffer * size_buffer], size_buffer);
        return result;
    }

    void Callback::call_items(buffer_item_t &b_item) {
        if (is_init() && xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
            item_t *_item;
            bool response;
            size_t pos = b_item.index_buffer * size_buffer;

            for (uint8_t i = 0; i < num_items; i++) {
                _item = &items[i];
                if (_item->p_item && (!_item->only_index || _item->only_index && b_item.index_item == i)) {
                    response = _item->p_item(&buffer[pos], _item->p_parameters);
                    if (response && cb_receive) cb_receive(&buffer[pos], p_receive_parameters);
                }
            }
            xSemaphoreGive(mutex);
        }
    }

    UBaseType_t Callback::task_stack_depth() {
        return uxTaskGetStackHighWaterMark(task_callback_call);
    }
}
