#include <stdexcept>
#include "sequence.h"

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

//template <class T, class T2>
//Sequence<T2>* Sequence<T>::map(T2 (*func)(const T &item)) const {
//  if (func == nullptr) throw std::invalid_argument("Нельзя выполнить map с нулевой функцией");
//
//
//}

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
