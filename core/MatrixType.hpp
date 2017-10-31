//
//  Matrix.hpp
//  lang
//
//  Created by Jeremy S on 2017-09-07.
//  Copyright © 2017 Jeremy S. All rights reserved.
//
// lang::math::foundation::MatrixType<T, M, N>          (template class)
//      A generic class to represent a matrix like objects (2 dimensional fixed array)
//      Designed to be used as a base class for other objects. May also be redefined
//      for specific template specifications.
//      ex:
//          template<size_t M, size_t N>
//          using lang::math::Matrix = lang::math::framework::MatrixType<double, M, N>;
//              as defined in Matrix.hpp
//
//      Template specifications:
//          T: typename, any type
//              must have a default and copy constructor
//          M: size_t (unsigned long), number of rows in the matrix
//          N: size_t (unsigned long), number of columns in the matrix
//
//      Usage:
//      Example Class Extension:
//          class TicTacToeBoard : public lang::math::foundation::MatrixType<char, 3, 3> {}
//
//      Example Parameter Pack Constuction: (using lang::math::Matrix)
//          lang::math::Matrix<3, 3> A = {
//              1, 0, 1
//              0, 1, 0
//              1, 0, 1
//          };

#ifndef __math_matrix_hpp__
#define __math_matrix_hpp__

// #include "Array.hpp" // For use with lang::Array from the lang library.
#ifndef Array_hpp 
    
    #include <vector>

    namespace lang {
        template<typename T>
        using Array = std::vector<T>;
    }    

#endif

#include "PointType.hpp"

#define double_loop_start(M, N) for (size_t i = 0; i < M; ++i) { for (size_t j = 0; j < N; ++j) {
#define double_loop_end } }

namespace lang {
    namespace math {
        namespace foundation {
            

            // As defined above:
            //  T is the contained type
            //  M is the number of rows
            //  N is the number of columns
            template<typename T, size_t M, size_t N>
            class MatrixType {
            private:
                
                // Storage array
                T __array[M][N];
                
                // Calls a given funtion or lambda statement for each element in the matrix.
                void foreach (void(*func)(T&)) {
                    for (size_t i = 0; i < M; ++i) {
                        for (size_t j = 0; j < N; ++j) {
                            func(__array[i][j]);
                        }
                    }
                }
                
            public:
                
                // Default Constructor
                // Calls T's default constructor.
                MatrixType () {
                    foreach([](T &elem){
                        elem = T{};
                    });
                }
                
                // Copy Constructor
                // Calls T's copy constructor or copy operator= if expicitly defined.
                MatrixType (MatrixType<T, M, N> const &other) {
                    double_loop_start(M, N)
                    this->__array[i][j] = other.__array[i][j];
                    double_loop_end
                }
                
                // Parameter pack constructor.
                // All parameter values must be of type T
                //
                // throws std::length_error if invalid number of parameters.
                template<typename ... Values>
                MatrixType (Values ... values) {
                    Array<T> valuesAsArray{static_cast<T>(values) ...};
                    
                    if (valuesAsArray.size() != M * N) {
                        throw std::length_error("Invalid number of arguments");
                    }
                    
                    for (size_t i = 0; i < M; ++i) {
                        for (size_t j = 0; j < N; ++j) {
                            __array[i][j] = valuesAsArray[(i * N) + j];
                        }
                    }
                }
                
                // Element Interface functions //
                // (row, column) indexing
                T at (size_t i, size_t j) const {
                    return __array[i][j];
                }
                
                T& at (size_t i, size_t j) {
                    return __array[i][j];
                }
                
                T at (int i, int j) const {
                    return at(static_cast<size_t>(i), static_cast<size_t>(j));
                }
                
                T& at (int i, int j) {
                    return at(static_cast<size_t>(i), static_cast<size_t>(j));
                }
                
