#include <cassert>
#include <iostream>
#include <stdexcept>

#include "../include/Bit/bit_sequence.h"
#include "../include/DynamicArray/dynamic_array.h"
#include "../include/Iterator/ienumerator.h"
#include "../include/LinkedList/linked_list.h"
#include "../include/Sequence/sequence.h"

namespace {

void print_test_menu() {
  std::cout << "\n==== Laboratory Work 2 Tests ====\n";
  std::cout << "1. DynamicArray tests\n";
  std::cout << "2. LinkedList tests\n";
  std::cout << "3. Sequence tests\n";
  std::cout << "4. BitSequence tests\n";
  std::cout << "5. Run all tests\n";
  std::cout << "0. Exit\n";
  std::cout << "Choice: ";
}

int read_choice() {
  int choice = 0;

  if (!(std::cin >> choice)) {
    std::cin.clear();
    std::cin.ignore(10000, '\n');
    return -1;
  }

  return choice;
}

namespace dynamic_array_tests {

void assert_out_of_range_on_get(const DynamicArray<int>& array, int index) {
  bool thrown = false;

  try {
    array.get(index);
    // assert( false && std::string("should have thrown").operator bool());
  } catch (const std::out_of_range&) {
    thrown = true;
  }

  assert(thrown);
}

void assert_out_of_range_on_set(DynamicArray<int>& array, int index) {
  bool thrown = false;

  try {
    array.set(index, 100);
  } catch (const std::out_of_range&) {
    thrown = true;
  }

  assert(thrown);
}

void test_default_constructor() {
  DynamicArray<int> array;

  assert(array.get_size() == 0);
  assert_out_of_range_on_get(array, 0);
  assert_out_of_range_on_get(array, -1);
}

void test_constructor_from_items() {
  int items[] = {10, 20, 30};
  DynamicArray<int> array(items, 3);

  assert(array.get_size() == 3);
  assert(array.get(0) == 10);
  assert(array.get(1) == 20);
  assert(array.get(2) == 30);
}

void test_sized_constructor_and_set() {
  DynamicArray<int> array(3);

  assert(array.get_size() == 3);

  array.set(0, 5);
  array.set(1, 15);
  array.set(2, 25);

  assert(array.get(0) == 5);
  assert(array.get(1) == 15);
  assert(array.get(2) == 25);
}

void test_copy_constructor_makes_deep_copy() {
  int items[] = {1, 2, 3};
  DynamicArray<int> original(items, 3);
  DynamicArray<int> copy(original);

  copy.set(0, 99);
  copy.set(2, 77);

  assert(original.get_size() == 3);
  assert(copy.get_size() == 3);
  assert(original.get(0) == 1);
  assert(original.get(2) == 3);
  assert(copy.get(0) == 99);
  assert(copy.get(2) == 77);
}

void test_assignment_makes_deep_copy() {
  int firstItems[] = {4, 5, 6};
  int secondItems[] = {10, 20};
  DynamicArray<int> first(firstItems, 3);
  DynamicArray<int> second(secondItems, 2);

  second = first;
  first.set(1, 500);

  assert(second.get_size() == 3);
  assert(second.get(0) == 4);
  assert(second.get(1) == 5);
  assert(second.get(2) == 6);
  assert(first.get(1) == 500);

  second = second;
  assert(second.get_size() == 3);
  assert(second.get(1) == 5);
}

void test_resize_grow_and_shrink() {
  int items[] = {7, 8, 9, 10};
  DynamicArray<int> array(items, 4);

  array.resize(6);
  assert(array.get_size() == 6);
  assert(array.get(0) == 7);
  assert(array.get(1) == 8);
  assert(array.get(2) == 9);
  assert(array.get(3) == 10);

  array.set(4, 11);
  array.set(5, 12);
  assert(array.get(4) == 11);
  assert(array.get(5) == 12);

  array.resize(2);
  assert(array.get_size() == 2);
  assert(array.get(0) == 7);
  assert(array.get(1) == 8);
  assert_out_of_range_on_get(array, 2);

  array.resize(0);
  assert(array.get_size() == 0);
  assert_out_of_range_on_get(array, 0);
}

void test_invalid_indexes_and_sizes() {
  DynamicArray<int> array(2);

  assert_out_of_range_on_get(array, -1);
  assert_out_of_range_on_get(array, 2);
  assert_out_of_range_on_set(array, -1);
  assert_out_of_range_on_set(array, 2);

  bool constructorThrown = false;
  try {
    DynamicArray<int> invalid(-1);
  } catch (const std::out_of_range&) {
    constructorThrown = true;
  }
  assert(constructorThrown);

  bool resizeThrown = false;
  try {
    array.resize(-1);
  } catch (const std::out_of_range&) {
    resizeThrown = true;
  }
  assert(resizeThrown);
}

void test_enumerator() {
  int items[] = {3, 6, 9};
  DynamicArray<int> array(items, 3);
  EnumeratorWrapper<int> iter = array.get_enumerator();

  int expected[] = {3, 6, 9};
  int index = 0;

  while (iter.move_next()) {
    assert(index < 3);
    assert(iter.get_current() == expected[index]);
    index++;
  }

  assert(index == 3);

  iter.reset();
  assert(iter.move_next());
  assert(iter.get_current() == 3);
}

void run() {
  test_default_constructor();
  test_constructor_from_items();
  test_sized_constructor_and_set();
  test_copy_constructor_makes_deep_copy();
  test_assignment_makes_deep_copy();
  test_resize_grow_and_shrink();
  test_invalid_indexes_and_sizes();
  test_enumerator();

  std::cout << "DynamicArray tests passed\n";
}

}

namespace linked_list_tests {

void assert_list_equals(const LinkedList<int>& list, const int* expected, int count) {
  assert(list.get_length() == count);

  EnumeratorWrapper<int> iter = list.get_enumerator();
  int index = 0;

  while (iter.move_next()) {
    assert(index < count);
    assert(iter.get_current() == expected[index]);
    index++;
  }

  assert(index == count);

  for (int i = 0; i < count; i++) {
    assert(*list.get(i) == expected[i]);
  }
}

void assert_out_of_range_on_get(const LinkedList<int>& list, int index) {
  bool thrown = false;

  try {
    list.get(index);
  } catch (const std::out_of_range&) {
    thrown = true;
  }

  assert(thrown);
}

void assert_out_of_range_on_insert(LinkedList<int>& list, int index) {
  bool thrown = false;

  try {
    list.insert_at(100, index);
  } catch (const std::out_of_range&) {
    thrown = true;
  }

  assert(thrown);
}

void test_default_constructor() {
  LinkedList<int> list;

  assert(list.get_length() == 0);
  assert_out_of_range_on_get(list, 0);
  assert_out_of_range_on_get(list, -1);

  bool firstThrown = false;
  try {
    list.get_first();
  } catch (const std::out_of_range&) {
    firstThrown = true;
  }
  assert(firstThrown);

  bool lastThrown = false;
  try {
    list.get_last();
  } catch (const std::out_of_range&) {
    lastThrown = true;
  }
  assert(lastThrown);
}

void test_constructor_from_items() {
  int items[] = {1, 2, 3, 4};
  LinkedList<int> list(items, 4);

  assert_list_equals(list, items, 4);
  assert(list.get_first() == 1);
  assert(list.get_last() == 4);
}

void test_append_prepend_insert_at() {
  LinkedList<int> list;

  list.append(20);
  list.append(40);
  list.prepend(10);
  list.insert_at(30, 2);
  list.insert_at(5, 0);
  list.insert_at(50, 5);

  int expected[] = {5, 10, 20, 30, 40, 50};
  assert_list_equals(list, expected, 6);
  assert(list.get_first() == 5);
  assert(list.get_last() == 50);
}

void test_copy_constructor_makes_deep_copy() {
  int items[] = {3, 6, 9};
  LinkedList<int> original(items, 3);
  LinkedList<int> copy(original);

  copy.prepend(0);
  copy.append(12);

  int originalExpected[] = {3, 6, 9};
  int copyExpected[] = {0, 3, 6, 9, 12};

  assert_list_equals(original, originalExpected, 3);
  assert_list_equals(copy, copyExpected, 5);
}

void test_assignment_makes_deep_copy() {
  int firstItems[] = {1, 2, 3};
  int secondItems[] = {10, 20};
  LinkedList<int> first(firstItems, 3);
  LinkedList<int> second(secondItems, 2);

  second = first;
  first.append(4);

  int firstExpected[] = {1, 2, 3, 4};
  int secondExpected[] = {1, 2, 3};
  assert_list_equals(first, firstExpected, 4);
  assert_list_equals(second, secondExpected, 3);

  second = second;
  assert_list_equals(second, secondExpected, 3);
}

void test_get_sub_list() {
  int items[] = {5, 10, 15, 20, 25};
  LinkedList<int> list(items, 5);

  LinkedList<int>* middle = list.get_sub_list(1, 3);
  int middleExpected[] = {10, 15, 20};
  assert_list_equals(*middle, middleExpected, 3);
  delete middle;

  LinkedList<int>* single = list.get_sub_list(2, 2);
  int singleExpected[] = {15};
  assert_list_equals(*single, singleExpected, 1);
  delete single;
}

void test_concat() {
  int firstItems[] = {1, 2};
  int secondItems[] = {3, 4, 5};
  LinkedList<int> first(firstItems, 2);
  LinkedList<int> second(secondItems, 3);

  LinkedList<int>* result = first.concat(&second);
  int expected[] = {1, 2, 3, 4, 5};
  assert_list_equals(*result, expected, 5);
  delete result;

  int firstExpected[] = {1, 2};
  int secondExpected[] = {3, 4, 5};
  assert_list_equals(first, firstExpected, 2);
  assert_list_equals(second, secondExpected, 3);
}

void test_invalid_arguments() {
  int items[] = {1, 2, 3};
  LinkedList<int> list(items, 3);

  assert_out_of_range_on_get(list, -1);
  assert_out_of_range_on_get(list, 3);
  assert_out_of_range_on_insert(list, -1);
  assert_out_of_range_on_insert(list, 4);

  bool negativeCountThrown = false;
  try {
    LinkedList<int> invalid(items, -1);
  } catch (const std::out_of_range&) {
    negativeCountThrown = true;
  }
  assert(negativeCountThrown);

  bool subListNegativeThrown = false;
  try {
    LinkedList<int>* subList = list.get_sub_list(-1, 1);
    delete subList;
  } catch (const std::out_of_range&) {
    subListNegativeThrown = true;
  }
  assert(subListNegativeThrown);

  bool subListOrderThrown = false;
  try {
    LinkedList<int>* subList = list.get_sub_list(2, 1);
    delete subList;
  } catch (const std::out_of_range&) {
    subListOrderThrown = true;
  }
  assert(subListOrderThrown);

  bool concatNullThrown = false;
  try {
    LinkedList<int>* result = list.concat(nullptr);
    delete result;
  } catch (const std::invalid_argument&) {
    concatNullThrown = true;
  }
  assert(concatNullThrown);
}

void test_enumerator_reset() {
  int items[] = {2, 4, 6};
  LinkedList<int> list(items, 3);
  EnumeratorWrapper<int> iter = list.get_enumerator();

  assert(iter.move_next());
  assert(iter.get_current() == 2);
  assert(iter.move_next());
  assert(iter.get_current() == 4);

  iter.reset();
  assert(iter.move_next());
  assert(iter.get_current() == 2);
}

void run() {
  test_default_constructor();
  test_constructor_from_items();
  test_append_prepend_insert_at();
  test_copy_constructor_makes_deep_copy();
  test_assignment_makes_deep_copy();
  test_get_sub_list();
  test_concat();
  test_invalid_arguments();
  test_enumerator_reset();

  std::cout << "LinkedList tests passed\n";
}

}

namespace sequence_tests {

int square(const int& value) {
  return value * value;
}

int add_index(const int& value, int index) {
  return value + index;
}

bool is_positive(const int& value) {
  return value > 0;
}

bool is_even(const int& value) {
  return value % 2 == 0;
}

int sum(const int& accumulator, const int& current) {
  return accumulator + current;
}

void assert_sequence_equals(const Sequence<int>& sequence, const int* expected, int count) {
  assert(sequence.get_length() == count);

  EnumeratorWrapper<int> iter = sequence.get_enumerator();
  int index = 0;

  while (iter.move_next()) {
    assert(index < count);
    assert(iter.get_current() == expected[index]);
    assert(sequence.get(index) == expected[index]);
    index++;
  }

  assert(index == count);
}

void assert_empty_sequence_contract(const Sequence<int>& sequence) {
  assert(sequence.get_length() == 0);
  assert(sequence.try_get_first().is_none());
  assert(sequence.try_get_last().is_none());
  assert(sequence.try_get(0).is_none());
  assert(sequence.try_get(-1).is_none());

  bool firstThrown = false;
  try {
    sequence.get_first();
  } catch (const std::out_of_range&) {
    firstThrown = true;
  }
  assert(firstThrown);

  bool lastThrown = false;
  try {
    sequence.get_last();
  } catch (const std::out_of_range&) {
    lastThrown = true;
  }
  assert(lastThrown);
}

void test_empty_sequences() {
  MutableArraySequence<int> mutableArray;
  ImmutableArraySequence<int> immutableArray;
  MutableListSequence<int> mutableList;
  ImmutableListSequence<int> immutableList;

  assert_empty_sequence_contract(mutableArray);
  assert_empty_sequence_contract(immutableArray);
  assert_empty_sequence_contract(mutableList);
  assert_empty_sequence_contract(immutableList);
}

void test_mutable_array_sequence_changes_current_object() {
  int items[] = {2, 3};
  MutableArraySequence<int> sequence(items, 2);

  Sequence<int>* appendResult = sequence.append(4);
  Sequence<int>* prependResult = sequence.prepend(1);
  Sequence<int>* insertResult = sequence.insert_at(99, 2);

  assert(appendResult == &sequence);
  assert(prependResult == &sequence);
  assert(insertResult == &sequence);

  int expected[] = {1, 2, 99, 3, 4};
  assert_sequence_equals(sequence, expected, 5);
  assert(sequence.get_first() == 1);
  assert(sequence.get_last() == 4);
}

void test_mutable_list_sequence_changes_current_object() {
  int items[] = {2, 3};
  MutableListSequence<int> sequence(items, 2);

  Sequence<int>* appendResult = sequence.append(4);
  Sequence<int>* prependResult = sequence.prepend(1);
  Sequence<int>* insertResult = sequence.insert_at(99, 2);

  assert(appendResult == &sequence);
  assert(prependResult == &sequence);
  assert(insertResult == &sequence);

  int expected[] = {1, 2, 99, 3, 4};
  assert_sequence_equals(sequence, expected, 5);
  assert(sequence.get_first() == 1);
  assert(sequence.get_last() == 4);
}

void test_immutable_array_sequence_keeps_original() {
  int items[] = {1, 2, 3};
  ImmutableArraySequence<int> original(items, 3);

  Sequence<int>* appended = original.append(4);
  Sequence<int>* prepended = original.prepend(0);
  Sequence<int>* inserted = original.insert_at(99, 1);

  assert(appended != &original);
  assert(prepended != &original);
  assert(inserted != &original);

  int originalExpected[] = {1, 2, 3};
  int appendExpected[] = {1, 2, 3, 4};
  int prependExpected[] = {0, 1, 2, 3};
  int insertExpected[] = {1, 99, 2, 3};

  assert_sequence_equals(original, originalExpected, 3);
  assert_sequence_equals(*appended, appendExpected, 4);
  assert_sequence_equals(*prepended, prependExpected, 4);
  assert_sequence_equals(*inserted, insertExpected, 4);

  delete appended;
  delete prepended;
  delete inserted;
}

void test_immutable_list_sequence_keeps_original() {
  int items[] = {1, 2, 3};
  ImmutableListSequence<int> original(items, 3);

  Sequence<int>* appended = original.append(4);
  Sequence<int>* prepended = original.prepend(0);
  Sequence<int>* inserted = original.insert_at(99, 1);

  assert(appended != &original);
  assert(prepended != &original);
  assert(inserted != &original);

  int originalExpected[] = {1, 2, 3};
  int appendExpected[] = {1, 2, 3, 4};
  int prependExpected[] = {0, 1, 2, 3};
  int insertExpected[] = {1, 99, 2, 3};

  assert_sequence_equals(original, originalExpected, 3);
  assert_sequence_equals(*appended, appendExpected, 4);
  assert_sequence_equals(*prepended, prependExpected, 4);
  assert_sequence_equals(*inserted, insertExpected, 4);

  delete appended;
  delete prepended;
  delete inserted;
}

void test_common_sequence_algorithms() {
  int firstItems[] = {1, -2, 3, 4};
  int secondItems[] = {5, 6};
  MutableArraySequence<int> first(firstItems, 4);
  MutableListSequence<int> second(secondItems, 2);

  Sequence<int>* subSequence = first.get_sub_sequence(1, 2);
  int subExpected[] = {-2, 3};
  assert_sequence_equals(*subSequence, subExpected, 2);
  delete subSequence;

  Sequence<int>* concatSequence = first.concat(second);
  int concatExpected[] = {1, -2, 3, 4, 5, 6};
  assert_sequence_equals(*concatSequence, concatExpected, 6);
  delete concatSequence;

  Sequence<int>* mapped = first.map(square);
  int mappedExpected[] = {1, 4, 9, 16};
  assert_sequence_equals(*mapped, mappedExpected, 4);
  delete mapped;

  Sequence<int>* mappedWithIndex = first.map(add_index);
  int mappedWithIndexExpected[] = {1, -1, 5, 7};
  assert_sequence_equals(*mappedWithIndex, mappedWithIndexExpected, 4);
  delete mappedWithIndex;

  Sequence<int>* filtered = first.where(is_positive);
  int filteredExpected[] = {1, 3, 4};
  assert_sequence_equals(*filtered, filteredExpected, 3);
  delete filtered;

  assert(first.reduce(sum, 0) == 6);
}

void test_try_methods_and_exceptions() {
  int items[] = {1, 2, 3};
  MutableArraySequence<int> sequence(items, 3);

  assert(sequence.try_get_first().is_some());
  assert(sequence.try_get_first().get_value() == 1);
  assert(sequence.try_get_last().is_some());
  assert(sequence.try_get_last().get_value() == 3);
  assert(sequence.try_get(1).is_some());
  assert(sequence.try_get(1).get_value() == 2);
  assert(sequence.try_get(-1).is_none());
  assert(sequence.try_get(3).is_none());
  assert(sequence.try_find(is_even).is_some());
  assert(sequence.try_find(is_even).get_value() == 2);

  bool insertThrown = false;
  try {
    sequence.insert_at(10, -1);
  } catch (const std::out_of_range&) {
    insertThrown = true;
  }
  assert(insertThrown);

  bool mapThrown = false;
  try {
    Sequence<int>* result = sequence.map(static_cast<int (*)(const int&)>(nullptr));
    delete result;
  } catch (const std::invalid_argument&) {
    mapThrown = true;
  }
  assert(mapThrown);

  bool mapWithIndexThrown = false;
  try {
    Sequence<int>* result = sequence.map(static_cast<int (*)(const int&, int)>(nullptr));
    delete result;
  } catch (const std::invalid_argument&) {
    mapWithIndexThrown = true;
  }
  assert(mapWithIndexThrown);
}

void run() {
  test_empty_sequences();
  test_mutable_array_sequence_changes_current_object();
  test_mutable_list_sequence_changes_current_object();
  test_immutable_array_sequence_keeps_original();
  test_immutable_list_sequence_keeps_original();
  test_common_sequence_algorithms();
  test_try_methods_and_exceptions();

  std::cout << "Sequence tests passed\n";
}

}

namespace bit_sequence_tests {

bool is_one(const bool& bit) {
  return bit;
}

bool bit_or(const bool& accumulator, const bool& current) {
  return accumulator || current;
}

void assert_bit_sequence_equals(const Sequence<bool>& sequence, const int* expected, int count) {
  assert(sequence.get_length() == count);

  EnumeratorWrapper<bool> iter = sequence.get_enumerator();
  int index = 0;

  while (iter.move_next()) {
    assert(index < count);
    assert(iter.get_current() == (expected[index] != 0));
    assert(sequence.get(index) == (expected[index] != 0));
    index++;
  }

  assert(index == count);
}

void test_empty_bit_sequence_contract() {
  BitSequence sequence;

  assert(sequence.get_length() == 0);
  assert(sequence.try_get_first().is_none());
  assert(sequence.try_get_last().is_none());
  assert(sequence.try_get(0).is_none());

  bool firstThrown = false;
  try {
    sequence.get_first();
  } catch (const std::out_of_range&) {
    firstThrown = true;
  }
  assert(firstThrown);
}

void test_sequence_interface_operations() {
  bool items[] = {true, false, true};
  BitSequence sequence(items, 3);

  assert(sequence.get_first());
  assert(sequence.get_last());
  assert(!sequence.get(1));

  Sequence<bool>* appendResult = sequence.append(false);
  Sequence<bool>* prependResult = sequence.prepend(true);
  Sequence<bool>* insertResult = sequence.insert_at(false, 2);

  assert(appendResult == &sequence);
  assert(prependResult == &sequence);
  assert(insertResult == &sequence);

  int expected[] = {1, 1, 0, 0, 1, 0};
  assert_bit_sequence_equals(sequence, expected, 6);
}

void test_sub_sequence_map_where_reduce() {
  bool items[] = {true, false, true, true, false};
  BitSequence sequence(items, 5);

  Sequence<bool>* subSequence = sequence.get_sub_sequence(1, 3);
  int subExpected[] = {0, 1, 1};
  assert_bit_sequence_equals(*subSequence, subExpected, 3);
  delete subSequence;

  Sequence<bool>* onlyOnes = sequence.where(is_one);
  int onesExpected[] = {1, 1, 1};
  assert_bit_sequence_equals(*onlyOnes, onesExpected, 3);
  delete onlyOnes;

  assert(sequence.reduce(bit_or, false));
}

void test_bit_reference_and_copying() {
  bool items[] = {true, false, false};
  BitSequence original(items, 3);
  BitSequence copy(original);

  copy[1] = true;
  copy[2] = true;

  int originalExpected[] = {1, 0, 0};
  int copyExpected[] = {1, 1, 1};
  assert_bit_sequence_equals(original, originalExpected, 3);
  assert_bit_sequence_equals(copy, copyExpected, 3);

  original = copy;
  copy[0] = false;

  int assignedExpected[] = {1, 1, 1};
  int changedCopyExpected[] = {0, 1, 1};
  assert_bit_sequence_equals(original, assignedExpected, 3);
  assert_bit_sequence_equals(copy, changedCopyExpected, 3);
}

void test_bitwise_operations() {
  bool firstItems[] = {true, false, true, false, true};
  bool secondItems[] = {true, true, false, false, true};
  BitSequence first(firstItems, 5);
  BitSequence second(secondItems, 5);

  BitSequence* andResult = first.bit_and(&second);
  BitSequence* orResult = first.bit_or(&second);
  BitSequence* xorResult = first.bit_xor(&second);
  BitSequence* notResult = first.bit_not();

  int andExpected[] = {1, 0, 0, 0, 1};
  int orExpected[] = {1, 1, 1, 0, 1};
  int xorExpected[] = {0, 1, 1, 0, 0};
  int notExpected[] = {0, 1, 0, 1, 0};

  assert_bit_sequence_equals(*andResult, andExpected, 5);
  assert_bit_sequence_equals(*orResult, orExpected, 5);
  assert_bit_sequence_equals(*xorResult, xorExpected, 5);
  assert_bit_sequence_equals(*notResult, notExpected, 5);

  delete andResult;
  delete orResult;
  delete xorResult;
  delete notResult;
}

void test_invalid_arguments() {
  bool items[] = {true, false, true};
  BitSequence sequence(items, 3);
  BitSequence shorter(items, 2);

  bool getThrown = false;
  try {
    sequence.get(-1);
  } catch (const std::out_of_range&) {
    getThrown = true;
  }
  assert(getThrown);

  bool insertThrown = false;
  try {
    sequence.insert_at(true, 10);
  } catch (const std::out_of_range&) {
    insertThrown = true;
  }
  assert(insertThrown);

  bool nullItemsThrown = false;
  try {
    BitSequence invalid(nullptr, 1);
  } catch (const std::invalid_argument&) {
    nullItemsThrown = true;
  }
  assert(nullItemsThrown);

  bool nullAndThrown = false;
  try {
    BitSequence* result = sequence.bit_and(nullptr);
    delete result;
  } catch (const std::invalid_argument&) {
    nullAndThrown = true;
  }
  assert(nullAndThrown);

  bool lengthThrown = false;
  try {
    BitSequence* result = sequence.bit_xor(&shorter);
    delete result;
  } catch (const std::invalid_argument&) {
    lengthThrown = true;
  }
  assert(lengthThrown);
}

void test() {
  BitSequence seq;
  seq.append(false);
  seq.append(true);
  seq.append(false);

  const bool& r1 = seq.get(0);
  const bool& r2 = seq.get(1);
  const bool& r3 = seq.get(2);

  assert(r1 == false);
  assert(r2 == true);
  assert(r3 == false);
}

void run() {
  test_empty_bit_sequence_contract();
  test_sequence_interface_operations();
  test_sub_sequence_map_where_reduce();
  test_bit_reference_and_copying();
  test_bitwise_operations();
  test_invalid_arguments();
  test();

  std::cout << "BitSequence tests passed\n";
}

}

void run_all_tests() {
  dynamic_array_tests::run();
  linked_list_tests::run();
  sequence_tests::run();
  bit_sequence_tests::run();

  std::cout << "All tests passed\n";
}

}

int main() {
  int choice = -1;

  while (choice != 0) {
    print_test_menu();
    choice = read_choice();

    if (choice == 1) {
      dynamic_array_tests::run();
    } else if (choice == 2) {
      linked_list_tests::run();
    } else if (choice == 3) {
      sequence_tests::run();
    } else if (choice == 4) {
      bit_sequence_tests::run();
    } else if (choice == 5) {
      run_all_tests();
    } else if (choice == 0) {
      std::cout << "Выход\n";
    } else {
      std::cout << "Неправильное значение!\n";
    }
  }

  return 0;
}
