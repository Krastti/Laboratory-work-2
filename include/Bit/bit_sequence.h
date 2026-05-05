#ifndef LABORATORYWORK2_BIT_SEQUENCE_H
#define LABORATORYWORK2_BIT_SEQUENCE_H

#include "../Sequence/sequence.h"
#include "../DynamicArray/dynamic_array.h"

class BitSequence : public Sequence<bool> {
private:
  DynamicArray<unsigned char> data;
  int bitCount;

  inline static const bool TRUE = true;
  inline static const bool FALSE = false;

  static int bytes_needed(int bitCount);

  bool get_bit(int index) const;
  const bool& get_bit_reference(int index) const;
  void set_bit(int index, bool value);
  void clear_unused_bits();

protected:
  void sys_append(const bool &item) override;
  Sequence<bool>* new_empty_instance() const override;

public:
  /*
   * Данный класс позволяет работать с битами внутри упакованного байта, т.к
   * мы не можем напрямую обратиться по ссылке к биту
   */
  class BitReference {
  private:
    BitSequence* sequence;
    int index;

    BitReference(BitSequence* sequence, int index);
    friend class BitSequence;

  public:
    BitReference& operator=(bool value);
    bool get() const;
    operator bool() const;
  };

  BitSequence();
  BitSequence(const bool* items, int count);
  BitSequence(const BitSequence &other);

  BitSequence& operator=(const BitSequence &other);

  const bool& get_first() const override;
  const bool& get_last() const override;
  const bool& get(int index) const override;
  const bool& operator[](int index) const override;
  BitReference operator[](int index);

  Option<bool> try_get_first() const override;
  Option<bool> try_get_last() const override;
  Option<bool> try_get(int index) const override;
  Option<bool> try_find(bool (*predicate)(const bool &element)) const override;

  int get_length() const override;

  Sequence<bool>* append(const bool &item) override;
  Sequence<bool>* prepend(const bool &item) override;
  Sequence<bool>* insert_at(const bool &item, int index) override;

  BitSequence* bit_and(const BitSequence* other) const;
  BitSequence* bit_or(const BitSequence* other) const;
  BitSequence* bit_xor(const BitSequence* other) const;
  BitSequence* bit_not() const;

  class Enumerator : public IEnumerator<bool> {
  private:
    const BitSequence* sequence;
    int index;

  public:
    Enumerator(const BitSequence* sequence) : sequence(sequence), index(-1) {}

    bool move_next() override {
      index++;
      return index < sequence->bitCount;
    }

    const bool& get_current() const override {
      return sequence->get(index);
    }

    void reset() override {
      index = -1;
    }
  };

  EnumeratorWrapper<bool> get_enumerator() const override {
    return EnumeratorWrapper<bool>(new Enumerator(this));
  }

  ~BitSequence() {}
};

#include "bit_sequence.tpp"

#endif // LABORATORYWORK2_BIT_SEQUENCE_H
