#include "poly.h"

void poly_add (poly* poly_r, const poly* poly_1, const poly* poly_2) 
{
    int i;
    for(i = 0; i < BRLWE_N; i++) {
        //x mod 1<<logq = x & (q-1)
        poly_r[i] = (poly_1[i] + poly_2[i]) & BRLWE_Q;
    };

};

void poly_sub (poly* poly_r, const poly* poly_1, const poly* poly_2) 
{
    int i;
    for(i = 0; i < BRLWE_N; i++) {
        //x mod 1<<logq = x & (q-1)
        poly_r[i] = (poly_1[i] - poly_2[i]) & BRLWE_Q;
    };
};

void poly_mul (poly* poly_r, const poly* poly_1, const poly* poly_2, int k)
{
//sparse mul: poly_1 is dense, poly_2 is sparse
    if (k == 0)
        //column wise
    else if (k == BRLWE_N)
        //row wise
    else
        //hybrid

};

void poly_random (poly* poly_1, uint8_t* coin) 
{


};

