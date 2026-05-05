#include "../include/Menu/menu.h"

#include <iostream>
#include <limits>
#include <stdexcept>

#include "../include/Bit/bit_sequence.h"
#include "../include/MapReduce/map_reduce.h"
#include "../include/Sequence/list_sequence.h"
#include "../include/Sequence/array_sequence.h"

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
    std::cout << "Invalid input. Try again: ";
  }

  return value;
}

int read_non_negative_count() {
  int count = read_int();

  if (count < 0) {
    throw std::out_of_range("Count must be non-negative");
  }

  return count;
}

int* read_int_items(int count) {
  if (count == 0) return nullptr;

  int* items = new int[count];

  for (int i = 0; i < count; i++) {
    std::cout << "Item " << i << ": ";
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
    std::cout << "No DynamicArray objects have been created yet\n";
    return -1;
  }

  std::cout << title << '\n';

  for (int i = 0; i < dynamicArrayCount; i++) {
    std::cout << i << ": ";
    print_dynamic_array(dynamicArrays[i]);
    std::cout << '\n';
  }

  std::cout << "Index: ";
  int index = read_int();

  if (index < 0 || index >= dynamicArrayCount) {
    std::cout << "Invalid index\n";
    return -1;
  }

  return index;
}

int select_linked_list(const char* title) {
  if (linkedListCount == 0) {
    std::cout << "No LinkedList objects have been created yet\n";
    return -1;
  }

  std::cout << title << '\n';

  for (int i = 0; i < linkedListCount; i++) {
    std::cout << i << ": ";
    print_linked_list(linkedLists[i]);
    std::cout << '\n';
  }

  std::cout << "Index: ";
  int index = read_int();

  if (index < 0 || index >= linkedListCount) {
    std::cout << "Invalid index\n";
    return -1;
  }

  return index;
}

int select_sequence(const char* title) {
  if (sequenceCount == 0) {
    std::cout << "No Sequence<int> objects have been created yet\n";
    return -1;
  }

  std::cout << title << '\n';

  for (int i = 0; i < sequenceCount; i++) {
    std::cout << i << ": ";
    print_sequence(sequences[i]);
    std::cout << '\n';
  }

  std::cout << "Index: ";
  int index = read_int();

  if (index < 0 || index >= sequenceCount) {
    std::cout << "Invalid index\n";
    return -1;
  }

  return index;
}

int select_bit_sequence(const char* title) {
  if (bitSequenceCount == 0) {
    std::cout << "No BitSequence objects have been created yet\n";
    return -1;
  }

  std::cout << title << '\n';

  for (int i = 0; i < bitSequenceCount; i++) {
    std::cout << i << ": ";
    print_bit_sequence(bitSequences[i]);
    std::cout << '\n';
  }

  std::cout << "Index: ";
  int index = read_int();

  if (index < 0 || index >= bitSequenceCount) {
    std::cout << "Invalid index\n";
    return -1;
  }

  return index;
}

void create_dynamic_array() {
  if (dynamicArrayCount >= MAX_OBJECTS) {
    std::cout << "DynamicArray storage is full\n";
    return;
  }

  std::cout << "Enter DynamicArray size: ";
  int count = read_non_negative_count();
  int* items = read_int_items(count);

  dynamicArrays[dynamicArrayCount] = new DynamicArray<int>(items, count);
  delete[] items;

  std::cout << "DynamicArray created at index " << dynamicArrayCount << '\n';
  dynamicArrayCount++;
}

