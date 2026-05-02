# LaboratoryWork2

Консольное приложение на C++20 для работы с учебными реализациями динамического массива, связного списка, последовательностей, битовых последовательностей и алгоритмов над ними.

## Структура проекта

- `main.cpp` - точка входа приложения; вызывает `run_menu()`.
- `include/Menu/menu.h`, `src/menu.cpp` - консольный UI для создания объектов и запуска операций над модулями.
- `include/DynamicArray/dynamic_array.h`, `include/DynamicArray/dynamic_array.tpp` - шаблонный динамический массив `DynamicArray<T>`.
- `include/LinkedList/linked_list.h`, `include/LinkedList/linked_list.tpp` - шаблонный связный список `LinkedList<T>`.
- `include/Sequence/sequence.h`, `include/Sequence/sequence.tpp` - общий интерфейс `Sequence<T>` и реализации `MutableArraySequence`, `ImmutableArraySequence`, `MutableListSequence`, `ImmutableListSequence`.
- `include/Bit/bit.h` - тип `Bit` и базовые битовые операции.
- `include/Bit/bit_sequence.h`, `include/Bit/bit_sequence.tpp` - `BitSequence`, последовательность битов поверх общего интерфейса `Sequence<Bit>`.
- `include/MapReduce/map_reduce.h`, `include/MapReduce/map_reduce.tpp` - дополнительные алгоритмы над последовательностями: `Pair`, `zip`, `unzip`, `skip`, `split`, `slice`, `flat_map`.
- `include/Iterator/ienumerator.h` - интерфейс итератора `IEnumerator<T>` и RAII-обертка `EnumeratorWrapper<T>`.
- `include/Option/option.h` - простой контейнер `Option<T>` для безопасного результата без исключения.
- `tests/all_tests.cpp` - единый консольный запуск тестов по группам.

## Сборка

Требования:

- CMake 3.16 или новее;
- компилятор C++ с поддержкой C++20.

Сборка из корня проекта:

```bash
cmake -S . -B cmake-build-debug
cmake --build cmake-build-debug --target LaboratoryWork2
```

В уже настроенной CLion/CMake-сборке достаточно выполнить:

```bash
cmake --build cmake-build-debug --target LaboratoryWork2
```

## Запуск приложения

После сборки запустите исполняемый файл:

```bash
./cmake-build-debug/LaboratoryWork2
```

На Windows из PowerShell:

```powershell
.\cmake-build-debug\LaboratoryWork2.exe
```

Приложение открывает главное консольное меню `Laboratory Work 2 Menu`. Пункты меню могут меняться во время доработки UI, поэтому устойчивый сценарий такой:

1. Сначала выберите пункт создания объекта нужного модуля.
2. Введите размер и элементы.
3. После создания объект получает индекс.
4. Выберите пункт операций этого же модуля.
5. Укажите индекс ранее созданного объекта.
6. Выберите нужную функцию внутри модуля и введите дополнительные параметры.
7. Для просмотра всех созданных объектов используйте пункт печати всех объектов.
8. Для выхода выберите `Exit`.

В текущем UI одновременно хранится до 10 объектов каждого типа: `DynamicArray<int>`, `LinkedList<int>`, `Sequence<int>` и `BitSequence`.

## Работа с модулями через консольный UI

### DynamicArray

Файлы модуля: `include/DynamicArray/dynamic_array.h`, `include/DynamicArray/dynamic_array.tpp`.

Чтобы запустить модуль отдельно через UI:

1. Выберите создание `DynamicArray<int>`.
2. Введите размер массива.
3. Введите элементы по индексам.
4. Выберите операции `DynamicArray`.
5. Укажите индекс созданного массива.

Доступные через UI операции:

- `Get` - получить элемент по индексу.
- `Set` - записать новое значение по индексу.
- `Resize` - изменить размер массива.
- `operator[] read` - прочитать элемент через оператор `[]`.
- `operator[] write` - изменить элемент через оператор `[]`.

Основные функции модуля в коде:

- конструкторы: пустой, из массива элементов, по размеру, копирующий;
- `operator=`;
- `get(index)`;
- `set(index, value)`;
- `operator[](index)` для чтения и записи;
- `get_size()`;
- `resize(newSize)`;
- `get_enumerator()`.

