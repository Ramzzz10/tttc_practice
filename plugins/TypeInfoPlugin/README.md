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