#include <Arduino.h>
#include "callback.h"
#include "bytes.h"
#include "clock.h"

typedef struct test_t : tools::Callback::call_value_t {
    uint8_t value;
} test_t;

bool on_test1(void *p_value, void *p_parameters) {
    Serial.println("on_test1");
    test_t *t = (test_t *) p_value;
    Serial.println(t->value);
    return false;
}

bool on_test2(void *p_value, void *p_parameters) {
    Serial.println("on_test2");
    return false;
}

bool on_test3(void *p_value, void *p_parameters) {
    Serial.println("on_test3");
    return false;
}

tools::Callback callback(16, sizeof(test_t));

void setup() {
    Serial.begin(115200);
    delay(1000);

    callback.init(3);
    callback.set(on_test1, nullptr, true);
    callback.set(on_test2, nullptr, true);
    callback.set(on_test3);
}

test_t test{};

void loop() {
    test.value++;
    callback.call(test);
    delay(1000);
//    Serial.println(uxTaskGetStackHighWaterMark(callback.task_callback_call));
}