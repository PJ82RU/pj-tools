#include "callback.h"

namespace tools {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

    void task_callback(void *pv_parameters) {
        Callback *callback = (Callback *) pv_parameters;
        int8_t buf[sizeof(void *) + sizeof(int8_t)];
        int8_t index;
        void *p_value{};

        for (;;) {
            if (xQueueReceive(callback->queue_callback, &buf, portMAX_DELAY) == pdTRUE) {
                index = buf[0];
                memcpy(p_value, &buf[1], sizeof(void *));
                callback->call_items(p_value, index);
            }
        }
    }

#pragma clang diagnostic pop

    Callback::Callback() {
        queue_callback = xQueueCreate(CALLBACK_BUFFER_NUM, sizeof(void *) + sizeof(int8_t));
        log_i("Queue callback created");

        xTaskCreate(&task_callback, "CALLBACK", 8192, this, 15, &task_callback_call);
        log_i("Task callback created");
    }

    Callback::~Callback() {
        vTaskDelete(task_callback_call);
        log_i("Task callback deleted");
        vQueueDelete(queue_callback);
        log_i("Queue callback deleted");

        if (items) delete items;
    }

    bool Callback::init(int8_t num) {
        if (num_items > 0 || num <= 0) return false;

        if (num > CALLBACK_ITEM_MAX) num = CALLBACK_ITEM_MAX;
        num_items = num;
        items = new item_t[num];
        clear();

        return true;
    }

    int8_t Callback::set(event_send_t item, void *p_parameters, bool only_index) {
        if (num_items > 0) {
            item_t *_item;
            for (int8_t i = 0; i < num_items; i++) {
                _item = &items[i];
                if (!_item->p_item) {
                    _item->p_item = item;
                    _item->p_parameters = p_parameters;
                    _item->only_index = only_index;
                    return i;
                }
            }
        }
        return -1;
    }

    void Callback::clear() {
        if (num_items > 0) {
            item_t *_item;
            for (int8_t i = 0; i < num_items; i++) {
                _item = &items[i];
                _item->p_item = nullptr;
                _item->p_parameters = nullptr;
                _item->only_index = false;
            }
        }
    }

    void Callback::call(void *p_value, int8_t index) {
        if (num_items > 0) {
            int8_t buf[sizeof(void *) + sizeof(int8_t)];
            buf[0] = index;
            memcpy(&buf[1], p_value, sizeof(void *));
            xQueueSend(queue_callback, &buf, 0);
        }
    }

    void Callback::call_items(void *p_value, int8_t index) {
        item_t *_item;
        size_t size;

        for (int8_t i = 0; i < num_items; i++) {
            _item = &items[i];
            if (_item->p_item && (!_item->only_index || _item->only_index && index == i)) {
                size = _item->p_item(p_value, _item->p_parameters);
                if (size != 0 && cb_receive) cb_receive(p_value, p_receive_parameters);
            }
        }
    }
}