void dynamic_array_operations() {
  int index = select_dynamic_array("Select DynamicArray:");
  if (index == -1) return;

  std::cout << "1. Get item (get)\n";
  std::cout << "2. Set item (set)\n";
  std::cout << "3. Resize (resize)\n";
  std::cout << "4. Read operator[]\n";
  std::cout << "5. Write through operator[]\n";
  std::cout << "6. Get size (get_size)\n";
  std::cout << "Choice: ";
  int choice = read_int();

  if (choice == 1) {
    std::cout << "Item index: ";
    int itemIndex = read_int();
    std::cout << "Value: " << dynamicArrays[index]->get(itemIndex) << '\n';
  } else if (choice == 2) {
    std::cout << "Item index: ";
    int itemIndex = read_int();
    std::cout << "New value: ";
    int value = read_int();
    dynamicArrays[index]->set(itemIndex, value);
    std::cout << "Updated: ";
    print_dynamic_array(dynamicArrays[index]);
    std::cout << '\n';
  } else if (choice == 3) {
    std::cout << "New size: ";
    int size = read_non_negative_count();
    dynamicArrays[index]->resize(size);
    std::cout << "Resized: ";
    print_dynamic_array(dynamicArrays[index]);
    std::cout << '\n';
  } else if (choice == 4) {
    std::cout << "Item index: ";
    int itemIndex = read_int();
    std::cout << "Value: " << (*dynamicArrays[index])[itemIndex] << '\n';
  } else if (choice == 5) {
    std::cout << "Item index: ";
    int itemIndex = read_int();
    std::cout << "New value: ";
    int value = read_int();
    (*dynamicArrays[index])[itemIndex] = value;
    std::cout << "Updated: ";
    print_dynamic_array(dynamicArrays[index]);
    std::cout << '\n';
  } else if (choice == 6) {
    std::cout << "Size: " << dynamicArrays[index]->get_size() << '\n';
  } else {
    std::cout << "Invalid choice\n";
  }
}

void create_linked_list() {
  if (linkedListCount >= MAX_OBJECTS) {
    std::cout << "LinkedList storage is full\n";
    return;
  }

  std::cout << "Enter LinkedList size: ";
  int count = read_non_negative_count();
  int* items = read_int_items(count);

  linkedLists[linkedListCount] = new LinkedList<int>(items, count);
  delete[] items;

  std::cout << "LinkedList created at index " << linkedListCount << '\n';
  linkedListCount++;
}

void linked_list_operations() {
  int index = select_linked_list("Select LinkedList:");
  if (index == -1) return;

  std::cout << "1. Get first item (get_first)\n";
  std::cout << "2. Get last item (get_last)\n";
  std::cout << "3. Get item (get)\n";
  std::cout << "4. Get length (get_length)\n";
  std::cout << "5. Append item (append)\n";
  std::cout << "6. Prepend item (prepend)\n";
  std::cout << "7. Insert by index (insert_at)\n";
  std::cout << "8. Get sublist (get_sub_list)\n";
  std::cout << "9. Concatenate lists (concat)\n";
  std::cout << "10. Read operator[]\n";
  std::cout << "11. Write through operator[]\n";
  std::cout << "Choice: ";
  int choice = read_int();

  if (choice == 1) {
    std::cout << "Value: " << linkedLists[index]->get_first() << '\n';
  } else if (choice == 2) {
    std::cout << "Value: " << linkedLists[index]->get_last() << '\n';
  } else if (choice == 3) {
    std::cout << "Item index: ";
    int itemIndex = read_int();
    std::cout << "Value: " << *linkedLists[index]->get(itemIndex) << '\n';
  } else if (choice == 4) {
    std::cout << "Length: " << linkedLists[index]->get_length() << '\n';
  } else if (choice == 5) {
    std::cout << "Value: ";
    linkedLists[index]->append(read_int());
  } else if (choice == 6) {
    std::cout << "Value: ";
    linkedLists[index]->prepend(read_int());
  } else if (choice == 7) {
    std::cout << "Item index: ";
    int itemIndex = read_int();
    std::cout << "Value: ";
    int value = read_int();
    linkedLists[index]->insert_at(value, itemIndex);
  } else if (choice == 8) {
    if (linkedListCount >= MAX_OBJECTS) {
      std::cout << "LinkedList storage is full\n";
      return;
    }

    std::cout << "Start index: ";
    int startIndex = read_int();
    std::cout << "End index: ";
    int endIndex = read_int();
    linkedLists[linkedListCount] = linkedLists[index]->get_sub_list(startIndex, endIndex);
    std::cout << "Sublist saved at index " << linkedListCount << '\n';
    linkedListCount++;
  } else if (choice == 9) {
    if (linkedListCount >= MAX_OBJECTS) {
      std::cout << "LinkedList storage is full\n";
      return;
    }

    int secondIndex = select_linked_list("Select second LinkedList:");
    if (secondIndex == -1) return;

    linkedLists[linkedListCount] = linkedLists[index]->concat(linkedLists[secondIndex]);
    std::cout << "Concat result saved at index " << linkedListCount << '\n';
    linkedListCount++;
  } else if (choice == 10) {
    std::cout << "Item index: ";
    int itemIndex = read_int();
    std::cout << "Value: " << (*linkedLists[index])[itemIndex] << '\n';
  } else if (choice == 11) {
    std::cout << "Item index: ";
    int itemIndex = read_int();
    std::cout << "New value: ";
    int value = read_int();
    (*linkedLists[index])[itemIndex] = value;
  } else {
    std::cout << "Invalid choice\n";
    return;
  }

  std::cout << "Current value: ";
  print_linked_list(linkedLists[index]);
  std::cout << '\n';
}