                // Operator [] Overloading //
                // Returns a N dimensional point coresponding to the row at a given index.
                PointType<T, N> operator[] (size_t i) const {
                    return row(i);
                }
                
                // Returns true if matrix is square.
                bool isSquare () const {
                    return M == N;
                }
                
                // Returns the row at a given index as a N dimensional point.
                PointType<T, N> row (size_t i) const {
                    if (i >= M) {
                        throw std::out_of_range("index out of range");
                    }
                    
                    PointType<T, N> p;
                    for (size_t j = 0; j < N; ++j) {
                        p[j] = __array[i][j];
                    }
                    return p;
                }
                
                // Returns the column at a given index as a M dimensional point.
                PointType<T, M> column (size_t j) const {
                    if (j >= N) {
                        throw std::out_of_range("index out of range");
                    }
                    
                    PointType<T, M> p;
                    for (size_t i = 0; i < M; ++i) {
                        p[i] = __array[i][j];
                    }
                    return p;
                }
                
                // Returns the matrix as an array of rows.
                Array<PointType<T, N>> rows () const {
                    Array<PointType<T, N>> output;
                    for (size_t i = 0; i < M; ++i) {
                        output.push_back(row(i));
                    }
                    return output;
                }
                
                // Returns the matrix as an array of columns.
                Array<PointType<T, M>> columns () const {
                    Array<PointType<T, M>> output;
                    for (size_t i = 0; i < N; ++i) {
                        output.push_back(column(i));
                    }
                    return output;
                }
                
                // Sets a row to a given N dimensional point.
                void setRow (size_t i, PointType<T, N> const &point_t) {
                    if (i >= M) {
                        throw std::out_of_range("index out of range");
                    }
                
                    for (size_t j = 0; j < N; ++j) {
                        at(i, j) = point_t[j];
                    }
                }
                
                // Sets a column to a given M dimensional point.
                void setColumn (size_t j, PointType<T, M> const &point_t) {
                    if (j >= N) {
                        throw std::out_of_range("index out of range");
                    }
                    
                    for (size_t i = 0; i < M; ++i) {
                        at(i, j) = point_t[i];
                    }
                }
                
            };
            
        } // namespace foundation
    } // namespace math
} // namespace lang


//           //
// Operators //
//           //

// Follows linear algebraic rules for matrix operations.

// For ease of use
#define matrix_t lang::math::foundation::MatrixType

// Sum of matricies
template<typename T, size_t M, size_t N>
inline matrix_t<T, M, N> operator+ (matrix_t<T, M, N> const &lhs, matrix_t<T, M, N> const &rhs) {
    matrix_t<T, M, N> sum;
    
    double_loop_start(M, N)
    sum.at(i, j) = lhs.at(i, j) + rhs.at(i, j);
    double_loop_end
    
    return sum;
}

// Scalar product of matrix and scalar.
template<typename T, size_t M, size_t N>
inline matrix_t<T, M, N> operator* (T const &lhs, matrix_t<T, M, N> const &rhs) {
    matrix_t<T, M, N> scalarProduct;
    
    double_loop_start(M, N)
    scalarProduct.at(i, j) = lhs * rhs.at(i, j);
    double_loop_end
}

template<typename T, size_t M, size_t N>
inline matrix_t<T, M, N> operator* (matrix_t<T, M, N> const &lhs, T const &rhs) {
    return rhs * lhs;
}

// Subtraction of matricies.
template<typename T, size_t M, size_t N>
inline matrix_t<T, M, N> operator- (matrix_t<T, M, N> const &lhs, matrix_t<T, M, N> const &rhs) {
    matrix_t<T, M, N> sum;
    
    double_loop_start(M, N)
    sum.at(i, j) = lhs.at(i, j) - rhs.at(i, j);
    double_loop_end
    
    return sum;
}

#undef matrix_t // remove as this macro should not exist outside of this section
#undef double_loop_start
#undef double_loop_end

#endif /* Matrix_h */
