# pjcan-tools
PJCAN Tools



### Создание объекта

```c++
tools::Callback callback(16, sizeof(value_t), "CALLBACK");
```



### Пример инициализации

```c++
callback.init(2);
callback.set(on_test);
callback.set(on_test2, nullptr, true);
```



### Пример функции обратного вызова

```c++
bool on_test(void *p_value, void *p_parameters) {
    auto *t = (value_t *) p_value;
    Serial.println(t->value);
    return false;
}
```



### Вызов функции

```c++
callback.call(&value);
```