void create_sequence() {
  if (sequenceCount >= MAX_OBJECTS) {
    std::cout << "Sequence storage is full\n";
    return;
  }

  std::cout << "1. MutableArraySequence<int>\n";
  std::cout << "2. ImmutableArraySequence<int>\n";
  std::cout << "3. MutableListSequence<int>\n";
  std::cout << "4. ImmutableListSequence<int>\n";
  std::cout << "Choice: ";
  int choice = read_int();

  std::cout << "Enter Sequence size: ";
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
    std::cout << "Invalid choice\n";
    return;
  }

  delete[] items;
  sequences[sequenceCount] = sequence;
  std::cout << "Sequence created at index " << sequenceCount << '\n';
  sequenceCount++;
}

void sequence_operations() {
  int index = select_sequence("Select Sequence<int>:");
  if (index == -1) return;

  std::cout << "1. Get first item (get_first)\n";
  std::cout << "2. Get last item (get_last)\n";
  std::cout << "3. Get item (get)\n";
  std::cout << "4. Try get first item (try_get_first)\n";
  std::cout << "5. Try get last item (try_get_last)\n";
  std::cout << "6. Try get item (try_get)\n";
  std::cout << "7. Find positive item (try_find)\n";
  std::cout << "8. Get length (get_length)\n";
  std::cout << "9. Append item (append)\n";
  std::cout << "10. Prepend item (prepend)\n";
  std::cout << "11. Insert by index (insert_at)\n";
  std::cout << "12. Get subsequence (get_sub_sequence)\n";
  std::cout << "13. Concatenate (concat)\n";
  std::cout << "14. Map: square\n";
  std::cout << "15. Map: value + index\n";
  std::cout << "16. Where: positive items\n";
  std::cout << "17. Reduce: sum\n";
  std::cout << "18. Read operator[]\n";
  std::cout << "19. Write through operator[] (mutable only)\n";
  std::cout << "Choice: ";
  int choice = read_int();

  if (choice == 1) {
    std::cout << "Value: " << sequences[index]->get_first() << '\n';
  } else if (choice == 2) {
    std::cout << "Value: " << sequences[index]->get_last() << '\n';
  } else if (choice == 3) {
    std::cout << "Item index: ";
    int itemIndex = read_int();
    std::cout << "Value: " << sequences[index]->get(itemIndex) << '\n';
  } else if (choice == 4) {
    std::cout << "Value: ";
    print_option(sequences[index]->try_get_first());
    std::cout << '\n';
  } else if (choice == 5) {
    std::cout << "Value: ";
    print_option(sequences[index]->try_get_last());
    std::cout << '\n';
  } else if (choice == 6) {
    std::cout << "Item index: ";
    int itemIndex = read_int();
    std::cout << "Value: ";
    print_option(sequences[index]->try_get(itemIndex));
    std::cout << '\n';
  } else if (choice == 7) {
    std::cout << "Value: ";
    print_option(sequences[index]->try_find(is_positive));
    std::cout << '\n';
  } else if (choice == 8) {
    std::cout << "Length: " << sequences[index]->get_length() << '\n';
  } else if (choice == 9) {
    std::cout << "Value: ";
    replace_sequence_if_needed(index, sequences[index]->append(read_int()));
  } else if (choice == 10) {
    std::cout << "Value: ";
    replace_sequence_if_needed(index, sequences[index]->prepend(read_int()));
  } else if (choice == 11) {
    std::cout << "Item index: ";
    int itemIndex = read_int();
    std::cout << "Value: ";
    int value = read_int();
    replace_sequence_if_needed(index, sequences[index]->insert_at(value, itemIndex));
  } else if (choice == 12) {
    if (sequenceCount >= MAX_OBJECTS) {
      std::cout << "Sequence storage is full\n";
      return;
    }

    std::cout << "Start index: ";
    int startIndex = read_int();
    std::cout << "End index: ";
    int endIndex = read_int();
    sequences[sequenceCount] = sequences[index]->get_sub_sequence(startIndex, endIndex);
    std::cout << "Subsequence saved at index " << sequenceCount << '\n';
    sequenceCount++;
  } else if (choice == 13) {
    if (sequenceCount >= MAX_OBJECTS) {
      std::cout << "Sequence storage is full\n";
      return;
    }

    int secondIndex = select_sequence("Select second Sequence<int>:");
    if (secondIndex == -1) return;

    sequences[sequenceCount] = sequences[index]->concat(*sequences[secondIndex]);
    std::cout << "Concat result saved at index " << sequenceCount << '\n';
    sequenceCount++;
  } else if (choice == 14) {
    if (sequenceCount >= MAX_OBJECTS) {
      std::cout << "Sequence storage is full\n";
      return;
    }

    sequences[sequenceCount] = sequences[index]->map(square);
    std::cout << "Map result saved at index " << sequenceCount << '\n';
    sequenceCount++;
  } else if (choice == 15) {
    if (sequenceCount >= MAX_OBJECTS) {
      std::cout << "Sequence storage is full\n";
      return;
    }

    sequences[sequenceCount] = sequences[index]->map(add_index);
    std::cout << "Map result saved at index " << sequenceCount << '\n';
    sequenceCount++;
  } else if (choice == 16) {
    if (sequenceCount >= MAX_OBJECTS) {
      std::cout << "Sequence storage is full\n";
      return;
    }

    sequences[sequenceCount] = sequences[index]->where(is_positive);
    std::cout << "Where result saved at index " << sequenceCount << '\n';
    sequenceCount++;
  } else if (choice == 17) {
    std::cout << "Reduce sum result: " << sequences[index]->reduce(sum, 0) << '\n';
  } else if (choice == 18) {
    std::cout << "Item index: ";
    int itemIndex = read_int();
    std::cout << "Value: " << (*sequences[index])[itemIndex] << '\n';
  } else if (choice == 19) {
    std::cout << "Item index: ";
    int itemIndex = read_int();
    std::cout << "New value: ";
    int value = read_int();

    MutableArraySequence<int>* arraySequence = dynamic_cast<MutableArraySequence<int>*>(sequences[index]);
    MutableListSequence<int>* listSequence = dynamic_cast<MutableListSequence<int>*>(sequences[index]);

    if (arraySequence != nullptr) {
      (*arraySequence)[itemIndex] = value;
    } else if (listSequence != nullptr) {
      (*listSequence)[itemIndex] = value;
    } else {
      std::cout << "Writing through operator[] is unavailable for immutable sequences\n";
    }
  } else {
    std::cout << "Invalid choice\n";
    return;
  }

  std::cout << "Current value: ";
  print_sequence(sequences[index]);
  std::cout << '\n';
}

