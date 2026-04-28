#include <iostream>
#include <cassert>
#include <cmath>
#include "mutable_array_sequence.h"
#include "immutable_array_sequence.h"
#include "mutable_list_sequence.h"
#include "immutable_list_sequence.h"
#include "bit_sequence.h"
#include "option.h"
#include "exceptions.h"

// Счётчик тестов
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST(name) void name()
#define RUN_TEST(name) do { \
    try { \
        std::cout << "Running: " << #name << "... "; \
        name(); \
        std::cout << "✓ PASSED\n"; \
        tests_passed++; \
    } catch (const std::exception& e) { \
        std::cout << "✗ FAILED: " << e.what() << "\n"; \
        tests_failed++; \
    } catch (...) { \
        std::cout << "✗ FAILED: Unknown exception\n"; \
        tests_failed++; \
    } \
} while(0)

#define ASSERT_EQ(a, b) do { \
    if ((a) != (b)) { \
        throw std::runtime_error("Assertion failed: " + std::to_string(a) + " != " + std::to_string(b)); \
    } \
} while(0)

#define ASSERT_TRUE(cond) do { \
    if (!(cond)) { \
        throw std::runtime_error("Assertion failed: condition is false"); \
    } \
} while(0)

#define ASSERT_FALSE(cond) do { \
    if (cond) { \
        throw std::runtime_error("Assertion failed: expected false, but got true"); \
    } \
} while(0)

#define ASSERT_THROW(expr, ExceptionType) do { \
    bool caught = false; \
    try { expr; } \
    catch (const ExceptionType&) { caught = true; } \
    if (!caught) throw std::runtime_error("Expected exception " #ExceptionType " not thrown"); \
} while(0)

// ==================== DynamicArray Tests ====================

TEST(test_dynamic_array_creation) {
    DynamicArray<int> arr1;
    ASSERT_EQ(arr1.GetSize(), 0);
    
    int data[] = {1, 2, 3, 4, 5};
    DynamicArray<int> arr2(data, 5);
    ASSERT_EQ(arr2.GetSize(), 5);
    ASSERT_EQ(arr2.Get(0), 1);
    ASSERT_EQ(arr2.Get(4), 5);
    
    DynamicArray<int> arr3(10);
    ASSERT_EQ(arr3.GetSize(), 10);
}

TEST(test_dynamic_array_copy) {
    int data[] = {10, 20, 30};
    DynamicArray<int> arr1(data, 3);
    DynamicArray<int> arr2(arr1);
    
    ASSERT_EQ(arr2.GetSize(), 3);
    ASSERT_EQ(arr2.Get(1), 20);
    
    arr2.Set(1, 99);
    ASSERT_EQ(arr1.Get(1), 20); 
    ASSERT_EQ(arr2.Get(1), 99);
}

TEST(test_dynamic_array_resize) {
    int data[] = {1, 2, 3};
    DynamicArray<int> arr(data, 3);
    
    arr.Resize(5);
    ASSERT_EQ(arr.GetSize(), 5);
    ASSERT_EQ(arr.Get(0), 1);
    ASSERT_EQ(arr.Get(2), 3);
    
    arr.Resize(2);
    ASSERT_EQ(arr.GetSize(), 2);
    ASSERT_EQ(arr.Get(1), 2);
}

TEST(test_dynamic_array_exceptions) {
    DynamicArray<int> arr;
    ASSERT_THROW(arr.Get(-1), IndexOutOfRangeException);
    ASSERT_THROW(arr.Get(0), IndexOutOfRangeException);
    
    int data[] = {1, 2, 3};
    DynamicArray<int> arr2(data, 3);
    ASSERT_THROW(arr2.Get(3), IndexOutOfRangeException);
    ASSERT_THROW(arr2.Set(5, 10), IndexOutOfRangeException);
    ASSERT_THROW(DynamicArray<int>(-5), InvalidArgumentException);
}

// ==================== LinkedList Tests ====================

