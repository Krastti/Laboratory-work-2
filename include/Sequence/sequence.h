#ifndef LABORATORYWORK2_SEQUENCE_H
#define LABORATORYWORK2_SEQUENCE_H

#include "../Option/option.h"

template <class T>
class Sequence {
protected:
  virtual void sys_append(const T &item) = 0;
  virtual Sequence<T>* new_empty_instance() const = 0;

public:
  virtual const T& get_first() const = 0;
  virtual const T& get_last() const = 0;
  virtual const T& get(int index) const = 0;
  virtual const T& operator[](int index) const = 0;

  virtual Option<T> try_get_first() const = 0;
  virtual Option<T> try_get_last() const = 0;
  virtual Option<T> try_get(int index) const = 0;
  virtual Option<T> try_find(bool (*predicate)(const T &element)) const = 0;

  virtual int get_length() const = 0;

  virtual Sequence<T>* get_sub_sequence(int startIndex, int endIndex) const;

  virtual Sequence<T>* append(const T &item) = 0;
  virtual Sequence<T>* prepend(const T &item) = 0;
  virtual Sequence<T>* insert_at(const T &item, int index) = 0;

  virtual Sequence<T>* concat(const Sequence<T> &other) const;
  virtual Sequence<T>* map(T (*func)(const T &item)) const;
  virtual Sequence<T>* map(T (*func)(const T &item, int index)) const;
  virtual Sequence<T>* where(bool (*predicate)(const T &element)) const;
  virtual T reduce(T (*func)(const T &accumulator, const T &current), const T &initialElement) const;

  virtual EnumeratorWrapper<T> get_enumerator() const = 0;

  virtual ~Sequence() {}
};

#include "sequence.tpp"

#endif // LABORATORYWORK2_SEQUENCE_H