void create_bit_sequence() {
  if (bitSequenceCount >= MAX_OBJECTS) {
    std::cout << "BitSequence storage is full\n";
    return;
  }

  std::cout << "Enter BitSequence size: ";
  int count = read_non_negative_count();

  bool* items = nullptr;
  if (count > 0) {
    items = new bool[count];
  }

  for (int i = 0; i < count; i++) {
    std::cout << "Bit " << i << " (0/1): ";
    items[i] = read_int() != 0;
  }

  bitSequences[bitSequenceCount] = new BitSequence(items, count);
  delete[] items;

  std::cout << "BitSequence created at index " << bitSequenceCount << '\n';
  bitSequenceCount++;
}

void bit_sequence_operations() {
  int index = select_bit_sequence("Select BitSequence:");
  if (index == -1) return;

  std::cout << "1. Get first bit (get_first)\n";
  std::cout << "2. Get last bit (get_last)\n";
  std::cout << "3. Get bit (get)\n";
  std::cout << "4. Try get first bit (try_get_first)\n";
  std::cout << "5. Try get last bit (try_get_last)\n";
  std::cout << "6. Try get bit (try_get)\n";
  std::cout << "7. Get length (get_length)\n";
  std::cout << "8. Append bit (append)\n";
  std::cout << "9. Prepend bit (prepend)\n";
  std::cout << "10. Insert by index (insert_at)\n";
  std::cout << "11. Get subsequence (get_sub_sequence)\n";
  std::cout << "12. Bitwise AND\n";
  std::cout << "13. Bitwise OR\n";
  std::cout << "14. Bitwise XOR\n";
  std::cout << "15. Bitwise NOT\n";
  std::cout << "16. Read operator[]\n";
  std::cout << "17. Write through operator[]\n";
  std::cout << "Choice: ";
  int choice = read_int();

  if (choice == 1) {
    std::cout << "Value: " << bitSequences[index]->get_first() << '\n';
  } else if (choice == 2) {
    std::cout << "Value: " << bitSequences[index]->get_last() << '\n';
  } else if (choice == 3) {
    std::cout << "Bit index: ";
    int itemIndex = read_int();
    std::cout << "Value: " << bitSequences[index]->get(itemIndex) << '\n';
  } else if (choice == 4) {
    std::cout << "Value: ";
    print_bit_option(bitSequences[index]->try_get_first());
    std::cout << '\n';
  } else if (choice == 5) {
    std::cout << "Value: ";
    print_bit_option(bitSequences[index]->try_get_last());
    std::cout << '\n';
  } else if (choice == 6) {
    std::cout << "Bit index: ";
    int itemIndex = read_int();
    std::cout << "Value: ";
    print_bit_option(bitSequences[index]->try_get(itemIndex));
    std::cout << '\n';
  } else if (choice == 7) {
    std::cout << "Length: " << bitSequences[index]->get_length() << '\n';
  } else if (choice == 8) {
    std::cout << "Bit value (0/1): ";
    bitSequences[index]->append(read_int() != 0);
  } else if (choice == 9) {
    std::cout << "Bit value (0/1): ";
    bitSequences[index]->prepend(read_int() != 0);
  } else if (choice == 10) {
    std::cout << "Bit index: ";
    int itemIndex = read_int();
    std::cout << "Bit value (0/1): ";
    bitSequences[index]->insert_at(read_int() != 0, itemIndex);
  } else if (choice == 11) {
    if (bitSequenceCount >= MAX_OBJECTS) {
      std::cout << "BitSequence storage is full\n";
      return;
    }

    std::cout << "Start index: ";
    int startIndex = read_int();
    std::cout << "End index: ";
    int endIndex = read_int();
    Sequence<bool>* subsequence = bitSequences[index]->get_sub_sequence(startIndex, endIndex);
    bitSequences[bitSequenceCount] = static_cast<BitSequence*>(subsequence);
    std::cout << "Subsequence saved at index " << bitSequenceCount << '\n';
    bitSequenceCount++;
  } else if (choice >= 12 && choice <= 14) {
    if (bitSequenceCount >= MAX_OBJECTS) {
      std::cout << "BitSequence storage is full\n";
      return;
    }

    int secondIndex = select_bit_sequence("Select second BitSequence:");
    if (secondIndex == -1) return;

    if (choice == 12) {
      bitSequences[bitSequenceCount] = bitSequences[index]->bit_and(bitSequences[secondIndex]);
    } else if (choice == 13) {
      bitSequences[bitSequenceCount] = bitSequences[index]->bit_or(bitSequences[secondIndex]);
    } else {
      bitSequences[bitSequenceCount] = bitSequences[index]->bit_xor(bitSequences[secondIndex]);
    }

    std::cout << "Bitwise operation result saved at index " << bitSequenceCount << '\n';
    bitSequenceCount++;
  } else if (choice == 15) {
    if (bitSequenceCount >= MAX_OBJECTS) {
      std::cout << "BitSequence storage is full\n";
      return;
    }

    bitSequences[bitSequenceCount] = bitSequences[index]->bit_not();
    std::cout << "NOT result saved at index " << bitSequenceCount << '\n';
    bitSequenceCount++;
  } else if (choice == 16) {
    std::cout << "Bit index: ";
    int itemIndex = read_int();
    const BitSequence* sequence = bitSequences[index];
    std::cout << "Value: " << (*sequence)[itemIndex] << '\n';
  } else if (choice == 17) {
    std::cout << "Bit index: ";
    int itemIndex = read_int();
    std::cout << "Bit value (0/1): ";
    (*bitSequences[index])[itemIndex] = read_int() != 0;
  } else {
    std::cout << "Invalid choice\n";
    return;
  }

  std::cout << "Current value: ";
  print_bit_sequence(bitSequences[index]);
  std::cout << '\n';
}

