cmake_minimum_required(VERSION 3.16)
project(LaboratoryWork2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(LaboratoryWork2
        main.cpp
        src/menu.cpp
        include/Menu/menu.h
        include/DynamicArray/dynamic_array.h
        include/DynamicArray/dynamic_array.tpp
        include/LinkedList/linked_list.h
        include/LinkedList/linked_list.tpp
        include/Iterator/ienumerator.h
        include/Option/option.h
        include/Sequence/sequence.h
        include/Sequence/sequence.tpp
        include/MapReduce/map_reduce.h
        include/MapReduce/map_reduce.tpp
        include/Bit/bit_sequence.h
        include/Bit/bit_sequence.tpp
)

add_executable(AllTests
        tests/all_tests.cpp
        include/DynamicArray/dynamic_array.h
        include/DynamicArray/dynamic_array.tpp
        include/LinkedList/linked_list.h
        include/LinkedList/linked_list.tpp
        include/Iterator/ienumerator.h
        include/Option/option.h
        include/Sequence/sequence.h
        include/Sequence/sequence.tpp
        include/Bit/bit_sequence.h
        include/Bit/bit_sequence.tpp
)
