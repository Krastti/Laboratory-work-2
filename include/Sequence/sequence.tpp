#include <stdexcept>
#include "sequence.h"

// TODO Необходимо улучшить оптимизацию данной функции
/*
 * Если создать последовательно размером миллион и взять элемент с 900000 до 900001, то нынешний алгоритм
 * пройдется по всей последовательность до 900000
 */
template <class T>
Sequence<T>* Sequence<T>::get_sub_sequence(int startIndex, int endIndex) const {
  int length = get_length();

  if (startIndex < 0) throw std::out_of_range("Индекс вне допустимого диапазона");
  if (endIndex < 0) throw std::out_of_range("Индекс вне допустимого диапазона");
  if (startIndex >= length) throw std::out_of_range("Индекс вне допустимого диапазона");
  if (endIndex >= length) throw std::out_of_range("Индекс вне допустимого диапазона");
  if (startIndex > endIndex) throw std::out_of_range("Индекс вне допустимого диапазона");

  Sequence<T>* result = new_empty_instance();
  EnumeratorWrapper<T> iter = get_enumerator();
  int index = 0;

  while (iter.move_next()) {
    if (index >= startIndex && index <= endIndex) {
      result->sys_append(iter.get_current());
    }

    if (index == endIndex) {
      break;
    }

    index++;
  }

  return result;
}

/*
 * Когда передается указатель в функцию, то можно передать nullptr, т.е в теории объекта может и не быть
 * Если же передавать ссылку, то мы говорим компилятору, что объект обязательно существует
 */

template <class T>
Sequence<T>* Sequence<T>::concat(const Sequence<T> &other) const {
  Sequence<T>* result = new_empty_instance();
  EnumeratorWrapper<T> firstIter = get_enumerator();

  while (firstIter.move_next()) {
    result->sys_append(firstIter.get_current());
  }

  EnumeratorWrapper<T> secondIter = other.get_enumerator();

  while (secondIter.move_next()) {
    result->sys_append(secondIter.get_current());
  }

  return result;
}

template <class T>
Sequence<T>* Sequence<T>::map(T (*func)(const T &item)) const {
  if (func == nullptr) throw std::invalid_argument("Нельзя выполнить map с нулевой функцией");

  Sequence<T>* result = new_empty_instance();
  EnumeratorWrapper<T> iter = get_enumerator();

  while (iter.move_next()) {
    result->sys_append(func(iter.get_current()));
  }

  return result;
}

template <class T>
Sequence<T>* Sequence<T>::map(T (*func)(const T &item, int index)) const {
  if (func == nullptr) throw std::invalid_argument("Нельзя выполнить map с нулевой функцией");

  Sequence<T>* result = new_empty_instance();
  EnumeratorWrapper<T> iter = get_enumerator();
  int index = 0;

  while (iter.move_next()) {
    result->sys_append(func(iter.get_current(), index));
    index++;
  }

  return result;
}

template <class T>
Sequence<T>* Sequence<T>::where(bool (*predicate)(const T &element)) const {
  if (predicate == nullptr) throw std::invalid_argument("Нельзя выполнить where с нулевым предикатом");

  Sequence<T>* result = new_empty_instance();
  EnumeratorWrapper<T> iter = get_enumerator();

  while (iter.move_next()) {
    const T& item = iter.get_current();

    if (predicate(item)) {
      result->sys_append(item);
    }
  }

  return result;
}

template <class T>
T Sequence<T>::reduce(T (*func)(const T &accumulator, const T &current), const T &initialElement) const {
  if (func == nullptr) throw std::invalid_argument("Нельзя выполнить reduce с нулевой функцией");

  T result = initialElement;
  EnumeratorWrapper<T> iter = get_enumerator();

  while (iter.move_next()) {
    result = func(result, iter.get_current());
  }

  return result;
}

template <class T>
void ArraySequence<T>::sys_append(const T &item) {
  if (count >= array.get_size()) {
    int newSize = array.get_size() == 0 ? 4 : array.get_size() * 2;
    array.resize(newSize);
  }

  array.set(count, item);
  count++;
}

template <class T>
Sequence<T>* ArraySequence<T>::new_empty_instance() const {
  return empty_clone();
}

template <class T>
ArraySequence<T>::ArraySequence() : array(), count(0) {}

template <class T>
ArraySequence<T>::ArraySequence(const T* items, int count) : array(items, count), count(count) {}

template <class T>
ArraySequence<T>::ArraySequence(const DynamicArray<T> &other) : array(other), count(other.get_size()) {}

template <class T>
ArraySequence<T>::ArraySequence(const ArraySequence<T> &other) : array(other.array), count(other.count) {}

template <class T>
const T& ArraySequence<T>::get_first() const {
  if (count == 0) throw std::out_of_range("Индекс вне допустимого диапазона");

  return array.get(0);
}

template <class T>
const T& ArraySequence<T>::get_last() const {
  if (count == 0) throw std::out_of_range("Индекс вне допустимого диапазона");

  return array.get(count - 1);
}