void print_all_objects() {
  std::cout << "\nDynamicArray objects:\n";
  for (int i = 0; i < dynamicArrayCount; i++) {
    std::cout << i << ": ";
    print_dynamic_array(dynamicArrays[i]);
    std::cout << '\n';
  }

  std::cout << "\nLinkedList objects:\n";
  for (int i = 0; i < linkedListCount; i++) {
    std::cout << i << ": ";
    print_linked_list(linkedLists[i]);
    std::cout << '\n';
  }

  std::cout << "\nSequence<int> objects:\n";
  for (int i = 0; i < sequenceCount; i++) {
    std::cout << i << ": ";
    print_sequence(sequences[i]);
    std::cout << '\n';
  }

  std::cout << "\nBitSequence objects:\n";
  for (int i = 0; i < bitSequenceCount; i++) {
    std::cout << i << ": ";
    print_bit_sequence(bitSequences[i]);
    std::cout << '\n';
  }
}

void run_auto_demo() {
  if (sequenceCount + 2 > MAX_OBJECTS || bitSequenceCount + 2 > MAX_OBJECTS) {
    std::cout << "Not enough space for the auto demo\n";
    return;
  }

  int values[] = {1, -2, 3, 4};
  sequences[sequenceCount] = new MutableArraySequence<int>(values, 4);
  std::cout << "Auto: Sequence<int> created at index " << sequenceCount << '\n';
  sequenceCount++;

  sequences[sequenceCount] = sequences[sequenceCount - 1]->map(square);
  std::cout << "Auto: map(square) result saved at index " << sequenceCount << '\n';
  sequenceCount++;

  bool bits[] = {true, false, true, true};
  bitSequences[bitSequenceCount] = new BitSequence(bits, 4);
  std::cout << "Auto: BitSequence created at index " << bitSequenceCount << '\n';
  bitSequenceCount++;

  bitSequences[bitSequenceCount] = bitSequences[bitSequenceCount - 1]->bit_not();
  std::cout << "Auto: NOT result saved at index " << bitSequenceCount << '\n';
  bitSequenceCount++;
}

