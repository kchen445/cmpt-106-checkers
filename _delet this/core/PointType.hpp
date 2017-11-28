//
//  PointType.hpp
//  lang
//
//  Created by Jeremy S on 2017-09-07.
//  Copyright © 2017 Jeremy S. All rights reserved.
//
// lang::math::foundation::PointType<T, N>              (template class)
//      A generic class to represent a fixed sequence of values in the form
//      of a N dimensional mathematical point.
//
// Usage:
// Parameter Pack Construction:
//      PointType<int, 3> p = {1, 0, 4};
//                  or
//      PointType<int, 3> p{1, 0, 4};

#ifndef __math_point_type_h__
#define __math_point_type_h__

#include <ostream>
#include <cmath>

namespace lang { 
    namespace math {
        namespace foundation {
            
            // Both lang::math::Point and lang::math::Vector extend this class.
            //
            // T must have a default and copy constructor
            // N should be > 0
            template<typename T, size_t N>
            class PointType {
            private:
                
                // Array of values stored values.
                T* __values;
                
            public:
                
                // Default constructor.
                // Sets __values to be an array of size N
                // setting each index to the default value for T.
                PointType () {
                    __values = new T[N];
                    for (int i = 0; i < N; ++i) {
                        __values[i] = T{};
                    }
                }
                
                // Copy constructor.
                PointType (PointType<T, N> const &other) {
                    __values = new T[N];
                    for (int i = 0; i < N; ++i) {
                        __values[i] = other.value(i);
                    }
                }
                
                // On destruction, delete __values array.
                virtual ~PointType () {
                    delete[] __values;
                }
                
                // Generic copy constructor.
                // T must be constructable from U.
                template<typename U>
                PointType (PointType<U, N> const &other) {
                    __values = new T[N];
                    for (int i = 0; i < N; ++i) {
                        __values[i] = static_cast<T>(other.value(i));
                    }
                }
                
                // Parameter pack constructor.
                // All Ts values must be of type T
                template<typename ... Ts>
                PointType (Ts ... _v) {
                    T _array[N] = {(T)_v ...};
                    __values = new T[N];
                    for (int i = 0; i < N; ++i) {
                        __values[i] = _array[i];
                    }
                }
                
                // Overloaded = operator for C style array.
                PointType<T, N> operator= (T array[N]) {
                    for (int i = 0; i < N; ++i) {
                        __values[i] = array[i];
                    }
                    return *this;
                }
                
                // Return the value at index i.
                // Does bound checking on i.
                T value (int i) const {
                    if (i >= N || i < 0) {
                        throw std::out_of_range("PointType<N>::value(T) const: index out of range");
                    }
                    return __values[i];
                }
                
                // Return the value at index i as a reference.
                // Does bound checking on i.
                T& value (int i) {
                    if (i >= N || i < 0) {
                        throw std::out_of_range("PointType<N>::value(T): index out of range");
                    }
                    return __values[i];
                }
                
                // Overload [] operators (same as PointType<T,N>::value(int))
                T operator[] (int i) const {
                    return value(i);
                }
                
                T& operator[] (int i) {
                    return value(i);
                }
                
                // Overload[] operator for size_t parameter.
                T operator[] (size_t i) const {
                    return value((int)i);
                }
                
                T& operator[] (size_t i) {
                    return value((int)i);
                }
                
                
                // Returns a PointType<T,N> will all values set to 0.
                // Only use this is T is constructable from 0, ie int, double etc.
                static PointType<T, N> zero () {
                    return PointType<T, N>{};
                }
                
                // Returns the magnitude of the point type using formula:
                //      sqrt(sqr(x1) + sqr(x2) + ... + sqr(xn))
                //
                // The distance from the point to the zero point.
                double magnitude () const {
                    double sum = 0;
                    for (int i = 0; i < N; ++i) {
                        sum += (value(i) * value(i));
                    }
                    return sqrt(sum);
                }
                
                // Returns the distance between two points.
                double distanceFrom (PointType<T, N> const &other) const {
                    return (*this - other).magnitude();
                }
                
                
                // ===== Operators ===== //
                PointType<T, N> operator- () const {
                    PointType<T, N> p;
                    for (int i = 0; i < N; ++i) {
                        p[i] = -value(i);
                    }
                    return p;
                }
                
