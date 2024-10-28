#include "callback.h"
#include "esp32-hal-log.h"

namespace tools {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

    void task_callback(void *pv_parameters) {
        if (pv_parameters) {
            auto *callback = (Callback *) pv_parameters;
            tools::Callback::buffer_item_t b_item;

            for (;;) {
                if (callback->queue.receive(&b_item, portMAX_DELAY)) {
                    callback->call_items(b_item);
                }
            }
        }
    }

#pragma clang diagnostic pop

    Callback::Callback(uint8_t num, size_t size, const char *t_name, uint32_t t_stack_depth, UBaseType_t t_priority) :
            thread(t_name, t_stack_depth, t_priority),
            queue(num, sizeof(buffer_item_t)),
            semaphore(false) {

        if (num > 0 && size > 0) {
            num_buffer = num;
            size_buffer = size;
            buffer = new uint8_t[num_buffer * size_buffer];
            log_i("Callback created");
        } else {
            log_e("Required parameters are missing");
        }
    }

    Callback::~Callback() {
        thread.stop();
        delete[] items;
        delete[] buffer;
    }

    bool Callback::init(uint8_t num) {
        bool result = buffer && num_items == 0 && num != 0;
        if (result) {
            num_items = num;
            items = new item_t[num];
            clear();
            result = thread.start(&task_callback, this);
        }
        return result;
    }

    bool Callback::is_init() {
        return buffer && items;
    }

    int16_t Callback::set(event_send_t item, void *p_parameters, bool only_index) {
        if (is_init()) {
            item_t *_item;
            for (int16_t i = 0; i < num_items; ++i) {
                _item = &items[i];
                if (!_item->p_item) {
                    if (semaphore.take()) {
                        _item->p_item = item;
                        _item->p_parameters = p_parameters;
                        _item->only_index = only_index;
                        log_d("A callback with index %d was recorded", i);
                        semaphore.give();
                    }
                    return i;
                }
            }
            log_d("There is no free cell to record the callback");
        } else {
            log_d("The object is not initialized");
        }
        return -1;
    }

    void Callback::clear() {
        if (is_init() && semaphore.take()) {
            item_t *_item;
            for (int16_t i = 0; i < num_items; ++i) {
                _item = &items[i];
                _item->p_item = nullptr;
                _item->p_parameters = nullptr;
                _item->only_index = false;
            }
            log_d("Clearing all cells");
            semaphore.give();
        } else {
            log_d("The object is not initialized");
        }
    }

    void Callback::call(void *value, int16_t index) {
        if (buffer && value && semaphore.take()) {
            memcpy(&buffer[index_buffer * size_buffer], value, size_buffer);
            buffer_item_t b_item{};
            b_item.index_item = index;
            b_item.index_buffer = index_buffer;

            index_buffer++;
            if (index_buffer >= num_buffer) index_buffer = 0;

            queue.send(&b_item);
            log_d("Calling the callback function");
            semaphore.give();
        }
    }

    bool Callback::read(void *value) {
        bool result = false;
        if (buffer && value && semaphore.take()) {
            buffer_item_t buf{};
            result = queue.receive(&buf, 0);
            if (result) memcpy(value, &buffer[buf.index_buffer * size_buffer], size_buffer);
            semaphore.give();
        }
        return result;
    }

    void Callback::call_items(buffer_item_t &b_item) {
        item_t *_item;
        bool response;
        size_t pos = b_item.index_buffer * size_buffer;

        for (uint8_t i = 0; i < num_items; ++i) {
            _item = &items[i];
            if (_item->p_item && (!_item->only_index || _item->only_index && b_item.index_item == i)) {
                response = _item->p_item(&buffer[pos], _item->p_parameters);
                if (response) parent_callback.call(&buffer[pos]);
            }
        }
    }
}
