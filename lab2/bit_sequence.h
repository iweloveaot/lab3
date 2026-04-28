#ifndef _BIT_SEQUENCE_H_
#define _BIT_SEQUENCE_H_

#include "mutable_array_sequence.h"
#include "exceptions.h"

class Bit {
private:
    bool value;

public:
    Bit(bool val = false) : value(val) {}

    Bit& operator=(bool val) {
        value = val;
        return *this;
    }

    Bit(int val) : value(val != 0) {}

    Bit& operator=(int val) {
        value = (val != 0);
        return *this;
    }

    operator bool() const {
        return value;
    }

    Bit operator~() const {
        return Bit(!value);
    }

    Bit operator&(const Bit& other) const {
        return Bit(value && other.value);
    }

    Bit operator|(const Bit& other) const {
        return Bit(value || other.value);
    }

    Bit operator^(const Bit& other) const {
        return Bit(value != other.value);
    }

    Bit& operator&=(const Bit& other) {
        value = value && other.value;
        return *this;
    }

    Bit& operator|=(const Bit& other) {
        value = value || other.value;
        return *this;
    }

    Bit& operator^=(const Bit& other) {
        value = value != other.value;
        return *this;
    }

    bool operator==(const Bit& other) const {
        return value == other.value;
    }

    bool operator!=(const Bit& other) const {
        return value != other.value;
    }
};

class BitSequence : public MutableArraySequence<Bit> {
private:
    BitSequence* applyBitwise(const BitSequence& other,
                              Bit (Bit::*op)(const Bit&) const) const {
        if (GetLength() != other.GetLength()) {
            throw InvalidArgumentException("BitSequence lengths must match for bitwise operations:", GetLength());
        }
        BitSequence* result = new BitSequence();
        if (result == nullptr)
            throw MemoryAllocationException("Error during bit operation");
        for (int i = 0; i < GetLength(); ++i) {
            result->AppendImplict((Get(i).*op)(other.Get(i)));
        }
        return result;
    }

public:
    BitSequence() : MutableArraySequence<Bit>() {}

    BitSequence(const int* items, int count) : MutableArraySequence<Bit>() {
        if (count < 0) throw InvalidArgumentException("Negative count");
        for (int i = 0; i < count; ++i) {
            AppendImplict(Bit(items[i]));
        }
    }    

    BitSequence(const bool* items, int count) : MutableArraySequence<Bit>() {
        if (count < 0) throw InvalidArgumentException("Negative count");
        for (int i = 0; i < count; ++i) {
            AppendImplict(Bit(items[i]));
        }
    }

    BitSequence(const Bit* items, int count) : MutableArraySequence<Bit>(items, count) {}

    explicit BitSequence(int size) : MutableArraySequence<Bit>(size) {}

    BitSequence(const DynamicArray<Bit>& arr) : MutableArraySequence<Bit>(arr) {}

    BitSequence(const BitSequence& other) : MutableArraySequence<Bit>(other) {}

    ~BitSequence() override {};

    // ===== Переопределение GetSubsequence для возврата правильного типа =====
    BitSequence* GetSubsequence(int startIndex, int endIndex) const {
        if (startIndex < 0 || endIndex >= GetLength() || startIndex > endIndex)
            throw IndexOutOfRangeException("Invalid subsequence indexes");
        
        DynamicArray<Bit> tmp_array(endIndex - startIndex + 1);
        int tmp_ind = 0;
        for (int i = startIndex; i <= endIndex; i++) {
            tmp_array.Set(tmp_ind, this->Get(i));
            tmp_ind++;
        }
        try {
            return new BitSequence(tmp_array);
        } catch (...) {
            throw MemoryAllocationException("Error while getting bit subsequence");
        }
    }

    BitSequence* operator~() const {
        BitSequence* result = new BitSequence();
        if (result == nullptr)
            throw MemoryAllocationException("Error during bit operation ~");
        for (int i = 0; i < GetLength(); ++i) {
            result->AppendImplict(~this->Get(i));
        }
        return result;
    }

    BitSequence* operator&(const BitSequence& other) const {
        return applyBitwise(other, &Bit::operator&);
    }

    BitSequence* operator|(const BitSequence& other) const {
        return applyBitwise(other, &Bit::operator|);
    }

    BitSequence* operator^(const BitSequence& other) const {
        return applyBitwise(other, &Bit::operator^);
    }

    bool GetBit(int index) const {
        return bool(this->Get(index));
    }
};

#endif /* _BIT_SEQUENCE_H_ */