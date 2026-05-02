#ifndef LABORATORYWORK2_MAP_REDUCE_H
#define LABORATORYWORK2_MAP_REDUCE_H

#include "../Sequence/sequence.h"

template <class TFirst, class TSecond>
class Pair {
private:
  TFirst firstValue;
  TSecond secondValue;

public:
  Pair();
  Pair(const TFirst &first, const TSecond &second);

  const TFirst& first() const;
  const TSecond& second() const;

  bool operator==(const Pair<TFirst, TSecond> &other) const;
  bool operator!=(const Pair<TFirst, TSecond> &other) const;
};

template <class TFirst, class TSecond>
Sequence<Pair<TFirst, TSecond>>* zip(const Sequence<TFirst> &first, const Sequence<TSecond> &second);

template <class TFirst, class TSecond>
Pair<Sequence<TFirst>*, Sequence<TSecond>*> unzip(const Sequence<Pair<TFirst, TSecond>> &sequence);

template <class T>
Sequence<T>* skip(const Sequence<T> &sequence, int count);

template <class T>
Pair<Sequence<T>*, Sequence<T>*> split(const Sequence<T> &sequence, bool (*predicate)(const T &item));

template <class T>
Sequence<T>* slice(const Sequence<T> &sequence, int index, int count, const Sequence<T>* replacement = nullptr);

template <class TSource, class TResult>
Sequence<TResult>* flat_map(const Sequence<TSource> &sequence, Sequence<TResult>* (*func)(const TSource &item));

#include "map_reduce.tpp"

#endif // LABORATORYWORK2_MAP_REDUCE_H