TEST(test_linked_list_creation) {
    LinkedList<int> list1;
    ASSERT_EQ(list1.GetLength(), 0);
    
    int data[] = {5, 10, 15};
    LinkedList<int> list2(data, 3);
    ASSERT_EQ(list2.GetLength(), 3);
    ASSERT_EQ(list2.GetFirst(), 5);
    ASSERT_EQ(list2.GetLast(), 15);
}

TEST(test_linked_list_append_prepend) {
    LinkedList<int> list;
    list.Append(1);
    list.Append(2);
    list.Prepend(0);
    
    ASSERT_EQ(list.GetLength(), 3);
    ASSERT_EQ(list.Get(0), 0);
    ASSERT_EQ(list.Get(1), 1);
    ASSERT_EQ(list.Get(2), 2);
}

TEST(test_linked_list_insert_at) {
    int data[] = {1, 2, 4, 5};
    LinkedList<int> list(data, 4);
    list.InsertAt(3, 2);
    
    ASSERT_EQ(list.GetLength(), 5);
    ASSERT_EQ(list.Get(2), 3);
    ASSERT_EQ(list.Get(4), 5);
}

TEST(test_linked_list_sublist) {
    int data[] = {0, 1, 2, 3, 4};
    LinkedList<int> list(data, 5);
    LinkedList<int>* sub = list.GetSubList(1, 3);
    
    ASSERT_EQ(sub->GetLength(), 3);
    ASSERT_EQ(sub->Get(0), 1);
    ASSERT_EQ(sub->Get(2), 3);
    delete sub;
}

TEST(test_linked_list_exceptions) {
    LinkedList<int> list;
    ASSERT_THROW(list.GetFirst(), EmptyContainerException);
    ASSERT_THROW(list.GetLast(), EmptyContainerException);
    ASSERT_THROW(list.Get(0), IndexOutOfRangeException);
    
    int data[] = {1, 2, 3};
    LinkedList<int> list2(data, 3);
    ASSERT_THROW(list2.Get(-1), IndexOutOfRangeException);
    ASSERT_THROW(list2.Get(3), IndexOutOfRangeException);
    ASSERT_THROW(list2.InsertAt(5, 10), IndexOutOfRangeException);
}

// ==================== Sequence Tests (ArraySequence) ====================

TEST(test_mutable_array_sequence_basic) {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);
    
    ASSERT_EQ(seq.GetLength(), 3);
    ASSERT_EQ(seq.GetFirst(), 1);
    ASSERT_EQ(seq.GetLast(), 3);
    ASSERT_EQ(seq.Get(1), 2);
}

TEST(test_mutable_array_sequence_append_prepend) {
    MutableArraySequence<int> seq;
    seq.Append(1)->Append(2)->Append(3);
    
    ASSERT_EQ(seq.GetLength(), 3);
    ASSERT_EQ(seq.Get(0), 1);
    
    seq.Prepend(0);
    ASSERT_EQ(seq.Get(0), 0);
    ASSERT_EQ(seq.GetLength(), 4);
}

TEST(test_mutable_array_sequence_insert_concat) {
    int data1[] = {1, 2};
    int data2[] = {3, 4};
    MutableArraySequence<int> seq1(data1, 2);
    MutableArraySequence<int> seq2(data2, 2);
    
    seq1.InsertAt(99, 1);
    ASSERT_EQ(seq1.Get(1), 99);
    
    seq1.Concat(&seq2);
    ASSERT_EQ(seq1.GetLength(), 5);
    ASSERT_EQ(seq1.Get(4), 4);
}

TEST(test_immutable_array_sequence) {
    int data[] = {1, 2, 3};
    ImmutableArraySequence<int> seq(data, 3);
    
    auto newSeq = seq.Append(4);
    ASSERT_EQ(seq.GetLength(), 3);      
    ASSERT_EQ(newSeq->GetLength(), 4);  
    ASSERT_EQ(newSeq->Get(3), 4);
    
    delete newSeq;
}

