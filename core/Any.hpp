//
//  Any.hpp, version 2.00
//  lang
//
//  Created by Jeremy S on 2017-09-05.
//  Copyright © 2017 Jeremy S. All rights reserved.
//
//  Any                 (object)
//      A type that stores a single value of any type (int, string, vector, etc.)
//      Arrays of 'Any' may be used to create arrays that contain more than one
//      type.  The function lang::as<T>(lang::Any) is used to cast an 'Any' type
//      into a normal type or object.  It is up the the programer to ensure that
//      these casts are valid.  The function will throw an exception if it is
//      unable to cast but there are some cases where it is able to cast but it
//      is still invalid.

#ifndef Any_hpp
#define Any_hpp

#include <stdlib.h>
#include <string> // Use of std::string instead of lang::String for compatibility.
#include "typeof.hpp"

namespace lang {
    
    class Any {
    private:
        
        std::shared_ptr<void> ptr;
        int size;
        std::string elemType;
        
    public:
        
        Any ()
        : ptr(nullptr, free), size(0), elemType()
        {}
        
        Any (Any const &other)
        : ptr(other.ptr), size(other.size), elemType(other.elemType)
        {}
        
        template<typename T>
        Any (T object)
        : ptr(malloc(sizeof(object)), free), size(sizeof(object)), elemType(type_of<T>())
        {
            std::memcpy(ptr.get(), &object, size);
        }
        
        ~Any () {
            size = 0;
        }
        
        const Any& operator= (Any const &other) {
            this->size = other.size;
            this->ptr = other.ptr;

            return *this;
        }
        
        template<typename T>
        const Any& operator= (T object) {
            ptr = std::shared_ptr<void>{malloc(sizeof(T))};
            this->size = sizeof(T);
            
            std::memcpy(ptr.get(), &object, size);
            return *this;
        }
        
        // Return the type as a string that the 'Any' type holds.
        // Returns "" if type does not have a typeof function defined for it.
        // To define a name for a type use the macro defined in typeof.hpp
        //      GEN_TYPENAME(<type>)
        std::string type () const {
            return this->elemType != "" ? this->elemType : "";
        }
        
        template<typename T>
        friend bool is (Any const &any);
        
        template<typename T>
        friend T as (Any const &any);
        
    };
    
    // Return true if 'Any' type is able to be converted into a given type.
    // If the given type has a type name defined, this function will compare
    // the type name of the given type with the one stored in the 'Any' type
    // to see if conversion is posible. If not, then the size of the type is
    // used which may lead to some flase positives.
    //      ex. If no type is defined for long and double, then since each
    //      has a size of 8 bytes, is<double>(Any<long>) will return true.
    template<typename T>
    inline bool is (Any const &any) {
        if (any.elemType != "" || type_of<T>() != "") {
            return any.elemType == type_of<T>();
        } else {
            return sizeof(T) == any.size;
        }
    }
    
    // Casts an any type to a given type. Throws std::logic_error if
    // unable to cast. Calls lang::is<T> to check for valid cast.
    template<typename T>
    inline T as (Any const &any) {
        if (!is<T>(any)) {
            throw std::logic_error("as<T>(Any): Given 'Any' type may not be converted to this type.");
        }
        
        return T{*static_cast<T*>(any.ptr.get())};
    }
    
}

#include <ostream>
inline std::ostream& operator<< (std::ostream &os, lang::Any const &any) {
    os << "Any" << '(';
    if (any.type() != "") {
        os << any.type();
    } else {
        os << "???";
    }
    os << ')';
    return os;
}

#endif /* Any_hpp */
