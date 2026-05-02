#include "../include/Menu/menu.h"

#include <windows.h>

#include <iostream>
#include <limits>
#include <stdexcept>

#include "../include/Bit/bit_sequence.h"
#include "../include/MapReduce/map_reduce.h"

namespace {

const int MAX_OBJECTS = 10;

DynamicArray<int>* dynamicArrays[MAX_OBJECTS];
int dynamicArrayCount = 0;

LinkedList<int>* linkedLists[MAX_OBJECTS];
int linkedListCount = 0;

Sequence<int>* sequences[MAX_OBJECTS];
int sequenceCount = 0;

BitSequence* bitSequences[MAX_OBJECTS];
int bitSequenceCount = 0;

int square(const int& value) {
  return value * value;
}

bool is_positive(const int& value) {
  return value > 0;
}

int sum(const int& accumulator, const int& current) {
  return accumulator + current;
}

int add_index(const int& value, int index) {
  return value + index;
}

Sequence<int>* expand_with_negative(const int& value) {
  Sequence<int>* result = new MutableArraySequence<int>();
  result->append(value);
  result->append(-value);
  return result;
}

void clear_input() {
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int read_int() {
  int value;

  while (!(std::cin >> value)) {
    clear_input();
    std::cout << "Некорректный ввод. Попробуйте еще раз: ";
  }

  return value;
}

int read_non_negative_count() {
  int count = read_int();

  if (count < 0) {
    throw std::out_of_range("Количество должно быть больше либо равно нулю");
  }

  return count;
}

int* read_int_items(int count) {
  if (count == 0) return nullptr;

  int* items = new int[count];

  for (int i = 0; i < count; i++) {
    std::cout << "Элемент " << i << ": ";
    items[i] = read_int();
  }

  return items;
}

void print_dynamic_array(const DynamicArray<int>* array) {
  std::cout << "[";

  for (int i = 0; i < array->get_size(); i++) {
    if (i > 0) std::cout << ", ";
    std::cout << array->get(i);
  }

  std::cout << "]";
}

void print_linked_list(const LinkedList<int>* list) {
  std::cout << "[";

  for (int i = 0; i < list->get_length(); i++) {
    if (i > 0) std::cout << ", ";
    std::cout << *list->get(i);
  }

  std::cout << "]";
}

void print_sequence(const Sequence<int>* sequence) {
  std::cout << "[";

  for (int i = 0; i < sequence->get_length(); i++) {
    if (i > 0) std::cout << ", ";
    std::cout << sequence->get(i);
  }

  std::cout << "]";
}

void print_pair_sequence(const Sequence<Pair<int, int>>* sequence) {
  std::cout << "[";

  for (int i = 0; i < sequence->get_length(); i++) {
    if (i > 0) std::cout << ", ";
    Pair<int, int> pair = sequence->get(i);
    std::cout << "(" << pair.first() << ", " << pair.second() << ")";
  }

  std::cout << "]";
}

void print_option(const Option<int>& option) {
  if (option.has_value()) {
    std::cout << "Some(" << option.get_value() << ")";
  } else {
    std::cout << "None";
  }
}

void print_double_sequence(const Sequence<double>* sequence) {
  std::cout << "[";

  for (int i = 0; i < sequence->get_length(); i++) {
    if (i > 0) std::cout << ", ";
    std::cout << sequence->get(i);
  }

  std::cout << "]";
}

void print_sequence_of_sequences(const Sequence<Sequence<int>*>* sequenceList) {
  std::cout << "[";

  for (int i = 0; i < sequenceList->get_length(); i++) {
    if (i > 0) std::cout << ", ";
    print_sequence(sequenceList->get(i));
  }

  std::cout << "]";
}

void print_bit_sequence(const BitSequence* sequence) {
  std::cout << "[";

  for (int i = 0; i < sequence->get_length(); i++) {
    if (i > 0) std::cout << ", ";
    std::cout << sequence->get(i);
  }

  std::cout << "]";
}

void print_bit_option(const Option<bool>& option) {
  if (option.has_value()) {
    std::cout << "Some(" << option.get_value() << ")";
  } else {
    std::cout << "None";
  }
}

void replace_sequence_if_needed(int index, Sequence<int>* result) {
  if (result == sequences[index]) return;

  delete sequences[index];
  sequences[index] = result;
}

int select_dynamic_array(const char* title) {
  if (dynamicArrayCount == 0) {
    std::cout << "Объекты DynamicArray еще не созданы\n";
    return -1;
  }

  std::cout << title << '\n';

  for (int i = 0; i < dynamicArrayCount; i++) {
    std::cout << i << ": ";
    print_dynamic_array(dynamicArrays[i]);
    std::cout << '\n';
  }

  std::cout << "Индекс: ";
  int index = read_int();

  if (index < 0 || index >= dynamicArrayCount) {
    std::cout << "Некорректный индекс\n";
    return -1;
  }

  return index;
}

int select_linked_list(const char* title) {
  if (linkedListCount == 0) {
    std::cout << "Объекты LinkedList еще не созданы\n";
    return -1;
  }

  std::cout << title << '\n';

  for (int i = 0; i < linkedListCount; i++) {
    std::cout << i << ": ";
    print_linked_list(linkedLists[i]);
    std::cout << '\n';
  }

  std::cout << "Индекс: ";
  int index = read_int();

  if (index < 0 || index >= linkedListCount) {
    std::cout << "Некорректный индекс\n";
    return -1;
  }

  return index;
}

int select_sequence(const char* title) {
  if (sequenceCount == 0) {
    std::cout << "Объекты Sequence<int> еще не созданы\n";
    return -1;
  }

  std::cout << title << '\n';

  for (int i = 0; i < sequenceCount; i++) {
    std::cout << i << ": ";
    print_sequence(sequences[i]);
    std::cout << '\n';
  }

  std::cout << "Индекс: ";
  int index = read_int();

  if (index < 0 || index >= sequenceCount) {
    std::cout << "Некорректный индекс\n";
    return -1;
  }

  return index;
}

int select_bit_sequence(const char* title) {
  if (bitSequenceCount == 0) {
    std::cout << "Объекты BitSequence еще не созданы\n";
    return -1;
  }

  std::cout << title << '\n';

  for (int i = 0; i < bitSequenceCount; i++) {
    std::cout << i << ": ";
    print_bit_sequence(bitSequences[i]);
    std::cout << '\n';
  }

  std::cout << "Индекс: ";
  int index = read_int();

  if (index < 0 || index >= bitSequenceCount) {
    std::cout << "Некорректный индекс\n";
    return -1;
  }

  return index;
}

void create_dynamic_array() {
  if (dynamicArrayCount >= MAX_OBJECTS) {
    std::cout << "Хранилище DynamicArray заполнено\n";
    return;
  }

  std::cout << "Введите размер DynamicArray: ";
  int count = read_non_negative_count();
  int* items = read_int_items(count);

  dynamicArrays[dynamicArrayCount] = new DynamicArray<int>(items, count);
  delete[] items;

  std::cout << "DynamicArray создан с индексом " << dynamicArrayCount << '\n';
  dynamicArrayCount++;
}

void dynamic_array_operations() {
  int index = select_dynamic_array("Выберите DynamicArray:");
  if (index == -1) return;

  std::cout << "1. Получить элемент (get)\n";
  std::cout << "2. Изменить элемент (set)\n";
  std::cout << "3. Изменить размер (resize)\n";
  std::cout << "4. Прочитать operator[]\n";
  std::cout << "5. Записать через operator[]\n";
  std::cout << "6. Получить размер (get_size)\n";
  std::cout << "Выбор: ";
  int choice = read_int();

  if (choice == 1) {
    std::cout << "Индекс элемента: ";
    int itemIndex = read_int();
    std::cout << "Значение: " << dynamicArrays[index]->get(itemIndex) << '\n';
  } else if (choice == 2) {
    std::cout << "Индекс элемента: ";
    int itemIndex = read_int();
    std::cout << "Новое значение: ";
    int value = read_int();
    dynamicArrays[index]->set(itemIndex, value);
    std::cout << "Обновлено: ";
    print_dynamic_array(dynamicArrays[index]);
    std::cout << '\n';
  } else if (choice == 3) {
    std::cout << "Новый размер: ";
    int size = read_non_negative_count();
    dynamicArrays[index]->resize(size);
    std::cout << "Размер изменен: ";
    print_dynamic_array(dynamicArrays[index]);
    std::cout << '\n';
  } else if (choice == 4) {
    std::cout << "Индекс элемента: ";
    int itemIndex = read_int();
    std::cout << "Значение: " << (*dynamicArrays[index])[itemIndex] << '\n';
  } else if (choice == 5) {
    std::cout << "Индекс элемента: ";
    int itemIndex = read_int();
    std::cout << "Новое значение: ";
    int value = read_int();
    (*dynamicArrays[index])[itemIndex] = value;
    std::cout << "Обновлено: ";
    print_dynamic_array(dynamicArrays[index]);
    std::cout << '\n';
  } else if (choice == 6) {
    std::cout << "Размер: " << dynamicArrays[index]->get_size() << '\n';
  } else {
    std::cout << "Некорректный выбор\n";
  }
}

void create_linked_list() {
  if (linkedListCount >= MAX_OBJECTS) {
    std::cout << "Хранилище LinkedList заполнено\n";
    return;
  }

  std::cout << "Введите размер LinkedList: ";
  int count = read_non_negative_count();
  int* items = read_int_items(count);

  linkedLists[linkedListCount] = new LinkedList<int>(items, count);
  delete[] items;

  std::cout << "LinkedList создан с индексом " << linkedListCount << '\n';
  linkedListCount++;
}

void linked_list_operations() {
  int index = select_linked_list("Выберите LinkedList:");
  if (index == -1) return;

  std::cout << "1. Получить первый элемент (get_first)\n";
  std::cout << "2. Получить последний элемент (get_last)\n";
  std::cout << "3. Получить элемент (get)\n";
  std::cout << "4. Получить длину (get_length)\n";
  std::cout << "5. Добавить в конец (append)\n";
  std::cout << "6. Добавить в начало (prepend)\n";
  std::cout << "7. Вставить по индексу (insert_at)\n";
  std::cout << "8. Получить подсписок (get_sub_list)\n";
  std::cout << "9. Объединить списки (concat)\n";
  std::cout << "10. Прочитать operator[]\n";
  std::cout << "11. Записать через operator[]\n";
  std::cout << "Выбор: ";
  int choice = read_int();

  if (choice == 1) {
    std::cout << "Значение: " << linkedLists[index]->get_first() << '\n';
  } else if (choice == 2) {
    std::cout << "Значение: " << linkedLists[index]->get_last() << '\n';
  } else if (choice == 3) {
    std::cout << "Индекс элемента: ";
    int itemIndex = read_int();
    std::cout << "Значение: " << *linkedLists[index]->get(itemIndex) << '\n';
  } else if (choice == 4) {
    std::cout << "Длина: " << linkedLists[index]->get_length() << '\n';
  } else if (choice == 5) {
    std::cout << "Значение: ";
    linkedLists[index]->append(read_int());
  } else if (choice == 6) {
    std::cout << "Значение: ";
    linkedLists[index]->prepend(read_int());
  } else if (choice == 7) {
    std::cout << "Индекс элемента: ";
    int itemIndex = read_int();
    std::cout << "Значение: ";
    int value = read_int();
    linkedLists[index]->insert_at(value, itemIndex);
  } else if (choice == 8) {
    if (linkedListCount >= MAX_OBJECTS) {
      std::cout << "Хранилище LinkedList заполнено\n";
      return;
    }

    std::cout << "Начальный индекс: ";
    int startIndex = read_int();
    std::cout << "Конечный индекс: ";
    int endIndex = read_int();
    linkedLists[linkedListCount] = linkedLists[index]->get_sub_list(startIndex, endIndex);
    std::cout << "Подсписок сохранен с индексом " << linkedListCount << '\n';
    linkedListCount++;
  } else if (choice == 9) {
    if (linkedListCount >= MAX_OBJECTS) {
      std::cout << "Хранилище LinkedList заполнено\n";
      return;
    }

    int secondIndex = select_linked_list("Выберите второй LinkedList:");
    if (secondIndex == -1) return;

    linkedLists[linkedListCount] = linkedLists[index]->concat(linkedLists[secondIndex]);
    std::cout << "Результат concat сохранен с индексом " << linkedListCount << '\n';
    linkedListCount++;
  } else if (choice == 10) {
    std::cout << "Индекс элемента: ";
    int itemIndex = read_int();
    std::cout << "Значение: " << (*linkedLists[index])[itemIndex] << '\n';
  } else if (choice == 11) {
    std::cout << "Индекс элемента: ";
    int itemIndex = read_int();
    std::cout << "Новое значение: ";
    int value = read_int();
    (*linkedLists[index])[itemIndex] = value;
  } else {
    std::cout << "Некорректный выбор\n";
    return;
  }

  std::cout << "Текущее значение: ";
  print_linked_list(linkedLists[index]);
  std::cout << '\n';
}

void create_sequence() {
  if (sequenceCount >= MAX_OBJECTS) {
    std::cout << "Хранилище Sequence заполнено\n";
    return;
  }

  std::cout << "1. MutableArraySequence<int>\n";
  std::cout << "2. ImmutableArraySequence<int>\n";
  std::cout << "3. MutableListSequence<int>\n";
  std::cout << "4. ImmutableListSequence<int>\n";
  std::cout << "Выбор: ";
  int choice = read_int();

  std::cout << "Введите размер Sequence: ";
  int count = read_non_negative_count();
  int* items = read_int_items(count);

  Sequence<int>* sequence = nullptr;

  if (choice == 1) {
    sequence = new MutableArraySequence<int>(items, count);
  } else if (choice == 2) {
    sequence = new ImmutableArraySequence<int>(items, count);
  } else if (choice == 3) {
    sequence = new MutableListSequence<int>(items, count);
  } else if (choice == 4) {
    sequence = new ImmutableListSequence<int>(items, count);
  } else {
    delete[] items;
    std::cout << "Некорректный выбор\n";
    return;
  }

  delete[] items;
  sequences[sequenceCount] = sequence;
  std::cout << "Sequence создан с индексом " << sequenceCount << '\n';
  sequenceCount++;
}

void sequence_operations() {
  int index = select_sequence("Выберите Sequence<int>:");
  if (index == -1) return;

  std::cout << "1. Получить первый элемент (get_first)\n";
  std::cout << "2. Получить последний элемент (get_last)\n";
  std::cout << "3. Получить элемент (get)\n";
  std::cout << "4. Безопасно получить первый (try_get_first)\n";
  std::cout << "5. Безопасно получить последний (try_get_last)\n";
  std::cout << "6. Безопасно получить элемент (try_get)\n";
  std::cout << "7. Найти положительный (try_find)\n";
  std::cout << "8. Получить длину (get_length)\n";
  std::cout << "9. Добавить в конец (append)\n";
  std::cout << "10. Добавить в начало (prepend)\n";
  std::cout << "11. Вставить по индексу (insert_at)\n";
  std::cout << "12. Получить подпоследовательность (get_sub_sequence)\n";
  std::cout << "13. Объединить (concat)\n";
  std::cout << "14. Map: квадрат\n";
  std::cout << "15. Map: значение + индекс\n";
  std::cout << "16. Where: положительные\n";
  std::cout << "17. Reduce: сумма\n";
  std::cout << "18. Прочитать operator[]\n";
  std::cout << "19. Записать через operator[] (только mutable)\n";
  std::cout << "Выбор: ";
  int choice = read_int();

  if (choice == 1) {
    std::cout << "Значение: " << sequences[index]->get_first() << '\n';
  } else if (choice == 2) {
    std::cout << "Значение: " << sequences[index]->get_last() << '\n';
  } else if (choice == 3) {
    std::cout << "Индекс элемента: ";
    int itemIndex = read_int();
    std::cout << "Значение: " << sequences[index]->get(itemIndex) << '\n';
  } else if (choice == 4) {
    std::cout << "Значение: ";
    print_option(sequences[index]->try_get_first());
    std::cout << '\n';
  } else if (choice == 5) {
    std::cout << "Значение: ";
    print_option(sequences[index]->try_get_last());
    std::cout << '\n';
  } else if (choice == 6) {
    std::cout << "Индекс элемента: ";
    int itemIndex = read_int();
    std::cout << "Значение: ";
    print_option(sequences[index]->try_get(itemIndex));
    std::cout << '\n';
  } else if (choice == 7) {
    std::cout << "Значение: ";
    print_option(sequences[index]->try_find(is_positive));
    std::cout << '\n';
  } else if (choice == 8) {
    std::cout << "Длина: " << sequences[index]->get_length() << '\n';
  } else if (choice == 9) {
    std::cout << "Значение: ";
    replace_sequence_if_needed(index, sequences[index]->append(read_int()));
  } else if (choice == 10) {
    std::cout << "Значение: ";
    replace_sequence_if_needed(index, sequences[index]->prepend(read_int()));
  } else if (choice == 11) {
    std::cout << "Индекс элемента: ";
    int itemIndex = read_int();
    std::cout << "Значение: ";
    int value = read_int();
    replace_sequence_if_needed(index, sequences[index]->insert_at(value, itemIndex));
  } else if (choice == 12) {
    if (sequenceCount >= MAX_OBJECTS) {
      std::cout << "Хранилище Sequence заполнено\n";
      return;
    }

    std::cout << "Начальный индекс: ";
    int startIndex = read_int();
    std::cout << "Конечный индекс: ";
    int endIndex = read_int();
    sequences[sequenceCount] = sequences[index]->get_sub_sequence(startIndex, endIndex);
    std::cout << "Подпоследовательность сохранена с индексом " << sequenceCount << '\n';
    sequenceCount++;
  } else if (choice == 13) {
    if (sequenceCount >= MAX_OBJECTS) {
      std::cout << "Хранилище Sequence заполнено\n";
      return;
    }

    int secondIndex = select_sequence("Выберите второй Sequence<int>:");
    if (secondIndex == -1) return;

    sequences[sequenceCount] = sequences[index]->concat(*sequences[secondIndex]);
    std::cout << "Результат concat сохранен с индексом " << sequenceCount << '\n';
    sequenceCount++;
  } else if (choice == 14) {
    if (sequenceCount >= MAX_OBJECTS) {
      std::cout << "Хранилище Sequence заполнено\n";
      return;
    }

    sequences[sequenceCount] = sequences[index]->map(square);
    std::cout << "Результат map сохранен с индексом " << sequenceCount << '\n';
    sequenceCount++;
  } else if (choice == 15) {
    if (sequenceCount >= MAX_OBJECTS) {
      std::cout << "Хранилище Sequence заполнено\n";
      return;
    }

    sequences[sequenceCount] = sequences[index]->map(add_index);
    std::cout << "Результат map сохранен с индексом " << sequenceCount << '\n';
    sequenceCount++;
  } else if (choice == 16) {
    if (sequenceCount >= MAX_OBJECTS) {
      std::cout << "Хранилище Sequence заполнено\n";
      return;
    }

    sequences[sequenceCount] = sequences[index]->where(is_positive);
    std::cout << "Результат where сохранен с индексом " << sequenceCount << '\n';
    sequenceCount++;
  } else if (choice == 17) {
    std::cout << "Результат reduce суммы: " << sequences[index]->reduce(sum, 0) << '\n';
  } else if (choice == 18) {
    std::cout << "Индекс элемента: ";
    int itemIndex = read_int();
    std::cout << "Значение: " << (*sequences[index])[itemIndex] << '\n';
  } else if (choice == 19) {
    std::cout << "Индекс элемента: ";
    int itemIndex = read_int();
    std::cout << "Новое значение: ";
    int value = read_int();

    MutableArraySequence<int>* arraySequence = dynamic_cast<MutableArraySequence<int>*>(sequences[index]);
    MutableListSequence<int>* listSequence = dynamic_cast<MutableListSequence<int>*>(sequences[index]);

    if (arraySequence != nullptr) {
      (*arraySequence)[itemIndex] = value;
    } else if (listSequence != nullptr) {
      (*listSequence)[itemIndex] = value;
    } else {
      std::cout << "Запись через operator[] недоступна для immutable-последовательности\n";
    }
  } else {
    std::cout << "Некорректный выбор\n";
    return;
  }

  std::cout << "Текущее значение: ";
  print_sequence(sequences[index]);
  std::cout << '\n';
}

void create_bit_sequence() {
  if (bitSequenceCount >= MAX_OBJECTS) {
    std::cout << "Хранилище BitSequence заполнено\n";
    return;
  }

  std::cout << "Введите размер BitSequence: ";
  int count = read_non_negative_count();

  bool* items = nullptr;
  if (count > 0) {
    items = new bool[count];
  }

  for (int i = 0; i < count; i++) {
    std::cout << "Бит " << i << " (0/1): ";
    items[i] = read_int() != 0;
  }

  bitSequences[bitSequenceCount] = new BitSequence(items, count);
  delete[] items;

  std::cout << "BitSequence создан с индексом " << bitSequenceCount << '\n';
  bitSequenceCount++;
}

void bit_sequence_operations() {
  int index = select_bit_sequence("Выберите BitSequence:");
  if (index == -1) return;

  std::cout << "1. Получить первый бит (get_first)\n";
  std::cout << "2. Получить последний бит (get_last)\n";
  std::cout << "3. Получить бит (get)\n";
  std::cout << "4. Безопасно получить первый (try_get_first)\n";
  std::cout << "5. Безопасно получить последний (try_get_last)\n";
  std::cout << "6. Безопасно получить бит (try_get)\n";
  std::cout << "7. Получить длину (get_length)\n";
  std::cout << "8. Добавить в конец (append)\n";
  std::cout << "9. Добавить в начало (prepend)\n";
  std::cout << "10. Вставить по индексу (insert_at)\n";
  std::cout << "11. Получить подпоследовательность (get_sub_sequence)\n";
  std::cout << "12. Побитовое И (AND)\n";
  std::cout << "13. Побитовое ИЛИ (OR)\n";
  std::cout << "14. Исключающее ИЛИ (XOR)\n";
  std::cout << "15. Побитовое НЕ (NOT)\n";
  std::cout << "16. Прочитать operator[]\n";
  std::cout << "17. Записать через operator[]\n";
  std::cout << "Выбор: ";
  int choice = read_int();

  if (choice == 1) {
    std::cout << "Значение: " << bitSequences[index]->get_first() << '\n';
  } else if (choice == 2) {
    std::cout << "Значение: " << bitSequences[index]->get_last() << '\n';
  } else if (choice == 3) {
    std::cout << "Индекс бита: ";
    int itemIndex = read_int();
    std::cout << "Значение: " << bitSequences[index]->get(itemIndex) << '\n';
  } else if (choice == 4) {
    std::cout << "Значение: ";
    print_bit_option(bitSequences[index]->try_get_first());
    std::cout << '\n';
  } else if (choice == 5) {
    std::cout << "Значение: ";
    print_bit_option(bitSequences[index]->try_get_last());
    std::cout << '\n';
  } else if (choice == 6) {
    std::cout << "Индекс бита: ";
    int itemIndex = read_int();
    std::cout << "Значение: ";
    print_bit_option(bitSequences[index]->try_get(itemIndex));
    std::cout << '\n';
  } else if (choice == 7) {
    std::cout << "Длина: " << bitSequences[index]->get_length() << '\n';
  } else if (choice == 8) {
    std::cout << "Значение бита (0/1): ";
    bitSequences[index]->append(read_int() != 0);
  } else if (choice == 9) {
    std::cout << "Значение бита (0/1): ";
    bitSequences[index]->prepend(read_int() != 0);
  } else if (choice == 10) {
    std::cout << "Индекс бита: ";
    int itemIndex = read_int();
    std::cout << "Значение бита (0/1): ";
    bitSequences[index]->insert_at(read_int() != 0, itemIndex);
  } else if (choice == 11) {
    if (bitSequenceCount >= MAX_OBJECTS) {
      std::cout << "Хранилище BitSequence заполнено\n";
      return;
    }

    std::cout << "Начальный индекс: ";
    int startIndex = read_int();
    std::cout << "Конечный индекс: ";
    int endIndex = read_int();
    Sequence<bool>* subsequence = bitSequences[index]->get_sub_sequence(startIndex, endIndex);
    bitSequences[bitSequenceCount] = static_cast<BitSequence*>(subsequence);
    std::cout << "Подпоследовательность сохранена с индексом " << bitSequenceCount << '\n';
    bitSequenceCount++;
  } else if (choice >= 12 && choice <= 14) {
    if (bitSequenceCount >= MAX_OBJECTS) {
      std::cout << "Хранилище BitSequence заполнено\n";
      return;
    }

    int secondIndex = select_bit_sequence("Выберите второй BitSequence:");
    if (secondIndex == -1) return;

    if (choice == 12) {
      bitSequences[bitSequenceCount] = bitSequences[index]->bit_and(bitSequences[secondIndex]);
    } else if (choice == 13) {
      bitSequences[bitSequenceCount] = bitSequences[index]->bit_or(bitSequences[secondIndex]);
    } else {
      bitSequences[bitSequenceCount] = bitSequences[index]->bit_xor(bitSequences[secondIndex]);
    }

    std::cout << "Результат битовой операции сохранен с индексом " << bitSequenceCount << '\n';
    bitSequenceCount++;
  } else if (choice == 15) {
    if (bitSequenceCount >= MAX_OBJECTS) {
      std::cout << "Хранилище BitSequence заполнено\n";
      return;
    }

    bitSequences[bitSequenceCount] = bitSequences[index]->bit_not();
    std::cout << "Результат NOT сохранен с индексом " << bitSequenceCount << '\n';
    bitSequenceCount++;
  } else if (choice == 16) {
    std::cout << "Индекс бита: ";
    int itemIndex = read_int();
    const BitSequence* sequence = bitSequences[index];
    std::cout << "Значение: " << (*sequence)[itemIndex] << '\n';
  } else if (choice == 17) {
    std::cout << "Индекс бита: ";
    int itemIndex = read_int();
    std::cout << "Значение бита (0/1): ";
    (*bitSequences[index])[itemIndex] = read_int() != 0;
  } else {
    std::cout << "Некорректный выбор\n";
    return;
  }

  std::cout << "Текущее значение: ";
  print_bit_sequence(bitSequences[index]);
  std::cout << '\n';
}

void print_all_objects() {
  std::cout << "\nОбъекты DynamicArray:\n";
  for (int i = 0; i < dynamicArrayCount; i++) {
    std::cout << i << ": ";
    print_dynamic_array(dynamicArrays[i]);
    std::cout << '\n';
  }

  std::cout << "\nОбъекты LinkedList:\n";
  for (int i = 0; i < linkedListCount; i++) {
    std::cout << i << ": ";
    print_linked_list(linkedLists[i]);
    std::cout << '\n';
  }

  std::cout << "\nОбъекты Sequence<int>:\n";
  for (int i = 0; i < sequenceCount; i++) {
    std::cout << i << ": ";
    print_sequence(sequences[i]);
    std::cout << '\n';
  }

  std::cout << "\nОбъекты BitSequence:\n";
  for (int i = 0; i < bitSequenceCount; i++) {
    std::cout << i << ": ";
    print_bit_sequence(bitSequences[i]);
    std::cout << '\n';
  }
}

void run_auto_demo() {
  if (sequenceCount + 2 > MAX_OBJECTS || bitSequenceCount + 2 > MAX_OBJECTS) {
    std::cout << "Недостаточно места для автодемо\n";
    return;
  }

  int values[] = {1, -2, 3, 4};
  sequences[sequenceCount] = new MutableArraySequence<int>(values, 4);
  std::cout << "Авто: Sequence<int> создан с индексом " << sequenceCount << '\n';
  sequenceCount++;

  sequences[sequenceCount] = sequences[sequenceCount - 1]->map(square);
  std::cout << "Авто: результат map(square) сохранен с индексом " << sequenceCount << '\n';
  sequenceCount++;

  bool bits[] = {true, false, true, true};
  bitSequences[bitSequenceCount] = new BitSequence(bits, 4);
  std::cout << "Авто: BitSequence создан с индексом " << bitSequenceCount << '\n';
  bitSequenceCount++;

  bitSequences[bitSequenceCount] = bitSequences[bitSequenceCount - 1]->bit_not();
  std::cout << "Авто: результат NOT сохранен с индексом " << bitSequenceCount << '\n';
  bitSequenceCount++;
}

void map_reduce_operations() {
  std::cout << "1. zip двух Sequence<int>\n";
  std::cout << "2. unzip пар, созданных из двух Sequence<int>\n";
  std::cout << "3. skip\n";
  std::cout << "4. split по предикату положительности\n";
  std::cout << "5. slice без замены\n";
  std::cout << "6. slice с заменой на Sequence<int>\n";
  std::cout << "7. flat_map: значение в [value, -value]\n";
  std::cout << "Выбор: ";
  int choice = read_int();

  if (choice == 1) {
    int firstIndex = select_sequence("Выберите первый Sequence<int>:");
    if (firstIndex == -1) return;
    int secondIndex = select_sequence("Выберите второй Sequence<int>:");
    if (secondIndex == -1) return;

    Sequence<Pair<int, int>>* result = zip(*sequences[firstIndex], *sequences[secondIndex]);
    std::cout << "Результат zip: ";
    print_pair_sequence(result);
    std::cout << '\n';
    delete result;
  } else if (choice == 2) {
    int firstIndex = select_sequence("Выберите первый Sequence<int> для пар:");
    if (firstIndex == -1) return;
    int secondIndex = select_sequence("Выберите второй Sequence<int> для пар:");
    if (secondIndex == -1) return;

    Sequence<Pair<int, int>>* zipped = zip(*sequences[firstIndex], *sequences[secondIndex]);
    Pair<Sequence<int>*, Sequence<int>*> result = unzip(*zipped);

    std::cout << "Первые значения: ";
    print_sequence(result.first());
    std::cout << '\n';
    std::cout << "Вторые значения: ";
    print_sequence(result.second());
    std::cout << '\n';

    delete zipped;
    delete result.first();
    delete result.second();
  } else if (choice == 3) {
    if (sequenceCount >= MAX_OBJECTS) {
      std::cout << "Хранилище Sequence заполнено\n";
      return;
    }

    int index = select_sequence("Выберите Sequence<int>:");
    if (index == -1) return;
    std::cout << "Сколько элементов пропустить: ";
    sequences[sequenceCount] = skip(*sequences[index], read_int());
    std::cout << "Результат skip сохранен с индексом " << sequenceCount << '\n';
    sequenceCount++;
  } else if (choice == 4) {
    int index = select_sequence("Выберите Sequence<int>:");
    if (index == -1) return;

    Pair<Sequence<int>*, Sequence<int>*> result = split(*sequences[index], is_positive);
    std::cout << "Положительные значения: ";
    print_sequence(result.first());
    std::cout << '\n';
    std::cout << "Остальные значения: ";
    print_sequence(result.second());
    std::cout << '\n';
    delete result.first();
    delete result.second();
  } else if (choice == 5 || choice == 6) {
    if (sequenceCount >= MAX_OBJECTS) {
      std::cout << "Хранилище Sequence заполнено\n";
      return;
    }

    int index = select_sequence("Выберите Sequence<int>:");
    if (index == -1) return;

    const Sequence<int>* replacement = nullptr;
    if (choice == 6) {
      int replacementIndex = select_sequence("Выберите Sequence<int> для замены:");
      if (replacementIndex == -1) return;
      replacement = sequences[replacementIndex];
    }

    std::cout << "Начальный индекс (может быть отрицательным): ";
    int startIndex = read_int();
    std::cout << "Сколько элементов удалить: ";
    int count = read_int();
    sequences[sequenceCount] = slice(*sequences[index], startIndex, count, replacement);
    std::cout << "Результат slice сохранен с индексом " << sequenceCount << '\n';
    sequenceCount++;
  } else if (choice == 7) {
    if (sequenceCount >= MAX_OBJECTS) {
      std::cout << "Хранилище Sequence заполнено\n";
      return;
    }

    int index = select_sequence("Выберите Sequence<int>:");
    if (index == -1) return;
    sequences[sequenceCount] = flat_map(*sequences[index], expand_with_negative);
    std::cout << "Результат flat_map сохранен с индексом " << sequenceCount << '\n';
    sequenceCount++;
  } else {
    std::cout << "Некорректный выбор\n";
  }
}

void clear_objects() {
  for (int i = 0; i < dynamicArrayCount; i++) {
    delete dynamicArrays[i];
    dynamicArrays[i] = nullptr;
  }

  for (int i = 0; i < linkedListCount; i++) {
    delete linkedLists[i];
    linkedLists[i] = nullptr;
  }

  for (int i = 0; i < sequenceCount; i++) {
    delete sequences[i];
    sequences[i] = nullptr;
  }

  for (int i = 0; i < bitSequenceCount; i++) {
    delete bitSequences[i];
    bitSequences[i] = nullptr;
  }
}

void print_menu() {
  std::cout << "1. Модуль DynamicArray<int>\n";
  std::cout << "2. Модуль LinkedList<int>\n";
  std::cout << "3. Модуль Sequence<int>\n";
  std::cout << "4. Модуль MapReduce\n";
  std::cout << "5. Модуль BitSequence\n";
  std::cout << "6. Показать все объекты\n";
  std::cout << "0. Выход\n";
  std::cout << "Выбор: ";
}

} // namespace

void run_menu() {
  SetConsoleCP(CP_UTF8);
  SetConsoleOutputCP(CP_UTF8);

  int choice = -1;

  while (choice != 0) {
    print_menu();
    choice = read_int();

    try {
      if (choice == 1) {
        run_dynamic_array_menu();
      } else if (choice == 2) {
        run_linked_list_menu();
      } else if (choice == 3) {
        run_sequence_menu();
      } else if (choice == 4) {
        run_map_reduce_menu();
      } else if (choice == 5) {
        run_bit_sequence_menu();
      } else if (choice == 6) {
        print_all_objects();
      } else if (choice == 0) {
        std::cout << "Выход\n";
      } else {
        std::cout << "Некорректный выбор\n";
      }
    } catch (const std::exception& exception) {
      std::cout << "Ошибка: " << exception.what() << '\n';
    }
  }

  clear_objects();
}

void run_dynamic_array_menu() {
  int choice = -1;

  while (choice != 0) {
    std::cout << "1. Создать DynamicArray<int>\n";
    std::cout << "2. Запустить функцию DynamicArray\n";
    std::cout << "3. Показать все объекты\n";
    std::cout << "0. Назад\n";
    std::cout << "Выбор: ";
    choice = read_int();

    try {
      if (choice == 1) {
        create_dynamic_array();
      } else if (choice == 2) {
        dynamic_array_operations();
      } else if (choice == 3) {
        print_all_objects();
      } else if (choice != 0) {
        std::cout << "Некорректный выбор\n";
      }
    } catch (const std::exception& exception) {
      std::cout << "Ошибка: " << exception.what() << '\n';
    }
  }
}

void run_linked_list_menu() {
  int choice = -1;

  while (choice != 0) {
    std::cout << "1. Создать LinkedList<int>\n";
    std::cout << "2. Запустить функцию LinkedList\n";
    std::cout << "3. Показать все объекты\n";
    std::cout << "0. Назад\n";
    std::cout << "Выбор: ";
    choice = read_int();

    try {
      if (choice == 1) {
        create_linked_list();
      } else if (choice == 2) {
        linked_list_operations();
      } else if (choice == 3) {
        print_all_objects();
      } else if (choice != 0) {
        std::cout << "Некорректный выбор\n";
      }
    } catch (const std::exception& exception) {
      std::cout << "Ошибка: " << exception.what() << '\n';
    }
  }
}

void run_sequence_menu() {
  int choice = -1;

  while (choice != 0) {
    std::cout << "1. Создать Sequence<int>\n";
    std::cout << "2. Запустить функцию Sequence\n";
    std::cout << "3. Показать все объекты\n";
    std::cout << "0. Назад\n";
    std::cout << "Выбор: ";
    choice = read_int();

    try {
      if (choice == 1) {
        create_sequence();
      } else if (choice == 2) {
        sequence_operations();
      } else if (choice == 3) {
        print_all_objects();
      } else if (choice != 0) {
        std::cout << "Некорректный выбор\n";
      }
    } catch (const std::exception& exception) {
      std::cout << "Ошибка: " << exception.what() << '\n';
    }
  }
}

void run_map_reduce_menu() {
  int choice = -1;

  while (choice != 0) {
    std::cout << "1. Создать Sequence<int>\n";
    std::cout << "2. Запустить функцию MapReduce\n";
    std::cout << "3. Показать все объекты\n";
    std::cout << "0. Назад\n";
    std::cout << "Выбор: ";
    choice = read_int();

    try {
      if (choice == 1) {
        create_sequence();
      } else if (choice == 2) {
        map_reduce_operations();
      } else if (choice == 3) {
        print_all_objects();
      } else if (choice != 0) {
        std::cout << "Некорректный выбор\n";
      }
    } catch (const std::exception& exception) {
      std::cout << "Ошибка: " << exception.what() << '\n';
    }
  }
}

void run_bit_sequence_menu() {
  int choice = -1;

  while (choice != 0) {
    std::cout << "1. Создать BitSequence\n";
    std::cout << "2. Запустить функцию BitSequence\n";
    std::cout << "3. Показать все объекты\n";
    std::cout << "0. Назад\n";
    std::cout << "Выбор: ";
    choice = read_int();

    try {
      if (choice == 1) {
        create_bit_sequence();
      } else if (choice == 2) {
        bit_sequence_operations();
      } else if (choice == 3) {
        print_all_objects();
      } else if (choice != 0) {
        std::cout << "Некорректный выбор\n";
      }
    } catch (const std::exception& exception) {
      std::cout << "Ошибка: " << exception.what() << '\n';
    }
  }
}
