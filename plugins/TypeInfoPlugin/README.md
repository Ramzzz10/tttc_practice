Плагин TypeInfo для Clang
===========================

Этот плагин выводит информацию о пользовательских типах данных:
- Поля
- Методы
- Базовые классы

Сборка плагина
-------------

Для сборки плагина выполните следующие команды:

```
cmake -S TypeInfoPlugin  -B build -G Ninja
ninja -C build  
clang -cc1 -load build/TypeInfoPlugin.dylib -plugin type-info ./tests/test.cpp

```

Плагин выведет:

Human
|_Fields
| |_ age (unsigned int|public)
| |_ height (unsigned int|public)
|
|_Methods
| |_ sleep (void()|public|virtual|pure)
| |_ eat (void()|public|virtual|pure)

Engineer -> Human
|_Fields
| |_ salary (unsigned int|public)
|
|_Methods
| |_ sleep (void()|public|virtual|override)
| |_ eat (void()|public|virtual|override)
| |_ work (void()|public)

Vehicle
|_Fields
| |_ year (int|private)
| |_ price (double|private)
| |_ model (char[50]|private)
|
|_Methods
| |_ Vehicle (void()|public)
| |_ start (void()|public|virtual)
| |_ stop (void()|public|virtual)
| |_ getYear (int()|public|const)
| |_ getPrice (double()|public|const)

Car -> Vehicle
|_Fields
| |_ doors (int|private)
|
|_Methods
| |_ Car (void()|public)
| |_ start (void()|public|virtual|override)
| |_ stop (void()|public|virtual|override)
| |_ getDoors (int()|public|const)


Инструментированная версия плагина
-----------------------------

В файле `InstrumentedTypeInfoPlugin.cpp` представлена версия плагина с добавленной инструментацией. Эта версия добавляет вызовы функций `instrument_start()` и `instrument_end()` в начало и конец каждой функции модуля.

### Особенности инструментированной версии

- Каждая функция в плагине инструментирована вызовами `instrument_start()` и `instrument_end()`
- Функции инструментации записывают вход и выход из каждой функции в stderr с помощью `llvm::errs()`
- Имена функций полностью квалифицированы (например, `TypeInfoVisitor::getAccessSpecifierString`) для лучшей ясности в логах

### Использование инструментированной версии

1. Для сборки инструментированной версии плагина используйте те же команды:
   ```
   cmake -S TypeInfoPlugin -B build -G Ninja
   ninja -C build
   ```

2. Запустите инструментированный плагин на тестовом файле:
   ```
   clang -cc1 -load build/InstrumentedTypeInfoPlugin.dylib -plugin type-info ./tests/test_instrumentation.cpp
   ```

3. Вы увидите логи инструментации в формате:
   ```
   ENTER: ИмяФункции
   EXIT: ИмяФункции
   ```

### Тестовый файл для инструментации

Файл `tests/test_instrumentation.cpp` содержит различные классы и методы для проверки работы инструментации:
- Базовый класс с виртуальными методами
- Производный класс с переопределением методов
- Шаблонный класс
- Класс с вложенными типами

### Преимущества инструментации

- Помогает при отладке, показывая точный путь выполнения
- Предоставляет информацию о последовательности вызовов функций
- Может использоваться для анализа производительности
- Облегчает понимание потока управления плагина
