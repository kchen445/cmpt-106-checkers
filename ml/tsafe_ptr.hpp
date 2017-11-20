//
//  tsafe_ptr.hpp
//  process
//
//  Created by Jeremy S on 2017-11-17.
//  Copyright © 2017 Jeremy S. All rights reserved.
//

#pragma once

#include <cstdlib>
#include <thread>
#include <chrono>
#include "__mem.hpp"

#ifndef PTR_DELAY_TIME_NANO
#define PTR_DELAY_TIME_NANO 100
#endif

namespace lang {
    
    // Thread Safe Pointer
    // Similar to std::shared_ptr but only allows one pointer access to the memory at a given time.
    template<typename T>
    class tsafe_ptr {
    private:
        // Memory interface index value for this pointer.
        __p::m_idx mem_idx;
        
        // Waits until the memory is unlocked then returns the block that this pointer is pointing to.
        __p::__mem_block *wait_for_access () {
            __p::__mem_block *target_block = __p::__mem::__instance->unsafe_block_access(this->mem_idx);
            while (target_block->is_locked() || __p::__mem::__instance->is_locked()) {
                // As to not bombard the memory interface with checks, put the thread to sleep for
                // a set number of nanoseconds. This value may be configured before including this
                // file by defining a different PTR_DELAY_TIME_NANO.
                //
                // Whether this conserves CPU resourses or not is unknown.
                std::this_thread::sleep_for(std::chrono::nanoseconds{PTR_DELAY_TIME_NANO});
            }
            return target_block;
        }
        
    public:
        
        // Default constructor points this object to the null block.
        tsafe_ptr ()
        : mem_idx(0)
        {
            __p::__mem::__instance->increment_ref_count(mem_idx);
        }
        
        // Copy constructor sets this pointer to point to the same block as other.
        tsafe_ptr (tsafe_ptr<T> const &other)
        : mem_idx(other.mem_idx)
        {
            __p::__mem::__instance->increment_ref_count(mem_idx);
        }
        
        // Pointer contructor allocates a new memory block and brings the pointer in
        // the parameter under custom memory control.
        // The parameter pointer should be a pointer on the free store.
        //      e.g. lang::tsafe_ptr<int> ptr{new int(4)};
        // __mem::alloc takes care of incrementing the reference count for the block
        // so it is not explicity done in the body of the contructor.
        explicit tsafe_ptr (T *new_obj)
        : mem_idx(__p::__mem::__instance->alloc<T>(new_obj))
        {}
        
        // Deconstructor decrements the reference count for the targeted memory block.
        // If the reference count becomes zero, __mem::dealloc is called to delete the
        // object that was being pointed to. (this is handled by __p::__mem)
        ~tsafe_ptr () {
            __p::__mem::__instance->decrement_ref_count<T>(mem_idx);
        }
        
        // Operator = overload for lang::tsafe_ptr.
        // Will first decrement the reference count for the current trageted block
        // then switch blocks and increment the count for the new block.
        const tsafe_ptr& operator= (tsafe_ptr const &other) {
            __p::__mem::__instance->decrement_ref_count<T>(mem_idx);
            this->mem_idx = other.mem_idx;
            __p::__mem::__instance->increment_ref_count(mem_idx);
            return *this;
        }
        
        // Operator = overload for regular pointer.
        // Will first decrement the reference count for the current targeted block
        // then allocate a new block brining the pointer under memory control.
        const tsafe_ptr& operator= (T *ptr) {
            __p::__mem::__instance->decrement_ref_count<T>(mem_idx);
            this->mem_idx = __p::__mem::__instance->alloc(ptr);
            return *this;
        }
        
        
        /*
            Execution Functions
         
            Because only one pointer may be accessing the memory at any given time,
            these pointers are unable to return direct references to the underlying
            objects. To allow for complex manipulation of the underlying objects
            without returning references to them, these execution (exec) functions
            are used. Each exec function takes a lambda expression or function pointer
            which will be executed on the underlying object once access is granted.
         
            There are 2 types of exec functions, each with the option to pass arguments
            to them which acts like lambda captures.
                - void exec   : returns nothing
                - return exec : returns a given type
         
            Example code: (returns the sum of the pointed to int and another int)
         
                 int x = 5;
                 lang::tsafe_ptr<int> ptr{new int(5)};
                 int out = ptr.exec<int, int>(x, [](int &src, int arg) -> int {
                    return src + arg;
                 });
                 printf("output %d\n", out); // prints "output 10"
         */
        
        // Return exec with argument.
        template<typename return_type, typename arg_type>
        return_type exec (arg_type const &arg, return_type(*func)(T &, arg_type const &)) {
            return_type output;
            __p::__mem_block *target_block = wait_for_access();
            target_block->locked = true;
            output = func(*((T *)__p::__mem::__instance->unsafe_raw_access(mem_idx)), arg);
            target_block->locked = false;
            return output;
        }
        
        // Same as above but argument is passed by value instead of const reference.
        template<typename return_type, typename arg_type>
        return_type exec (arg_type const &arg, return_type(*func)(T &, arg_type)) {
            return_type output;
            __p::__mem_block *target_block = wait_for_access();
            target_block->locked = true;
            output = func(*((T *)__p::__mem::__instance->unsafe_raw_access(mem_idx)), arg);
            target_block->locked = false;
            return output;
        }
        
        // Void exec with argument.
        template<typename arg_type>
        void exec (arg_type const &arg, void(*func)(T &, arg_type const &)) {
            __p::__mem_block *target_block = wait_for_access();
            target_block->locked = true;
            func(*((T *)__p::__mem::__instance->unsafe_raw_access(mem_idx)), arg);
            target_block->locked = false;
        }
        
        // Same as above but argument is passed by value.
        template<typename arg_type>
        void exec (arg_type const &arg, void(*func)(T &, arg_type)) {
            __p::__mem_block *target_block = wait_for_access();
            target_block->locked = true;
            func(*((T *)__p::__mem::__instance->unsafe_raw_access(mem_idx)), arg);
            target_block->locked = false;
        }
        
        // Return exec with no argument.
        template<typename return_type>
        return_type exec (return_type(*func)(T &)) {
            return_type output;
            __p::__mem_block *target_block = wait_for_access();
            target_block->locked = true;
            output = func(*((T *)__p::__mem::__instance->unsafe_raw_access(mem_idx)));
            target_block->locked = false;
            return output;
        }
        
        // Void exec with no argument.
        void exec (void(*func)(T &)) {
            __p::__mem_block *target_block = wait_for_access();
            target_block->locked = true;
            func(*((T *)__p::__mem::__instance->unsafe_raw_access(mem_idx)));
            target_block->locked = false;
        }
        
        
        // Returns a copy of the value that is pointed to by this pointer.
        T get () {
            return exec<T> ([](T &src) -> T {
                return src;
            });
        }
        
        // Returns a const reference to the value that is pointed to by this pointer.
        // If other threads are activly modifying the underlying value, this reference
        // may change. It is almost always a better idea to use get() instead to ensure
        // that the object returned will not be modified by other threads.
        const T& get_ref () {
            return *exec<T*> ([](T &src) -> T* {
                return &src;
            });
        }
        
        // Sets the underlying value to be a given value.
        void set (T const &new_val) {
            exec<T>([](T &src, T const &n_val) {
                src = n_val;
            });
        }
        
        // Unsafe raw access to the underlying pointer.
        // It is safe to use this if only one thread is executing.
        T *unsafe_raw () {
            return (T *)__p::__mem::__instance->unsafe_raw_access(mem_idx);
        }
        
    }; // class tsafe_ptr<T>
    
} // namespace lang