### LinkedList

Файлы модуля: `include/LinkedList/linked_list.h`, `include/LinkedList/linked_list.tpp`.

Чтобы запустить модуль отдельно через UI:

1. Выберите создание `LinkedList<int>`.
2. Введите размер списка.
3. Введите элементы по порядку.
4. Выберите операции `LinkedList`.
5. Укажите индекс созданного списка.

Доступные через UI операции:

- `Get` - получить элемент по индексу.
- `Append` - добавить элемент в конец.
- `Prepend` - добавить элемент в начало.
- `InsertAt` - вставить элемент по индексу.
- `GetSubList` - создать новый список из диапазона индексов.
- `Concat` - создать новый список как объединение двух списков.
- `operator[] read` - прочитать элемент через оператор `[]`.
- `operator[] write` - изменить элемент через оператор `[]`.

Основные функции модуля в коде:

- конструкторы: пустой, из массива элементов, копирующий;
- `operator=`;
- `get_first()`;
- `get_last()`;
- `get(index)`;
- `operator[](index)` для чтения и записи;
- `get_length()`;
- `get_sub_list(startIndex, endIndex)`;
- `append(item)`;
- `prepend(item)`;
- `insert_at(item, index)`;
- `concat(other)`;
- `get_enumerator()`.

### Sequence

Файлы модуля: `include/Sequence/sequence.h`, `include/Sequence/sequence.tpp`.

Чтобы запустить модуль отдельно через UI:

1. Выберите создание `Sequence<int>`.
2. Выберите реализацию: `MutableArraySequence<int>`, `ImmutableArraySequence<int>`, `MutableListSequence<int>` или `ImmutableListSequence<int>`.
3. Введите размер последовательности.
4. Введите элементы.
5. Выберите операции `Sequence<int>`.
6. Укажите индекс созданной последовательности.

Доступные через UI операции:

- `Get` - получить элемент по индексу.
- `Append` - добавить элемент в конец.
- `Prepend` - добавить элемент в начало.
- `InsertAt` - вставить элемент по индексу.
- `GetSubSequence` - создать новую подпоследовательность по диапазону индексов.
- `Concat` - создать результат объединения двух последовательностей.
- `Map square` - создать новую последовательность квадратов элементов.
- `Where positive` - создать новую последовательность только из положительных элементов.
- `Reduce sum` - посчитать сумму элементов.
- `operator[] read` - прочитать элемент через оператор `[]`.
- `operator[] write` - изменить элемент через оператор `[]`, если выбранная реализация mutable.

Если в текущей версии меню отображаются дополнительные пункты для лабораторной фазы 5, запускайте их из раздела `Sequence<int> operations`. Они относятся к алгоритмам над уже созданной `Sequence<int>` и могут сохранять результат как новый объект или печатать вычисленное значение.

Основные функции интерфейса `Sequence<T>` в коде:

- `get_first()`, `get_last()`, `get(index)`;
- `operator[](index)` для чтения;
- `try_get_first()`, `try_get_last()`, `try_get(index)`, `try_find(predicate)`;
- `get_length()`;
- `get_sub_sequence(startIndex, endIndex)`;
- `append(item)`, `prepend(item)`, `insert_at(item, index)`;
- `concat(list)`;
- `map(func)`, `map(funcWithIndex)`;
- `where(predicate)`;
- `reduce(func, initialElement)`;
- `get_enumerator()`.

Важная особенность: mutable-реализации изменяют текущий объект, а immutable-реализации возвращают новый объект. UI учитывает это и заменяет сохраненную последовательность результатом операции, если операция вернула новый объект.

### Bit и BitSequence

Файлы модуля: `include/Bit/bit.h`, `include/Bit/bit_sequence.h`, `include/Bit/bit_sequence.tpp`.

Чтобы запустить модуль отдельно через UI:

1. Выберите создание `BitSequence`.
2. Введите размер битовой последовательности.
3. Введите каждый бит как `0` или `1`.
4. Выберите операции `BitSequence`.
5. Укажите индекс созданной битовой последовательности.

Доступные через UI операции:

