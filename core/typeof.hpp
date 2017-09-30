//
//  typeof.h
//  lang
//
//  Created by Jeremy S on 2017-09-05.
//  Copyright © 2017 Jeremy S. All rights reserved.
//

#ifndef typeof_h
#define typeof_h

template<typename T>
inline const char* type_of () {
    return "";
}

#define GEN_TYPENAME(A) template<> inline const char* type_of<A>() { return #A; }

GEN_TYPENAME(char)
GEN_TYPENAME(int)
GEN_TYPENAME(long)
GEN_TYPENAME(unsigned char)
GEN_TYPENAME(unsigned int)
GEN_TYPENAME(unsigned long)
GEN_TYPENAME(float)
GEN_TYPENAME(double)


#endif /* typeof_h */
