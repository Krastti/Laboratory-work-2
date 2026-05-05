#include <stdexcept>
#include "map_reduce.h"
#include "../Sequence/array_sequence.h"
#include "../Sequence/list_sequence.h"

template <class TFirst, class TSecond>
Pair<TFirst, TSecond>::Pair() : firstValue(), secondValue() {}

template <class TFirst, class TSecond>
Pair<TFirst, TSecond>::Pair(const TFirst &first, const TSecond &second)
  : firstValue(first), secondValue(second) {}

template <class TFirst, class TSecond>
const TFirst& Pair<TFirst, TSecond>::first() const {
  return firstValue;
}

template <class TFirst, class TSecond>
const TSecond& Pair<TFirst, TSecond>::second() const {
  return secondValue;
}

template <class TFirst, class TSecond>
bool Pair<TFirst, TSecond>::operator==(const Pair<TFirst, TSecond> &other) const {
  return firstValue == other.firstValue && secondValue == other.secondValue;
}

template <class TFirst, class TSecond>
bool Pair<TFirst, TSecond>::operator!=(const Pair<TFirst, TSecond> &other) const {
  return !(*this == other);
}

template <class TFirst, class TSecond>
Sequence<Pair<TFirst, TSecond>>* zip(const Sequence<TFirst> &first, const Sequence<TSecond> &second) {
  Sequence<Pair<TFirst, TSecond>>* result = new MutableArraySequence<Pair<TFirst, TSecond>>();

  EnumeratorWrapper<TFirst> firstIter = first.get_enumerator();
  EnumeratorWrapper<TSecond> secondIter = second.get_enumerator();

  while (firstIter.move_next() && secondIter.move_next()) {
    result->append(Pair<TFirst, TSecond>(firstIter.get_current(), secondIter.get_current()));
  }

  return result;
}

template <class TFirst, class TSecond>
Pair<Sequence<TFirst>*, Sequence<TSecond>*> unzip(const Sequence<Pair<TFirst, TSecond>> &sequence) {
  Sequence<TFirst>* firstResult = new MutableArraySequence<TFirst>();
  Sequence<TSecond>* secondResult = new MutableArraySequence<TSecond>();

  EnumeratorWrapper<Pair<TFirst, TSecond>> iter = sequence.get_enumerator();

  while (iter.move_next()) {
    const Pair<TFirst, TSecond> &item = iter.get_current();
    firstResult->append(item.first());
    secondResult->append(item.second());
  }

  return Pair<Sequence<TFirst>*, Sequence<TSecond>*>(firstResult, secondResult);
}

template <class T>
Sequence<T>* skip(const Sequence<T> &sequence, int count) {
  if (count < 0) throw std::out_of_range("Индекс вне допустимого диапазона");

  Sequence<T>* result = new MutableArraySequence<T>();
  EnumeratorWrapper<T> iter = sequence.get_enumerator();
  int index = 0;

  while (iter.move_next()) {
    if (index >= count) {
      result->append(iter.get_current());
    }

    index++;
  }

  return result;
}

template <class T>
Pair<Sequence<T>*, Sequence<T>*> split(const Sequence<T> &sequence, bool (*predicate)(const T &item)) {
  if (predicate == nullptr) throw std::invalid_argument("Нельзя выполнить split с нулевым предикатом");

  Sequence<T>* matched = new MutableArraySequence<T>();
  Sequence<T>* notMatched = new MutableArraySequence<T>();

  EnumeratorWrapper<T> iter = sequence.get_enumerator();

  while (iter.move_next()) {
    const T &item = iter.get_current();

    if (predicate(item)) {
      matched->append(item);
    } else {
      notMatched->append(item);
    }
  }

  return Pair<Sequence<T>*, Sequence<T>*>(matched, notMatched);
}

template <class T>
Sequence<T>* slice(const Sequence<T> &sequence, int index, int count, const Sequence<T>* replacement) {
  if (count < 0) throw std::out_of_range("Индекс вне допустимого диапазона");

  int length = sequence.get_length();

  if (index < 0) {
    index = length + index;
  }

  if (index < 0) throw std::out_of_range("Индекс вне допустимого диапазона");
  if (index > length) throw std::out_of_range("Индекс вне допустимого диапазона");

  Sequence<T>* result = new MutableArraySequence<T>();
  EnumeratorWrapper<T> iter = sequence.get_enumerator();
  int currentIndex = 0;
  bool replacementInserted = false;

  while (iter.move_next()) {
    if (currentIndex == index && !replacementInserted) {
      if (replacement != nullptr) {
        EnumeratorWrapper<T> replacementIter = replacement->get_enumerator();

        while (replacementIter.move_next()) {
          result->append(replacementIter.get_current());
        }
      }

      replacementInserted = true;
    }

    if (currentIndex < index || currentIndex >= index + count) {
      result->append(iter.get_current());
    }

    currentIndex++;
  }

  if (!replacementInserted) {
    if (replacement != nullptr) {
      EnumeratorWrapper<T> replacementIter = replacement->get_enumerator();

      while (replacementIter.move_next()) {
        result->append(replacementIter.get_current());
      }
    }
  }

  return result;
}

template <class TSource, class TResult>
Sequence<TResult>* flat_map(const Sequence<TSource> &sequence, Sequence<TResult>* (*func)(const TSource &item)) {
  if (func == nullptr) throw std::invalid_argument("Нельзя выполнить flat_map с нулевой функцией");

  Sequence<TResult>* result = new MutableArraySequence<TResult>();
  EnumeratorWrapper<TSource> iter = sequence.get_enumerator();

  while (iter.move_next()) {
    Sequence<TResult>* mapped = func(iter.get_current());

    if (mapped == nullptr) {
      delete result;
      throw std::invalid_argument("Функция flat_map вернула нулевой указатель");
    }

    EnumeratorWrapper<TResult> mappedIter = mapped->get_enumerator();

    while (mappedIter.move_next()) {
      result->append(mappedIter.get_current());
    }

    delete mapped;
  }

  return result;
}
