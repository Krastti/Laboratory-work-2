#ifndef LABORATORYWORK2_SEQUENCE_H
#define LABORATORYWORK2_SEQUENCE_H

#include "../DynamicArray/dynamic_array.h"
#include "../LinkedList/linked_list.h"
#include "../Option/option.h"

template <class T>
class Sequence {
protected:
  virtual void sys_append(const T &item) = 0;
  virtual Sequence<T>* sys_empty_clone() const = 0;

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

template <class T>
class ArraySequence : public Sequence<T> {
protected:
  DynamicArray<T> array;
  int count;

  void sys_append(const T &item) override;
  Sequence<T>* sys_empty_clone() const override;

  virtual ArraySequence<T>* instance() = 0;
  virtual ArraySequence<T>* empty_clone() const = 0;
public:
  ArraySequence();
  ArraySequence(const T* items, int count);
  ArraySequence(const DynamicArray<T> &other);
  ArraySequence(const ArraySequence<T> &other);

  const T& get_first() const override;
  const T& get_last() const override;
  const T& get(int index) const override;
  const T& operator[](int index) const override;

  Option<T> try_get_first() const override;
  Option<T> try_get_last() const override;
  Option<T> try_get(int index) const override;
  Option<T> try_find(bool (*predicate)(const T &element)) const override;

  int get_length() const override;

  Sequence<T>* append(const T &item) override;
  Sequence<T>* prepend(const T &item) override;
  Sequence<T>* insert_at(const T &item, int index) override;;

  EnumeratorWrapper<T> get_enumerator() const override {
    return array.get_enumerator(count);
  }

  ~ArraySequence() {}
};

template <class T>
class MutableArraySequence : public ArraySequence<T> {
protected:
  ArraySequence<T>* instance() override;
  ArraySequence<T>* empty_clone() const override;

public:
  using ArraySequence<T>::operator[];

  MutableArraySequence();
  MutableArraySequence(const T* items, int count);
  MutableArraySequence(const DynamicArray<T> &array);
  MutableArraySequence(const ArraySequence<T> &array);
  MutableArraySequence(const MutableArraySequence<T> &other);

  T& operator[](int index);

  ~MutableArraySequence() {}
};

template <class T>
class ImmutableArraySequence : public ArraySequence<T> {
protected:
  ArraySequence<T>* instance() override;
  ArraySequence<T>* empty_clone() const override;

public:
  ImmutableArraySequence();
  ImmutableArraySequence(const T* items, int count);
  ImmutableArraySequence(const DynamicArray<T> &array);
  ImmutableArraySequence(const ArraySequence<T> &array);
  ImmutableArraySequence(const ImmutableArraySequence<T> &other);

  ImmutableArraySequence<T>* clone() const;

  ~ImmutableArraySequence() {}
};

template <class T>
class ListSequence : public Sequence<T> {
protected:
  LinkedList<T> list;

  void sys_append(const T &item) override;
  Sequence<T>* sys_empty_clone() const override;

  virtual ListSequence<T>* instance() = 0;
  virtual ListSequence<T>* empty_clone() const = 0;
public:
  ListSequence();
  ListSequence(const T* items, int count);
  ListSequence(const LinkedList<T> &other);
  ListSequence(const ListSequence<T> &other);

  const T& get_first() const override;
  const T& get_last() const override;
  const T& get(int index) const override;
  const T& operator[](int index) const override;

  Option<T> try_get_first() const override;
  Option<T> try_get_last() const override;
  Option<T> try_get(int index) const override;
  Option<T> try_find(bool (*predicate)(const T &element)) const override;

  int get_length() const override;

  Sequence<T>* append(const T &item) override;
  Sequence<T>* prepend(const T &item) override;
  Sequence<T>* insert_at(const T &item, int index) override;

  EnumeratorWrapper<T> get_enumerator() const override {
    return list.get_enumerator();
  }

  ~ListSequence() {}
};

template <class T>
class MutableListSequence : public ListSequence<T> {
protected:
  ListSequence<T>* instance() override;
  ListSequence<T>* empty_clone() const override;

public:
  using ListSequence<T>::operator[];

  MutableListSequence();
  MutableListSequence(const T* items, int count);
  MutableListSequence(const LinkedList<T> &other);
  MutableListSequence(const ListSequence<T> &other);
  MutableListSequence(const MutableListSequence<T> &other);

  T& operator[](int index);

  ~MutableListSequence() {}
};

template <class T>
class ImmutableListSequence : public ListSequence<T> {
protected:
  ListSequence<T>* instance() override;
  ListSequence<T>* empty_clone() const override;

public:
  ImmutableListSequence();
  ImmutableListSequence(const T* items, int count);
  ImmutableListSequence(const LinkedList<T> &other);
  ImmutableListSequence(const ListSequence<T> &other);
  ImmutableListSequence(const ImmutableListSequence<T> &other);

  ImmutableListSequence<T>* clone() const;

  ~ImmutableListSequence() {}
};

#include "sequence.tpp"

#endif // LABORATORYWORK2_SEQUENCE_H
