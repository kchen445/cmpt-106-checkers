/*
 * Optional.h, version 2.00
 *
 * Created by Jeremy Schwartz on August 24, 2017.
 *
 * A generic class that may be used to store a value that may be nil.
 *
 * Abstract:
 *
 *   Optional<T>        (type)
 *      A template type that may contain any value
 *      associated with its type or a nil.
 *
 *      Optional<T>.value() may be used to interact
 *      with the value held by the optional type but
 *      it will throw an exception if the optional is
 *      nil.
 *
 *   NilType            (type)
 *      An empty type to represent nil.
 *
 *      lang::nil is a global constant of this type.
 */

#ifndef __lang_optional_h__
#define __lang_optional_h__

#include <ostream>
#include <stdexcept>

namespace lang {
#ifndef __nil_defined__
#define __nil_defined__
    namespace framework {
        // Blank type for the nil constant.
        struct NilType {};

    } // namespace framework

    // Constant NilType definition.
    const framework::NilType nil{};
#endif
    
    // Generic Optional class.
    // T may be any value besides NilType.
    // T must have a copy constructor.
    template<typename T>
    class Optional {
    private:
        bool __isNil;
        T* __value;

    protected:
        // Throws an exception if object is set to nil.
        void assertNotNil () const {
            if (__isNil) {
                throw std::runtime_error("Optional<T>::assertNotNil(): Optional value is nil.");
            }
        }

    public:

        // Default Constructor.
        // Sets the optional to nil
        // Does not call any constructor for the templated type.
        Optional () {
            this->__isNil = true;
            this->__value = nullptr;
        }

        // Copy Constructor.
        // Calls copy constructor for type T
        Optional (T const &value) {
            this->__isNil = false;
            this->__value = new T{value};
        }

        // Nil Constructor.
        // Sets this object to nil.
        Optional (framework::NilType const &nilType) {
            this->__isNil = true;
            this->__value = nullptr;
        }
        
        // Free __value if it is not nullptr.
        ~Optional () {
            if (__value != nullptr) {
                delete __value;
                __value = nullptr;
                __isNil = true;
            }
        }
        
        // Overload = operator from Optional type.
        const Optional<T>& operator= (Optional<T> const &other) {
            if (other.isNil()) {
                __isNil = true;
            } else {
                if (__value == nullptr) {
                    __value = new T{*other.__value};
                    __isNil = false;
                } else {
                    *__value = *other.__value;
                    __isNil = false;
                }
            }
            return *this;
        }
        
        // Overload = operator from NilType.
        const Optional<T>& operator= (framework::NilType const &nilType) {
            __isNil = true;
            return *this;
        }
        
        // Overload = operator from templated type.
        const Optional<T>& operator= (T const &value) {
            if (__value == nullptr) {
                __value = new T{value};
            } else {
                *__value = value;
            }
            
            __isNil = false;
            return *this;
        }

        // Constant and Reference methods for interfacing with the contained value.
        // Throws std::runtime_error if object is set to nil.
        T value () const {
            assertNotNil();
            return *__value;
        }

        T& value () {
            assertNotNil();
            return *__value;
        }

        // Returns true if the object is set to nil.
        bool isNil () const {
            return __isNil;
        }

        // Returns true if the object is not set to nil.
        bool hasValue () const {
            return !__isNil;
        }

        // Overload == operator.
        // Returns true if both objects are set to nil or their values are the same.
        bool operator== (Optional<T> const &other) const {
            if (__isNil && other.__isNil) {
                return true;
            } else if (__isNil && !other.__isNil) {
                return false;
            } else {
                return __value == other.__value;
            }
        }

        bool operator== (framework::NilType const &other) const {
            return this->isNil();
        }

        bool operator!= (framework::NilType const &other) const {
            return this->hasValue();
        }

    };

} // namespace lang

// Overload << operator.
// Format:
//      "Optional(<value>)" or "nil" if object is nil
//
// T must have an << operator to use this function.
template<typename T>
std::ostream& operator<< (std::ostream &os, lang::Optional<T> const &anOptional) {
    if (anOptional.isNil()) {
        os << "nil";
    } else {
        os << "Optional(" << anOptional.value() << ")";
    }
    return os;
}

#endif
