#ifndef _POLY_H_
#define _POLY_H_

#include "polynomial.h"
#include "parameter.h"

void poly_add (poly* poly_r, const poly* poly_1, const poly* poly_2);
void poly_sub (poly* poly_r, const poly* poly_1, const poly* poly_2);
void poly_mul (poly* poly_r, const poly* poly_1, const poly* poly_2);
void poly_random (poly* poly_1, uint8_t* coin);

#endif