template <class T>
const T& ArraySequence<T>::get(int index) const {
  if (index >= count) throw std::out_of_range("Индекс вне допустимого диапазона");
  if (index < 0) throw std::out_of_range("Индекс вне допустимого диапазона");

  return array.get(index);
}

template <class T>
const T& ArraySequence<T>::operator[](int index) const {
  return get(index);
}

template <class T>
Option<T> ArraySequence<T>::try_get_first() const {
  if (count == 0) return Option<T>::none();

  return Option<T>::some(array.get(0));
}

template <class T>
Option<T> ArraySequence<T>::try_get_last() const {
  if (count == 0) return Option<T>::none();

  return Option<T>::some(array.get(count - 1));
}

template <class T>
Option<T> ArraySequence<T>::try_get(int index) const {
  if (index < 0) return Option<T>::none();
  if (index >= count) return Option<T>::none();

  return Option<T>::some(array.get(index));
}

template <class T>
Option<T> ArraySequence<T>::try_find(bool (*predicate)(const T &element)) const {
  if (predicate == nullptr) throw std::invalid_argument("Нельзя выполнить try_find с нулевым предикатом");

  EnumeratorWrapper<T> iter = get_enumerator();

  while (iter.move_next()) {
    const T& item = iter.get_current();

    if (predicate(item)) {
      return Option<T>::some(item);
    }
  }

  return Option<T>::none();
}

template <class T>
int ArraySequence<T>::get_length() const {
  return count;
}

template <class T>
Sequence<T>* ArraySequence<T>::append(const T &item) {
  ArraySequence<T>* currentInstance = instance();
  currentInstance->sys_append(item);

  return currentInstance;
}

template <class T>
Sequence<T>* ArraySequence<T>::prepend(const T &item) {
  ArraySequence<T>* currentInstance = instance();

  if (currentInstance->count >= currentInstance->array.get_size()) {
    int newSize = currentInstance->array.get_size() == 0 ? 4 : currentInstance->array.get_size() * 2;
    currentInstance->array.resize(newSize);
  }

  for (int i = currentInstance->count; i > 0; i--) {
    currentInstance->array.set(i, currentInstance->array.get(i - 1));
  }

  currentInstance->array.set(0, item);
  currentInstance->count++;

  return currentInstance;
}

template <class T>
Sequence<T>* ArraySequence<T>::insert_at(const T &item, int index) {
  ArraySequence<T>* currentInstance = instance();

  if (index > currentInstance->count) throw std::out_of_range("Индекс вне допустимого диапазона");
  if (index < 0) throw std::out_of_range("Индекс вне допустимого диапазона");

  if (currentInstance->count >= currentInstance->array.get_size()) {
    int newSize = currentInstance->array.get_size() == 0 ? 4 : currentInstance->array.get_size() * 2;
    currentInstance->array.resize(newSize);
  }

  for (int i = currentInstance->count; i > index; i--) {
    currentInstance->array.set(i, currentInstance->array.get(i - 1));
  }

  currentInstance->array.set(index, item);
  currentInstance->count++;

  return currentInstance;
}

template <class T>
ArraySequence<T>* MutableArraySequence<T>::instance() {
  return this;
}

template <class T>
ArraySequence<T>* MutableArraySequence<T>::empty_clone() const {
  return new MutableArraySequence<T>();
}

template <class T>
MutableArraySequence<T>::MutableArraySequence() : ArraySequence<T>() {}

template <class T>
MutableArraySequence<T>::MutableArraySequence(const T* items, int count) : ArraySequence<T>(items, count) {}

template <class T>
MutableArraySequence<T>::MutableArraySequence(const DynamicArray<T> &array) : ArraySequence<T>(array) {}

template <class T>
MutableArraySequence<T>::MutableArraySequence(const ArraySequence<T> &array) : ArraySequence<T>(array) {}

template <class T>
MutableArraySequence<T>::MutableArraySequence(const MutableArraySequence<T> &other) : ArraySequence<T>(other) {}

template <class T>
T& MutableArraySequence<T>::operator[](int index) {
  if (index >= this->count) throw std::out_of_range("Индекс вне допустимого диапазона");
  if (index < 0) throw std::out_of_range("Индекс вне допустимого диапазона");

  return this->array[index];
}

template <class T>
ArraySequence<T>* ImmutableArraySequence<T>::instance() {
  return clone();
}

template <class T>
ArraySequence<T>* ImmutableArraySequence<T>::empty_clone() const {
  return new ImmutableArraySequence<T>();
}

template <class T>
ImmutableArraySequence<T>::ImmutableArraySequence() : ArraySequence<T>() {}

template <class T>
ImmutableArraySequence<T>::ImmutableArraySequence(const T* items, int count) : ArraySequence<T>(items, count) {}

template <class T>
ImmutableArraySequence<T>::ImmutableArraySequence(const DynamicArray<T> &array) : ArraySequence<T>(array) {}

template <class T>
ImmutableArraySequence<T>::ImmutableArraySequence(const ArraySequence<T> &array) : ArraySequence<T>(array) {}

template <class T>
ImmutableArraySequence<T>::ImmutableArraySequence(const ImmutableArraySequence<T> &other) : ArraySequence<T>(other) {}