                PointType<T, N> operator+ (PointType<T, N> const &other) const {
                    PointType<T, N> p;
                    for (int i = 0; i < N; ++i) {
                        p[i] = value(i) + other[i];
                    }
                    return p;
                }
                
                PointType<T, N> operator- (PointType<T, N> const &other) const {
                    return *this + (-other);
                }
                
                PointType<T, N> operator* (T const &multiple) const {
                    PointType<T, N> p;
                    for (int i = 0; i < N; ++i) {
                        p[i] = value(i) * multiple;
                    }
                    return p;
                }
                
                bool operator== (PointType<T, N> const &other) const {
                    for (int i = 0; i < N; ++i) {
                        if (value(i) != other[i]) {
                            return false;
                        }
                    }
                    return true;
                }
                
            };
            
            
            // Extension class of PointType.
            // Contains convenience variables for interfacing with the values of the point.
            // 
            // Ex: (Same for PointType3D)
            //      PointType2D<int> p = {0, 0};
            //      p.x = 5;
            //      *** p is now (5, 0) ***
            //
            // **** NOTE ****
            // Point values may still be edited using the reference variables even if the
            // variable is declared const.
            template<typename T>
            class PointType2D : public PointType<T, 2> {
            public:
                
                // Reference variables for value in __value array.
                T& x;
                T& y;
                
                PointType2D ()
                : PointType<T, 2>(), x(this->value(0)), y(this->value(1))
                {}
                
                PointType2D (T x, T y)
                : PointType<T, 2>(x, y), x(this->value(0)), y(this->value(1))
                {}
                
                PointType2D (PointType<T, 2> const &other)
                : PointType<T, 2>(other), x(this->value(0)), y(this->value(1))
                {}
                
                const PointType2D<T>& operator= (PointType2D<T> const &other) {
                    this->value(0) = other.value(0);
                    this->value(1) = other.value(1);
                    return *this;
                }
                
                // Returns the inverse of this point.
                // (x, y) -> (y, x)
                PointType2D<T> inverse () const {
                    return PointType2D<T>(y, x);
                }
                
                // Mutating method.
                // Inverts this point.
                void invert () {
                    T tmp = x;
                    x = y;
                    y = tmp;
                }
                
            };
            
            
            // Extension class of PointType.
            // Contains convenience variables for interfacing with the values of the point.
            template<typename T>
            class PointType3D : public PointType<T, 3> {
            public:
                
                // Reference variables for value in __value array.
                T& x;
                T& y;
                T& z;
                
                PointType3D ()
                : PointType<T, 3>(), x(this->value(0)), y(this->value(1)), z(this->value(2))
                {}
                
                PointType3D (T x, T y, T z)
                : PointType<T, 3>(x, y, z), x(this->value(0)), y(this->value(1)), z(this->value(2))
                {}
                
                PointType3D (PointType<T, 3> const &other)
                : PointType<T, 3>(other), x(this->value(0)), y(this->value(1)), z(this->value(2))
                {}
                
                const PointType3D<T>& operator= (PointType3D<T> const &other) {
                    this->value(0) = other.value(0);
                    this->value(1) = other.value(1);
                    this->value(2) = other.value(2);
                    return *this;
                }
                
            };
            
            
            // Stream insertion overload.
            // Format:
            //      (x1, x2, ..., xn)
            template<typename T, size_t N>
            inline std::ostream& operator<< (std::ostream &os, PointType<T, N> const &p) {
                os << '(';
                    for (int i = 0; i < N; ++i) {
                        os << p.value(i);
                        if (i != N - 1) {
                            os << ", ";
                        }
                    }
                os << ')';
                return os;
            }
            
            
        } // namespace foundation
        
    } // namespace math
} // namespace lang

template<typename T, size_t N>
inline lang::math::foundation::PointType<T, N> operator* (T const &multiple,
                                                          lang::math::foundation::PointType<T, N> const &point)
{
    return point * multiple;
}


#endif
