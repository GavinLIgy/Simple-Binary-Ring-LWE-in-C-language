#ifndef _POLYNOMIAL_H_
#define _POLYNOMIAL_H_

#include "parameter.h"
#include <stdint.h>

struct poly
{
    uint8_t coeff[BRLWE_N];
};

#endif
