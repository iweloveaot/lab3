#ifndef _OPTION_H_
#define _OPTION_H_

#include "exceptions.h"

template<typename T>
class Option {
private:
    T* value;
    bool hasValue;

public:
    Option() : value(nullptr), hasValue(false) {}
    
    Option(const T& val) : hasValue(true) {
        try {
            value = new T(val);
        } catch (...) {
            throw MemoryAllocationException("Failed to allocate Option value");
        }
    }
    
    Option(const Option& other) : hasValue(other.hasValue), value(nullptr) {
        if (hasValue) {
            try {
                value = new T(*other.value);
            } catch (...) {
                throw MemoryAllocationException("Failed to allocate Option value in copy");
            }
        }
    }
    
    ~Option() {
        delete value;
    }
    
    Option& operator=(const Option& other) {
        if (this != &other) {
            delete value;
            hasValue = other.hasValue;
            if (hasValue) {
                try {
                    value = new T(*other.value);
                } catch (...) {
                    value = nullptr;
                    hasValue = false;
                    throw MemoryAllocationException("Failed to allocate Option value in assignment");
                }
            } else {
                value = nullptr;
            }
        }
        return *this;
    }
    
    bool HasValue() const { return hasValue; }
    
    const T& GetValue() const {
        if (!hasValue)
            throw EmptyContainerException("Option has no value");
        return *value;
    }
    
    T GetValueOrDefault(const T& defaultValue) const {
        return hasValue ? *value : defaultValue;
    }
    
    static Option<T> None() {
        return Option<T>();
    }
    
    static Option<T> Some(const T& val) {
        return Option<T>(val);
    }
};

#endif /* _OPTION_H_ */