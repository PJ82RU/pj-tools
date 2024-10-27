#include "simple_callback.h"

namespace tools {
    SimpleCallback::SimpleCallback(SimpleCallback::event_receive_t cb, void *p_parameters) {
        set(cb, p_parameters);
    }

    void SimpleCallback::set(SimpleCallback::event_receive_t cb, void *p_parameters) {
        cb_receive = cb;
        p_receive_parameters = p_parameters;
    }

    void SimpleCallback::free() {
        cb_receive = nullptr;
        p_receive_parameters = nullptr;
    }

    void SimpleCallback::call(void *p_value) {
        if (cb_receive) cb_receive(p_value, p_receive_parameters);
    }
}
