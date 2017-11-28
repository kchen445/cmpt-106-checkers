//
//  Point.hpp
//  lang
//
//  Created by Jeremy S on 2017-09-07.
//  Copyright © 2017 Jeremy S. All rights reserved.

#ifndef __math_point_h__
#define __math_point_h__

#include "PointType.hpp"

// Redefinition of lang::math::foundation::PointType<T,N> with T as int.

namespace lang {
    
    namespace math {
        
        template<size_t N>
        using Point = foundation::PointType<int, N>;
        
        using Point2D = foundation::PointType2D<int>;
        
        using Point3D = foundation::PointType3D<int>;
        
    } // namespace math
} // namespace lang




#endif