TEST(test_array_sequence_map) {
    int data[] = {1, 2, 3, 4};
    MutableArraySequence<int> seq(data, 4);
    
    auto doubled = seq.Map([](const int& x) { return x * 2; });
    
    ASSERT_EQ(doubled->GetLength(), 4);
    ASSERT_EQ(doubled->Get(0), 2);
    ASSERT_EQ(doubled->Get(3), 8);
    
    delete doubled;
}

TEST(test_array_sequence_where) {
    int data[] = {1, 2, 3, 4, 5, 6};
    MutableArraySequence<int> seq(data, 6);
    
    auto evens = seq.Where([](const int& x) { return x % 2 == 0; });
    
    ASSERT_EQ(evens->GetLength(), 3);
    ASSERT_EQ(evens->Get(0), 2);
    ASSERT_EQ(evens->Get(2), 6);
    
    delete evens;
}

TEST(test_array_sequence_reduce) {
    int data[] = {1, 2, 3, 4};
    MutableArraySequence<int> seq(data, 4);
    
    int sum;
    seq.Reduce([](const int& a, const int& b) { return a + b; }, 0, &sum);
    ASSERT_EQ(sum, 10);
    
    int product;
    seq.Reduce([](const int& a, const int& b) { return a * b; }, 1, &product);
    ASSERT_EQ(product, 24);
}

TEST(test_array_sequence_try_get) {
    int data[] = {2, 4, 6, 8};
    MutableArraySequence<int> seq(data, 4);
    
    auto first_even = seq.TryGetFirst([](const int& x) { return x % 2 == 0; });
    ASSERT_TRUE(first_even.HasValue());
    ASSERT_EQ(first_even.GetValue(), 2);
    
    auto first_odd = seq.TryGetFirst([](const int& x) { return x % 2 == 1; });
    ASSERT_TRUE(!first_odd.HasValue());
    
    auto last_large = seq.TryGetLast([](const int& x) { return x > 5; });
    ASSERT_TRUE(last_large.HasValue());
    ASSERT_EQ(last_large.GetValue(), 8);
}

// ==================== Sequence Tests (ListSequence) ====================

TEST(test_mutable_list_sequence_basic) {
    int data[] = {10, 20, 30};
    MutableListSequence<int> seq(data, 3);
    
    ASSERT_EQ(seq.GetLength(), 3);
    ASSERT_EQ(seq.GetFirst(), 10);
    ASSERT_EQ(seq.GetLast(), 30);
}

TEST(test_immutable_list_sequence) {
    int data[] = {1, 2, 3};
    ImmutableListSequence<int> seq(data, 3);
    
    auto newSeq = seq.Append(4);
    ASSERT_EQ(seq.GetLength(), 3);
    ASSERT_EQ(newSeq->GetLength(), 4);
    
    delete newSeq;
}

TEST(test_list_sequence_map_reduce) {
    int data[] = {1, 2, 3};
    MutableListSequence<int> seq(data, 3);
    
    auto squared = seq.Map([](const int& x) { return x * x; });
    ASSERT_EQ(squared->Get(2), 9);
    
    int sum;
    squared->Reduce([](const int& a, const int& b) { return a + b; }, 0, &sum);
    ASSERT_EQ(sum, 14);  // 1 + 4 + 9
    
    delete squared;
}

// ==================== BitSequence Tests ====================

TEST(test_bit_sequence_creation) {
    bool bits[] = {true, false, true, false};
    BitSequence seq(bits, 4);
    
    ASSERT_EQ(seq.GetLength(), 4);
    ASSERT_TRUE(seq.GetBit(0));
    ASSERT_FALSE(seq.GetBit(1));
}

TEST(test_bit_sequence_bitwise_not) {
    bool bits[] = {true, false, true};
    BitSequence seq(bits, 3);
    
    auto inverted = ~seq;
    ASSERT_FALSE(inverted->GetBit(0));
    ASSERT_TRUE(inverted->GetBit(1));
    ASSERT_FALSE(inverted->GetBit(2));
    
    delete inverted;
}

