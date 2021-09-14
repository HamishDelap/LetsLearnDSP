#ifndef _LLDSP_LIB_
#define _LLDSP_LIB_

#include <cmath>


/**
 * @brief Applies tanh waveshaping to a signal on a per sample basis.
 * 
 * @param sample 
 * @param gain 
 * @return float 
 */
namespace lldsp{
    
    float tanhDistortion(float sample, float gain) {
        return std::tanh(sample * gain);
    }
    
}
#endif