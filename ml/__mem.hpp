//
//  __mem.hpp
//  process
//
//  Created by Jeremy S on 2017-11-15.
//  Copyright © 2017 Jeremy S. All rights reserved.
//

#pragma once

#include <vector>
#include "p_error.hpp"

#ifndef __MEM_SIZE
#define __MEM_SIZE 256
#endif

namespace lang {
    namespace __p {
        
        /// Memory index type (unsigned long)
        using m_idx = size_t;
        
        
        struct __mem_block {
            void *ptr;
            size_t ref_count;
            bool locked;
            
            __mem_block ()
            : ptr(nullptr), ref_count(0), locked(false)
            {}
            
            bool is_locked () const {
                return locked;
            }
        };
        
        
        class __mem {
        private:
            
            // The interface for memory access.
            // Index 0 is kept as a null block.
            std::vector<__mem_block> mem_interf{__MEM_SIZE, __mem_block{}};
            
            bool full_lock = false;
            
            size_t usage = 0;
            
            bool is_block_free (m_idx idx) {
                return mem_interf.at(idx).ref_count == 0;
            }
            
        public:
            
            static __mem *__instance;
            
            /// Provieds raw access to the pointer within the memory block at a given index.
            ///
            /// Throws lang::mem_error if idx == 0 or underlying pointer is nullptr.
            void *unsafe_raw_access (m_idx idx) {
                if (idx == 0) {
                    throw mem_error{"memory interface via unsafe_raw_access: bad access [m_idx 0]"};
                } else if (mem_interf.at(idx).ptr == nullptr) {
                    throw mem_error{"memory interface via unsafe_raw_access: physical bad access [nullptr]"};
                }
                return this-> mem_interf.at(idx).ptr;
            }
            
            /// Provides raw access to the memory block at a given index.
            __mem_block *unsafe_block_access (m_idx idx) {
                try {
                    return &this->mem_interf.at(idx);
                } catch (std::out_of_range e) {
                    std::string emsg = "out of range exception: m_idx = " + std::to_string(idx);
                    throw mem_error{emsg.c_str()};
                }
            }
            
            bool is_locked () const {
                return full_lock;
            }
            
            /// Allocates a new block of memory.
            template<typename T>
            m_idx alloc (T *ptr) {
                m_idx idx = 0;
                for (m_idx i = 1; i < mem_interf.size(); ++i) {
                    if (is_block_free(i)) {
                        idx = i;
                        break;
                    }
                }
                
                // Check to see if a free block was found
                // If not then increase the size of mem_interf
                if (idx == 0) {
                    full_lock = true; // lock the whole memory interface
                    // As resizing the vector may cause memory to be moved arround,
                    // the interface is locked so that no process can access memory
                    // as it is being moved.
                    
                    m_idx current_size = mem_interf.size();
                    mem_interf.resize(current_size + __MEM_SIZE);
                    full_lock = false; // unlock the interface
                    idx = current_size; // set idx to a new mem index
                }
                
                // mark the memory at idx to be in use by incrementing the ref_cout
                mem_interf.at(idx).ref_count++;
                
                // allocate the physical memory
                mem_interf.at(idx).ptr = ptr;
                
                return idx;
            }
            
            /// Deallocates memory with a given index
            template<typename T>
            void dealloc (m_idx idx) {
                if (idx != 0) {
                    if (mem_interf.at(idx).ptr == nullptr) {
                        throw mem_error{"memory interface via dealloc: attempt to deallocate nullptr"};
                    }
                    delete (T *)mem_interf.at(idx).ptr;
                }
            }
            
            
            void increment_ref_count (m_idx idx) {
                mem_interf.at(idx).ref_count++;
            }
            
            template<typename T>
            void decrement_ref_count (m_idx idx) {
                mem_interf.at(idx).ref_count--;
                if (mem_interf[idx].ref_count == 0) {
                    dealloc<T>(idx);
                }
            }
            
        };
        
        __mem *__mem::__instance = new __mem{};
        
    } // namespace __p
} // namespace lang
