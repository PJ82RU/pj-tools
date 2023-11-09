#include <Arduino.h>
#include "callback.h"
#include "bytes.h"
#include "clock.h"

typedef struct test_t {
    uint8_t value[512];
} test_t;

bool on_test1(void *p_value, void *p_parameters) {
    Serial.println("on_test1");
    auto *t = (test_t *) p_value;
    Serial.println(t->value[500]);
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
    callback.set(on_test1);
    callback.set(on_test2, nullptr, true);
    callback.set(on_test3, nullptr, true);
}

test_t test{};

void loop() {
    test.value[500]++;
    callback.call(&test);
    delay(1000);
    Serial.println(callback.task_stack_depth());
}