TEST(test_bit_sequence_bitwise_and) {
    bool bits1[] = {true, false, true, false};
    bool bits2[] = {true, true, false, false};
    BitSequence seq1(bits1, 4);
    BitSequence seq2(bits2, 4);
    
    auto result = seq1 & seq2;
    ASSERT_TRUE(result->GetBit(0));
    ASSERT_FALSE(result->GetBit(1));
    ASSERT_FALSE(result->GetBit(2));
    ASSERT_FALSE(result->GetBit(3));
    
    delete result;
}

TEST(test_bit_sequence_bitwise_or_xor) {
    bool bits1[] = {true, false, true};
    bool bits2[] = {false, false, true};
    BitSequence seq1(bits1, 3);
    BitSequence seq2(bits2, 3);
    
    auto or_result = seq1 | seq2;
    ASSERT_TRUE(or_result->GetBit(0));
    ASSERT_FALSE(or_result->GetBit(1));
    ASSERT_TRUE(or_result->GetBit(2));
    
    auto xor_result = seq1 ^ seq2;
    ASSERT_TRUE(xor_result->GetBit(0));
    ASSERT_FALSE(xor_result->GetBit(1));
    ASSERT_FALSE(xor_result->GetBit(2));
    
    delete or_result;
    delete xor_result;
}

TEST(test_bit_sequence_as_mask) {
    bool mask_bits[] = {true, false, true, false, true};
    BitSequence mask(mask_bits, 5);
    
    // Using as bitmask: check which positions are set
    for (int i = 0; i < mask.GetLength(); ++i) {
        if (i % 2 == 0) ASSERT_TRUE(mask.GetBit(i));
        else ASSERT_FALSE(mask.GetBit(i));
    }
}

// ==================== Option Tests ====================

TEST(test_option_some) {
    Option<int> opt(42);
    ASSERT_TRUE(opt.HasValue());
    ASSERT_EQ(opt.GetValue(), 42);
}

TEST(test_option_none) {
    Option<int> opt = Option<int>::None();
    ASSERT_TRUE(!opt.HasValue());
    ASSERT_EQ(opt.GetValueOrDefault(100), 100);
}

TEST(test_option_copy_assignment) {
    Option<int> opt1(10);
    Option<int> opt2(opt1);
    ASSERT_TRUE(opt2.HasValue());
    ASSERT_EQ(opt2.GetValue(), 10);
    
    Option<int> opt3;
    opt3 = opt1;
    ASSERT_EQ(opt3.GetValue(), 10);
}

// ==================== Iterator Tests ====================

TEST(test_array_enumerator) {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);
    
    auto* iter = seq.GetEnumerator();
    ASSERT_TRUE(iter->HasNext());
    ASSERT_EQ(iter->GetCurrent(), 1);
    
    ASSERT_TRUE(iter->HasNext());
    ASSERT_EQ(iter->GetCurrent(), 2);
    
    ASSERT_TRUE(iter->HasNext());
    ASSERT_EQ(iter->GetCurrent(), 3);
    
    ASSERT_TRUE(!iter->HasNext());
    
    iter->Reset();
    ASSERT_TRUE(iter->HasNext());
    ASSERT_EQ(iter->GetCurrent(), 1);
    
    delete iter;
}

TEST(test_list_enumerator) {
    int data[] = {10, 20, 30};
    MutableListSequence<int> seq(data, 3);
    
    auto* iter = seq.GetEnumerator();
    int count = 0;
    while (iter->HasNext()) {
        iter->GetCurrent();
        count++;
    }
    ASSERT_EQ(count, 3);
    
    delete iter;
}

// ==================== Exception Handling Tests ====================

TEST(test_exception_inheritance) {
    try {
        throw IndexOutOfRangeException("Test", 5);
    } catch (const BaseException& e) {
        ASSERT_TRUE(e.what() != nullptr);
    }
}

TEST(test_null_reference_exception) {
    MutableArraySequence<int>* nullSeq = nullptr;
    ASSERT_THROW(nullSeq->GetLength(), NullReferenceException);
}