void map_reduce_operations() {
  std::cout << "1. zip two Sequence<int> objects\n";
  std::cout << "2. unzip pairs created from two Sequence<int> objects\n";
  std::cout << "3. skip\n";
  std::cout << "4. split by positive predicate\n";
  std::cout << "5. slice without replacement\n";
  std::cout << "6. slice with Sequence<int> replacement\n";
  std::cout << "7. flat_map: value to [value, -value]\n";
  std::cout << "Choice: ";
  int choice = read_int();

  if (choice == 1) {
    int firstIndex = select_sequence("Select first Sequence<int>:");
    if (firstIndex == -1) return;
    int secondIndex = select_sequence("Select second Sequence<int>:");
    if (secondIndex == -1) return;

    Sequence<Pair<int, int>>* result = zip(*sequences[firstIndex], *sequences[secondIndex]);
    std::cout << "Zip result: ";
    print_pair_sequence(result);
    std::cout << '\n';
    delete result;
  } else if (choice == 2) {
    int firstIndex = select_sequence("Select first Sequence<int> for pairs:");
    if (firstIndex == -1) return;
    int secondIndex = select_sequence("Select second Sequence<int> for pairs:");
    if (secondIndex == -1) return;

    Sequence<Pair<int, int>>* zipped = zip(*sequences[firstIndex], *sequences[secondIndex]);
    Pair<Sequence<int>*, Sequence<int>*> result = unzip(*zipped);

    std::cout << "First values: ";
    print_sequence(result.first());
    std::cout << '\n';
    std::cout << "Second values: ";
    print_sequence(result.second());
    std::cout << '\n';

    delete zipped;
    delete result.first();
    delete result.second();
  } else if (choice == 3) {
    if (sequenceCount >= MAX_OBJECTS) {
      std::cout << "Sequence storage is full\n";
      return;
    }

    int index = select_sequence("Select Sequence<int>:");
    if (index == -1) return;
    std::cout << "Number of items to skip: ";
    sequences[sequenceCount] = skip(*sequences[index], read_int());
    std::cout << "Skip result saved at index " << sequenceCount << '\n';
    sequenceCount++;
  } else if (choice == 4) {
    int index = select_sequence("Select Sequence<int>:");
    if (index == -1) return;

    Pair<Sequence<int>*, Sequence<int>*> result = split(*sequences[index], is_positive);
    std::cout << "Positive values: ";
    print_sequence(result.first());
    std::cout << '\n';
    std::cout << "Other values: ";
    print_sequence(result.second());
    std::cout << '\n';
    delete result.first();
    delete result.second();
  } else if (choice == 5 || choice == 6) {
    if (sequenceCount >= MAX_OBJECTS) {
      std::cout << "Sequence storage is full\n";
      return;
    }

    int index = select_sequence("Select Sequence<int>:");
    if (index == -1) return;

    const Sequence<int>* replacement = nullptr;
    if (choice == 6) {
      int replacementIndex = select_sequence("Select Sequence<int> replacement:");
      if (replacementIndex == -1) return;
      replacement = sequences[replacementIndex];
    }

    std::cout << "Start index (can be negative): ";
    int startIndex = read_int();
    std::cout << "Number of items to remove: ";
    int count = read_int();
    sequences[sequenceCount] = slice(*sequences[index], startIndex, count, replacement);
    std::cout << "Slice result saved at index " << sequenceCount << '\n';
    sequenceCount++;
  } else if (choice == 7) {
    if (sequenceCount >= MAX_OBJECTS) {
      std::cout << "Sequence storage is full\n";
      return;
    }

    int index = select_sequence("Select Sequence<int>:");
    if (index == -1) return;
    sequences[sequenceCount] = flat_map(*sequences[index], expand_with_negative);
    std::cout << "Flat_map result saved at index " << sequenceCount << '\n';
    sequenceCount++;
  } else {
    std::cout << "Invalid choice\n";
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
  std::cout << "1. DynamicArray<int> module\n";
  std::cout << "2. LinkedList<int> module\n";
  std::cout << "3. Sequence<int> module\n";
  std::cout << "4. MapReduce module\n";
  std::cout << "5. BitSequence module\n";
  std::cout << "6. Show all objects\n";
  std::cout << "0. Exit\n";
  std::cout << "Choice: ";
}

}

