#include <stdexcept>
#include "bit_sequence.h"

inline int BitSequence::bytes_needed(int bitsCount) {
  if (bitsCount <= 0) return 0;

  return (bitsCount + 7) / 8;
}

inline bool BitSequence::get_bit(int index) const {
  unsigned char byte = data.get(index / 8);
  return ((byte >> (index % 8)) & 1) != 0;
}

inline const bool& BitSequence::get_bit_reference(int index) const {
  if (get_bit(index)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

inline void BitSequence::set_bit(int index, bool value) {
  int byteIndex = index / 8;
  int bitIndex = index % 8;
  unsigned char byte = data.get(byteIndex);
  unsigned char mask = static_cast<unsigned char>(1 << bitIndex);

  if (value) {
    byte = static_cast<unsigned char>(byte | mask);
  } else {
    byte = static_cast<unsigned char>(byte & ~mask);
  }

  data.set(byteIndex, byte);
}

inline void BitSequence::clear_unused_bits() {
  if (bitCount == 0) return;

  int usedBits = bitCount % 8;
  if (usedBits == 0) return;

  int lastByteIndex = bytes_needed(bitCount) - 1;
  unsigned char mask = static_cast<unsigned char>((1 << usedBits) - 1);
  data.set(lastByteIndex, static_cast<unsigned char>(data.get(lastByteIndex) & mask));
}

inline void BitSequence::sys_append(const bool &item) {
  int newCount = bitCount + 1;
  int newBytes = bytes_needed(newCount);

  if (newBytes > data.get_size()) {
    data.resize(newBytes);
    data.set(newBytes - 1, 0);
  }

  bitCount = newCount;
  set_bit(bitCount - 1, item);
}

inline Sequence<bool>* BitSequence::new_empty_instance() const {
  return new BitSequence();
}

inline BitSequence::BitReference::BitReference(BitSequence* sequence, int index)
  : sequence(sequence), index(index) {}

inline BitSequence::BitReference& BitSequence::BitReference::operator=(bool value) {
  sequence->set_bit(index, value);
  return *this;
}

inline bool BitSequence::BitReference::get() const {
  return sequence->get_bit(index);
}

inline BitSequence::BitReference::operator bool() const {
  return sequence->get_bit(index);
}

inline BitSequence::BitSequence() : data(), bitCount(0) {}

inline BitSequence::BitSequence(const bool* items, int count) : data(bytes_needed(count)), bitCount(count) {
  if (count < 0) throw std::out_of_range("Размер должен быть больше, либо равен нулю!");
  if (count > 0 && items == nullptr) throw std::invalid_argument("Нельзя создать BitSequence из нулевого указателя");

  int bytes = bytes_needed(count);

  for (int i = 0; i < bytes; i++) {
    data.set(i, 0);
  }

  for (int i = 0; i < count; i++) {
    set_bit(i, items[i]);
  }
}

inline BitSequence::BitSequence(const BitSequence &other) : data(other.data), bitCount(other.bitCount) {}

inline BitSequence& BitSequence::operator=(const BitSequence &other) {
  if (this == &other) return *this;

  data = other.data;
  bitCount = other.bitCount;

  return *this;
}

inline const bool& BitSequence::get_first() const {
  if (bitCount == 0) throw std::out_of_range("Индекс вне допустимого диапазона");

  return get_bit_reference(0);
}

inline const bool& BitSequence::get_last() const {
  if (bitCount == 0) throw std::out_of_range("Индекс вне допустимого диапазона");

  return get_bit_reference(bitCount - 1);
}

inline const bool& BitSequence::get(int index) const {
  if (index >= bitCount) throw std::out_of_range("Индекс вне допустимого диапазона");
  if (index < 0) throw std::out_of_range("Индекс вне допустимого диапазона");

  return get_bit_reference(index);
}

inline const bool& BitSequence::operator[](int index) const {
  return get(index);
}

inline BitSequence::BitReference BitSequence::operator[](int index) {
  if (index >= bitCount) throw std::out_of_range("Индекс вне допустимого диапазона");
  if (index < 0) throw std::out_of_range("Индекс вне допустимого диапазона");

  return BitReference(this, index);
}

inline Option<bool> BitSequence::try_get_first() const {
  if (bitCount == 0) return Option<bool>::none();

  return Option<bool>::some(get_bit(0));
}

inline Option<bool> BitSequence::try_get_last() const {
  if (bitCount == 0) return Option<bool>::none();

  return Option<bool>::some(get_bit(bitCount - 1));
}

inline Option<bool> BitSequence::try_get(int index) const {
  if (index < 0) return Option<bool>::none();
  if (index >= bitCount) return Option<bool>::none();

  return Option<bool>::some(get_bit(index));
}

inline Option<bool> BitSequence::try_find(bool (*predicate)(const bool &element)) const {
  if (predicate == nullptr) return Option<bool>::none();

  for (int i = 0; i < bitCount; i++) {
    bool value = get_bit(i);
    if (predicate(value)) {
      return Option<bool>::some(value);
    }
  }

  return Option<bool>::none();
}

inline int BitSequence::get_length() const {
  return bitCount;
}

inline Sequence<bool>* BitSequence::append(const bool &item) {
  sys_append(item);
  return this;
}

inline Sequence<bool>* BitSequence::prepend(const bool &item) {
  return insert_at(item, 0);
}

inline Sequence<bool>* BitSequence::insert_at(const bool &item, int index) {
  if (index > bitCount) throw std::out_of_range("Индекс вне допустимого диапазона");
  if (index < 0) throw std::out_of_range("Индекс вне допустимого диапазона");

  int oldCount = bitCount;
  int oldBytes = bytes_needed(oldCount);
  DynamicArray<unsigned char> oldData(oldBytes);

  for (int i = 0; i < oldBytes; i++) {
    oldData.set(i, data.get(i));
  }

  int newCount = oldCount + 1;
  int newBytes = bytes_needed(newCount);

  data.resize(newBytes);

  for (int i = 0; i < newBytes; i++) {
    data.set(i, 0);
  }

  bitCount = newCount;

  for (int i = 0; i < index; i++) {
    unsigned char byte = oldData.get(i / 8);
    bool value = ((byte >> (i % 8)) & 1) != 0;
    set_bit(i, value);
  }

  set_bit(index, item);

  for (int i = index; i < oldCount; i++) {
    unsigned char byte = oldData.get(i / 8);
    bool value = ((byte >> (i % 8)) & 1) != 0;
    set_bit(i + 1, value);
  }

  clear_unused_bits();
  return this;
}

inline BitSequence* BitSequence::bit_and(const BitSequence* other) const {
  if (other == nullptr) throw std::invalid_argument("Нельзя выполнить AND с нулевым указателем");
  if (bitCount != other->bitCount) throw std::invalid_argument("Длины BitSequence должны совпадать");

  BitSequence* result = new BitSequence();
  int bytes = bytes_needed(bitCount);

  result->data.resize(bytes);
  result->bitCount = bitCount;

  for (int i = 0; i < bytes; i++) {
    result->data.set(i, static_cast<unsigned char>(data.get(i) & other->data.get(i)));
  }

  result->clear_unused_bits();
  return result;
}

inline BitSequence* BitSequence::bit_or(const BitSequence* other) const {
  if (other == nullptr) throw std::invalid_argument("Нельзя выполнить OR с нулевым указателем");
  if (bitCount != other->bitCount) throw std::invalid_argument("Длины BitSequence должны совпадать");

  BitSequence* result = new BitSequence();
  int bytes = bytes_needed(bitCount);

  result->data.resize(bytes);
  result->bitCount = bitCount;

  for (int i = 0; i < bytes; i++) {
    result->data.set(i, static_cast<unsigned char>(data.get(i) | other->data.get(i)));
  }

  result->clear_unused_bits();
  return result;
}

inline BitSequence* BitSequence::bit_xor(const BitSequence* other) const {
  if (other == nullptr) throw std::invalid_argument("Нельзя выполнить XOR с нулевым указателем");
  if (bitCount != other->bitCount) throw std::invalid_argument("Длины BitSequence должны совпадать");

  BitSequence* result = new BitSequence();
  int bytes = bytes_needed(bitCount);

  result->data.resize(bytes);
  result->bitCount = bitCount;

  for (int i = 0; i < bytes; i++) {
    result->data.set(i, static_cast<unsigned char>(data.get(i) ^ other->data.get(i)));
  }

  result->clear_unused_bits();
  return result;
}

inline BitSequence* BitSequence::bit_not() const {
  BitSequence* result = new BitSequence();
  int bytes = bytes_needed(bitCount);

  result->data.resize(bytes);
  result->bitCount = bitCount;

  for (int i = 0; i < bytes; i++) {
    result->data.set(i, static_cast<unsigned char>(~data.get(i)));
  }

  result->clear_unused_bits();
  return result;
}