// ==================== Edge Cases Tests ====================

TEST(test_empty_sequence_operations) {
    MutableArraySequence<int> seq;
    
    ASSERT_THROW(seq.GetFirst(), EmptyContainerException);
    ASSERT_THROW(seq.GetLast(), EmptyContainerException);
    
    auto filtered = seq.Where([](const int& x) { return x > 0; });
    ASSERT_EQ(filtered->GetLength(), 0);
    delete filtered;
}

TEST(test_subsequence_boundaries) {
    int data[] = {0, 1, 2, 3, 4};
    MutableArraySequence<int> seq(data, 5);
    
    auto sub1 = seq.GetSubsequence(0, 4);  // Full
    ASSERT_EQ(sub1->GetLength(), 5);
    
    auto sub2 = seq.GetSubsequence(2, 2);  // Single element
    ASSERT_EQ(sub2->GetLength(), 1);
    ASSERT_EQ(sub2->Get(0), 2);
    
    delete sub1;
    delete sub2;
}

TEST(test_negative_index_exceptions) {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);
    
    ASSERT_THROW(seq.Get(-1), IndexOutOfRangeException);
    ASSERT_THROW(seq.GetSubsequence(-1, 1), IndexOutOfRangeException);
    ASSERT_THROW(seq.InsertAt(5, -1), IndexOutOfRangeException);
}

// ==================== Main Test Runner ====================
int runAllTests() {
    std::cout << "=== Running Lab2 Sequences Tests ===\n\n";
    
    // DynamicArray
    RUN_TEST(test_dynamic_array_creation);
    RUN_TEST(test_dynamic_array_copy);
    RUN_TEST(test_dynamic_array_resize);
    RUN_TEST(test_dynamic_array_exceptions);
    
    // LinkedList
    RUN_TEST(test_linked_list_creation);
    RUN_TEST(test_linked_list_append_prepend);
    RUN_TEST(test_linked_list_insert_at);
    RUN_TEST(test_linked_list_sublist);
    RUN_TEST(test_linked_list_exceptions);
    
    // ArraySequence
    RUN_TEST(test_mutable_array_sequence_basic);
    RUN_TEST(test_mutable_array_sequence_append_prepend);
    RUN_TEST(test_mutable_array_sequence_insert_concat);
    RUN_TEST(test_immutable_array_sequence);
    RUN_TEST(test_array_sequence_map);
    RUN_TEST(test_array_sequence_where);
    RUN_TEST(test_array_sequence_reduce);
    RUN_TEST(test_array_sequence_try_get);
    
    // ListSequence
    RUN_TEST(test_mutable_list_sequence_basic);
    RUN_TEST(test_immutable_list_sequence);
    RUN_TEST(test_list_sequence_map_reduce);
    
    // BitSequence
    RUN_TEST(test_bit_sequence_creation);
    RUN_TEST(test_bit_sequence_bitwise_not);
    RUN_TEST(test_bit_sequence_bitwise_and);
    RUN_TEST(test_bit_sequence_bitwise_or_xor);
    RUN_TEST(test_bit_sequence_as_mask);
    
    // Option
    RUN_TEST(test_option_some);
    RUN_TEST(test_option_none);
    RUN_TEST(test_option_copy_assignment);
    
    // Iterators
    RUN_TEST(test_array_enumerator);
    RUN_TEST(test_list_enumerator);
    
    // Exceptions
    RUN_TEST(test_exception_inheritance);
    
    // Edge cases
    RUN_TEST(test_empty_sequence_operations);
    RUN_TEST(test_subsequence_boundaries);
    RUN_TEST(test_negative_index_exceptions);
    
    std::cout << "\n=== Test Summary ===\n";
    std::cout << "Passed: " << tests_passed << "\n";
    std::cout << "Failed: " << tests_failed << "\n";
    std::cout << "Total:  " << (tests_passed + tests_failed) << "\n";
    
    return tests_failed > 0 ? 1 : 0;
}