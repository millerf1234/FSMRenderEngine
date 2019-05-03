
//  Work In Progress....

//
//  Reference Sources:
//          https://stackoverflow.com/questions/16976720/how-do-i-restrict-a-template-class-to-certain-built-in-types
//          http://www2.math.uconn.edu/~troby/Math2210S14/LT/sec4_3.pdf
//

//  The Idea
//Template < [float-type] T, [size_type] n>
//Class linearlyIndependentVectors {
//   Construct an object consisting of 2 n-vectors of T. Upon construction or modification, verify 
//   their linear dependence from each other. Maintain a customizable tolerance value close
//   to 0 to be able to detect cancellation error and what not. 
//    
//   If constructed with a linearly-dependent pair, throw an exception. If 
// }
// 
//


#pragma once

#ifndef LINEARLY_INDEPENDENT_VECTOR_PAIR_H_
#define LINEARLY_INDEPENDENT_VECTOR_PAIR_H_

#include <type_traits>

#include "GraphicsLanguage.h"

template<typename RealType, size_t n>
class LinearlyIndependentVectorPair {
   /* static_asset(std::is_floating_point<RealType>::value,
        "A LinearlyIndependentVectorPair can only be instantiated with floating point types!");
*/
    
private:
    glm::vec3 up;
    glm::vec3 forward;

};





#endif //LINEARLY_INDEPENDENT_VECTOR_PAIR_H_