- `Get` - получить бит по индексу.
- `Append` - добавить бит в конец.
- `Prepend` - добавить бит в начало.
- `InsertAt` - вставить бит по индексу.
- `GetSubSequence` - создать новую битовую подпоследовательность.
- `AND` - создать результат побитового И двух `BitSequence`.
- `OR` - создать результат побитового ИЛИ двух `BitSequence`.
- `XOR` - создать результат исключающего ИЛИ двух `BitSequence`.
- `NOT` - создать инвертированную копию последовательности.
- `operator[] read` - прочитать бит через оператор `[]`.
- `operator[] write` - изменить бит через оператор `[]`.

Основные функции `Bit` в коде:

- конструкторы из `bool` и `int`;
- `get()`;
- операторы `&`, `|`, `^`, `~`;
- операторы сравнения `==` и `!=`.

Основные функции `BitSequence` в коде:

- конструкторы: пустой, из массива `Bit`, копирующий;
- `operator=`;
- `get_first()`, `get_last()`, `get(index)`;
- `operator[](index)` для чтения и записи через `BitReference`;
- `try_get_first()`, `try_get_last()`, `try_get(index)`;
- `get_length()`;
- `append(bit)`, `prepend(bit)`, `insert_at(bit, index)`;
- `get_sub_sequence(startIndex, endIndex)`;
- `AND(other)`, `OR(other)`, `XOR(other)`, `NOT()`;
- `get_enumerator()`.

### MapReduce

Файлы модуля: `include/MapReduce/map_reduce.h`, `include/MapReduce/map_reduce.tpp`.

Этот модуль предоставляет алгоритмы для использования из кода. В текущем консольном UI напрямую вынесены базовые `map`, `where` и `reduce` из `Sequence<T>`, а функции `MapReduce` можно вызывать из собственных тестов или демонстрационного кода.

Доступные функции:

- `Pair<TFirst, TSecond>` - пара значений с доступом через `first()` и `second()`.
- `zip(first, second)` - объединить две последовательности в последовательность пар.
- `unzip(sequence)` - разделить последовательность пар на две последовательности.
- `skip(sequence, count)` - пропустить первые `count` элементов.
- `split(sequence, predicate)` - разделить элементы по предикату.
- `slice(sequence, index, count, replacement)` - вырезать или заменить диапазон.
- `flat_map(sequence, func)` - отобразить каждый элемент в последовательность и объединить результаты.

### Option и Iterator

Файлы модулей: `include/Option/option.h`, `include/Iterator/ienumerator.h`.

Эти модули поддерживают остальные структуры данных и обычно не запускаются отдельно через UI.

`Option<T>` используется для безопасных методов `try_*`:

- `has_value()`;
- `is_some()`;
- `is_none()`;
- `get_value()`;
- `Option<T>::some(value)`;
- `Option<T>::none()`.

`IEnumerator<T>` и `EnumeratorWrapper<T>` используются для обхода элементов:

- `move_next()`;
- `get_current()`;
- `reset()`;
- автоматическое освобождение итератора в `EnumeratorWrapper`.

## Автодемонстрация и просмотр объектов

В главном меню есть служебные пункты:

- печать всех объектов - выводит все созданные `DynamicArray`, `LinkedList`, `Sequence<int>` и `BitSequence` с их индексами;
- автодемонстрация - создает пример `Sequence<int>`, применяет `map(square)`, создает `BitSequence` и применяет `NOT`.

Эти пункты удобны, если нужно быстро проверить, что меню и базовые операции работают без ручного ввода большого набора данных.

## Тесты

Сборка тестового target:

```bash
cmake --build cmake-build-debug --target AllTests
```

Запуск:

```bash
./cmake-build-debug/AllTests
```

На Windows из PowerShell:

```powershell
.\cmake-build-debug\AllTests.exe
```

Тестовое меню `Laboratory Work 2 Tests` позволяет отдельно запустить:

- `DynamicArray tests`;
- `LinkedList tests`;
- `Sequence tests`;
- `BitSequence tests`;
- все тесты сразу.

Проверки используют `assert`, поэтому при сборке с отключенными assert-проверками часть тестовой логики не будет выполняться.