void run_menu() {
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
        std::cout << "Exit\n";
      } else {
        std::cout << "Invalid choice\n";
      }
    } catch (const std::exception& exception) {
      std::cout << "Error: " << exception.what() << '\n';
    }
  }

  clear_objects();
}

void run_dynamic_array_menu() {
  int choice = -1;

  while (choice != 0) {
    std::cout << "1. Create DynamicArray<int>\n";
    std::cout << "2. Run DynamicArray operation\n";
    std::cout << "3. Show all objects\n";
    std::cout << "0. Back\n";
    std::cout << "Choice: ";
    choice = read_int();

    try {
      if (choice == 1) {
        create_dynamic_array();
      } else if (choice == 2) {
        dynamic_array_operations();
      } else if (choice == 3) {
        print_all_objects();
      } else if (choice != 0) {
        std::cout << "Invalid choice\n";
      }
    } catch (const std::exception& exception) {
      std::cout << "Error: " << exception.what() << '\n';
    }
  }
}

void run_linked_list_menu() {
  int choice = -1;

  while (choice != 0) {
    std::cout << "1. Create LinkedList<int>\n";
    std::cout << "2. Run LinkedList operation\n";
    std::cout << "3. Show all objects\n";
    std::cout << "0. Back\n";
    std::cout << "Choice: ";
    choice = read_int();

    try {
      if (choice == 1) {
        create_linked_list();
      } else if (choice == 2) {
        linked_list_operations();
      } else if (choice == 3) {
        print_all_objects();
      } else if (choice != 0) {
        std::cout << "Invalid choice\n";
      }
    } catch (const std::exception& exception) {
      std::cout << "Error: " << exception.what() << '\n';
    }
  }
}

