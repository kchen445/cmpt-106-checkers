//
//  p_error.hpp
//  process
//
//  Created by Jeremy S on 2017-11-16.
//  Copyright © 2017 Jeremy S. All rights reserved.
//

#pragma once

#include <exception>

namespace lang {
    namespace __p {
        
        struct p_error : public std::exception {
            const char *msg;
            
            p_error (const char *m)
            : std::exception(),
              msg(m)
            {}
            
            virtual ~p_error() {};
            
            virtual const char *what () const noexcept override {
                return this->msg;
            }
        };
        
    }
    
    struct mem_error : public __p::p_error {
        mem_error (const char *msg)
        : __p::p_error(msg)
        {}
    };
    
}