template <class T>
ImmutableArraySequence<T>* ImmutableArraySequence<T>::clone() const {
  return new ImmutableArraySequence<T>(*this);
}

template <class T>
void ListSequence<T>::sys_append(const T &item) {
  list.append(item);
}

template <class T>
Sequence<T>* ListSequence<T>::new_empty_instance() const {
  return empty_clone();
}

template <class T>
ListSequence<T>::ListSequence() : list() {}

template <class T>
ListSequence<T>::ListSequence(const T* items, int count) : list() {
  if (count < 0) throw std::out_of_range("Длина должна быть больше, либо равна нулю!");

  for (int i = 0; i < count; i++) {
    list.append(items[i]);
  }
}

template <class T>
ListSequence<T>::ListSequence(const LinkedList<T> &other) : list(other) {}

template <class T>
ListSequence<T>::ListSequence(const ListSequence<T> &other) : list(other.list) {}

template <class T>
const T& ListSequence<T>::get_first() const {
  return list.get_first();
}

template <class T>
const T& ListSequence<T>::get_last() const {
  return list.get_last();
}

template <class T>
const T& ListSequence<T>::get(int index) const {
  return *list.get(index);
}

template <class T>
const T& ListSequence<T>::operator[](int index) const {
  return list[index];
}

template <class T>
Option<T> ListSequence<T>::try_get_first() const {
  if (list.get_length() == 0) return Option<T>::none();

  return Option<T>::some(list.get_first());
}

template <class T>
Option<T> ListSequence<T>::try_get_last() const {
  if (list.get_length() == 0) return Option<T>::none();

  return Option<T>::some(list.get_last());
}

template <class T>
Option<T> ListSequence<T>::try_get(int index) const {
  if (index < 0) return Option<T>::none();
  if (index >= list.get_length()) return Option<T>::none();

  return Option<T>::some(*list.get(index));
}

template <class T>
Option<T> ListSequence<T>::try_find(bool (*predicate)(const T &element)) const {
  if (predicate == nullptr) throw std::invalid_argument("Нельзя выполнить try_find с нулевым предикатом");

  EnumeratorWrapper<T> iter = get_enumerator();

  while (iter.move_next()) {
    const T& item = iter.get_current();

    if (predicate(item)) {
      return Option<T>::some(item);
    }
  }

  return Option<T>::none();
}

template <class T>
int ListSequence<T>::get_length() const {
  return list.get_length();
}

template <class T>
Sequence<T>* ListSequence<T>::append(const T &item) {
  ListSequence<T>* currentInstance = instance();
  currentInstance->list.append(item);

  return currentInstance;
}

template <class T>
Sequence<T>* ListSequence<T>::prepend(const T &item) {
  ListSequence<T>* currentInstance = instance();
  currentInstance->list.prepend(item);

  return currentInstance;
}

template <class T>
Sequence<T>* ListSequence<T>::insert_at(const T &item, int index) {
  ListSequence<T>* currentInstance = instance();
  currentInstance->list.insert_at(item, index);

  return currentInstance;
}

template <class T>
ListSequence<T>* MutableListSequence<T>::instance() {
  return this;
}

template <class T>
ListSequence<T>* MutableListSequence<T>::empty_clone() const {
  return new MutableListSequence<T>();
}

template <class T>
MutableListSequence<T>::MutableListSequence() : ListSequence<T>() {}

template <class T>
MutableListSequence<T>::MutableListSequence(const T* items, int count) : ListSequence<T>(items, count) {}

template <class T>
MutableListSequence<T>::MutableListSequence(const LinkedList<T> &other) : ListSequence<T>(other) {}

template <class T>
MutableListSequence<T>::MutableListSequence(const ListSequence<T> &other) : ListSequence<T>(other) {}

template <class T>
MutableListSequence<T>::MutableListSequence(const MutableListSequence<T> &other) : ListSequence<T>(other) {}

template <class T>
T& MutableListSequence<T>::operator[](int index) {
  return this->list[index];
}

template <class T>
ListSequence<T>* ImmutableListSequence<T>::instance() {
  return clone();
}

template <class T>
ListSequence<T>* ImmutableListSequence<T>::empty_clone() const {
  return new ImmutableListSequence<T>();
}

template <class T>
ImmutableListSequence<T>::ImmutableListSequence() : ListSequence<T>() {}

template <class T>
ImmutableListSequence<T>::ImmutableListSequence(const T* items, int count) : ListSequence<T>(items, count) {}

template <class T>
ImmutableListSequence<T>::ImmutableListSequence(const LinkedList<T> &other) : ListSequence<T>(other) {}

template <class T>
ImmutableListSequence<T>::ImmutableListSequence(const ListSequence<T> &other) : ListSequence<T>(other) {}

template <class T>
ImmutableListSequence<T>::ImmutableListSequence(const ImmutableListSequence<T> &other) : ListSequence<T>(other) {}

template <class T>
ImmutableListSequence<T>* ImmutableListSequence<T>::clone() const {
  return new ImmutableListSequence<T>(*this);
}