void run_sequence_menu() {
  int choice = -1;

  while (choice != 0) {
    std::cout << "1. Create Sequence<int>\n";
    std::cout << "2. Run Sequence operation\n";
    std::cout << "3. Show all objects\n";
    std::cout << "0. Back\n";
    std::cout << "Choice: ";
    choice = read_int();

    try {
      if (choice == 1) {
        create_sequence();
      } else if (choice == 2) {
        sequence_operations();
      } else if (choice == 3) {
        print_all_objects();
      } else if (choice != 0) {
        std::cout << "Invalid choice\n";
      }
    } catch (const std::exception& exception) {
      std::cout << "Error: " << exception.what() << '\n';
    }
  }
}

void run_map_reduce_menu() {
  int choice = -1;

  while (choice != 0) {
    std::cout << "1. Create Sequence<int>\n";
    std::cout << "2. Run MapReduce operation\n";
    std::cout << "3. Show all objects\n";
    std::cout << "0. Back\n";
    std::cout << "Choice: ";
    choice = read_int();

    try {
      if (choice == 1) {
        create_sequence();
      } else if (choice == 2) {
        map_reduce_operations();
      } else if (choice == 3) {
        print_all_objects();
      } else if (choice != 0) {
        std::cout << "Invalid choice\n";
      }
    } catch (const std::exception& exception) {
      std::cout << "Error: " << exception.what() << '\n';
    }
  }
}

void run_bit_sequence_menu() {
  int choice = -1;

  while (choice != 0) {
    std::cout << "1. Create BitSequence\n";
    std::cout << "2. Run BitSequence operation\n";
    std::cout << "3. Show all objects\n";
    std::cout << "0. Back\n";
    std::cout << "Choice: ";
    choice = read_int();

    try {
      if (choice == 1) {
        create_bit_sequence();
      } else if (choice == 2) {
        bit_sequence_operations();
      } else if (choice == 3) {
        print_all_objects();
      } else if (choice != 0) {
        std::cout << "Invalid choice\n";
      }
    } catch (const std::exception& exception) {
      std::cout << "Error: " << exception.what() << '\n';
    }
  }
}
