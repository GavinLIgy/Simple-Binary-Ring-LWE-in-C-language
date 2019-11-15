/*
Introduction:
BRLWE is a light-weighted noval PQC algorithm based on RLWE which supersede the Gaussian noise with Binary noise.
BRLWE scheme consists of three main phases: key generation, encryption, and decryption.


*/


/*****************************************************************************/
/* Includes:                                                                 */
/*****************************************************************************/
#include <stdint.h>
#include <string.h>
#include <time.h>
#include<stdlib.h>
#include "brlwe.h"

/*****************************************************************************/
/* Private variables:                                                        */
/*****************************************************************************/

/*****************************************************************************/
/* Private functions:                                                        */
/*****************************************************************************/

//initialize a polynomial by sampling a uniform distribution with binary coefficients 
void BRLWE_init_bin_sampling(struct BRLWE_Ring_polynomials* poly) { 
	time_t t;
	int i = 0;
	int r = 0;
	srand((unsigned)time(&t));
	for (int i = 0; i < BRLWE_N; i++) {
		r = rand() % 2;//more powerful RNG required.
		poly->polynomial[i] = (uint8_t) r;
	};
};

//initialize a polynomial by input hex in form of string.
//pre-requirement: length(str) = n, rev = {0,1}^1
//rev = 1: str[n] = poly[0]; else: str[0] = poly[0]
void BRLWE_init_hex(struct BRLWE_Ring_polynomials* poly, uint8_t* str, int rev) {
	if (rev == 1) {
		for (int i = 0; i < BRLWE_N; i++)
			poly->polynomial[i] = (uint8_t)str[BRLWE_N-1-i];
	}
	else {
		for (int i = 0; i < BRLWE_N; i++)
			poly->polynomial[i] = (uint8_t)str[i];
	};
};

//initialize a polynomial with all 0.
void BRLWE_init(struct BRLWE_Ring_polynomials* poly) {
	for (int i = 0; i < BRLWE_N; i++)
		poly->polynomial[i] = (uint8_t)0x00;
};

//Main Function 1: Key Generation
//a is a global parameter shared by Alice and Bob
// r1 and r2 are randomly selected binary polynomials, r2 is secret key
// p = r1 - a * r2, p is public key and would be sent to Bob after Key_Gen
void BRLWE_Key_Gen(const struct BRLWE_Ring_polynomials* a, struct BRLWE_Ring_polynomials* p, struct BRLWE_Ring_polynomials* r2) {
	struct BRLWE_Ring_polynomials r1;
	BRLWE_init_bin_sampling(&r1);
	BRLWE_init_bin_sampling(r2);
	*p = Ring_sub(r1, Simple_Ring_mul(*a, *r2));
	return;
};

//Main Function 2: Encryption
//pre-requirement: length(m) = n, m belongs to {0,1}^n;
//a is a global parameter shared by Alice and Bob, p is public key and would be sent to Bob after Key_Gen, m is the message to be crypto
//After receiving p, Bob uses 3 error(binary) polynomials e1, e2, e3
//m_wave = encode(m), c1 = a*e1 +e2, c2 = p*e1 + e3 + m_wave
//[c1,c2] belonging to R_q^2 are cipertext
struct BRLWE_Ring_polynomials2 BRLWE_Encry(const struct BRLWE_Ring_polynomials* a, const struct BRLWE_Ring_polynomials* p, uint8_t* m) {
	struct BRLWE_Ring_polynomials e1, e2, e3, m_wave;
	struct BRLWE_Ring_polynomials2 c;
	BRLWE_init_bin_sampling(&e1);
	BRLWE_init_bin_sampling(&e2);
	BRLWE_init_bin_sampling(&e3);
	BRLWE_init(&c.c1);
	BRLWE_init(&c.c2);
	BRLWE_Encode(m, &m_wave);
	c.c1 = Ring_add(Simple_Ring_mul(*a, e1), e2);
	c.c2 = Ring_add(Ring_add(Simple_Ring_mul(*p, e1), e3), m_wave);
	return c;
};

//Main Function 3: Decryption
//output m' = Decode(c1*r2+c2)
//r2 is secret key
uint8_t* BRLWE_Decry(struct BRLWE_Ring_polynomials2* c, struct BRLWE_Ring_polynomials* r2) {
	return BRLWE_Decode(Ring_add(Simple_Ring_mul(c->c1, *r2), c->c2));
};

//Encode string m into polynomial m_wave
//pre-requirement: length(m) = n, m belongs to {0,1}^n;;
void BRLWE_Encode(uint8_t* m, struct BRLWE_Ring_polynomials* m_wave) {
	int i = 0;
	for (i = 0; i < BRLWE_N; i++) {
		if (m[i] == 0) continue;
		else m[i] = (uint8_t)(BRLWE_Q / 2);
	};
	BRLWE_init_hex(m_wave, m, 0);
	return;
};

//Decode polynomial m_wave into string m
uint8_t* BRLWE_Decode(struct BRLWE_Ring_polynomials m_wave){
	uint8_t* m = (uint8_t *)malloc(sizeof(uint8_t) * BRLWE_N);
	int i = 0;
	for (i = 0; i < BRLWE_N; i++) {
		if (abs(m_wave.polynomial[i] - i - round((BRLWE_N - 3) / 2)) <= (BRLWE_Q / 4) ) m[i] = 0;
		else m[i] = 1;
	};
	return m;
};

//return value = a + b;
struct BRLWE_Ring_polynomials Ring_add(const struct BRLWE_Ring_polynomials a, const struct BRLWE_Ring_polynomials b) {
	int i = 0;
	struct BRLWE_Ring_polynomials r;
	for (int i = 0; i < BRLWE_N; i++) {
		r.polynomial[i] = (a.polynomial[i] + b.polynomial[i]) % BRLWE_Q;
	};
	return r;
};

//return value = a - b;
struct BRLWE_Ring_polynomials Ring_sub(const struct BRLWE_Ring_polynomials a, const struct BRLWE_Ring_polynomials b) {
	int i = 0;
	struct BRLWE_Ring_polynomials r;
	for (int i = 0; i < BRLWE_N; i++) {
		r.polynomial[i] = (a.polynomial[i] - b.polynomial[i]) % BRLWE_Q;
	};
	return r;
};

//return value = a * b; b is with binary coefficiences
struct BRLWE_Ring_polynomials Simple_Ring_mul(const struct BRLWE_Ring_polynomials a, const struct BRLWE_Ring_polynomials b) {
	int i = 0;
	int j = 0;
	struct BRLWE_Ring_polynomials r;
	BRLWE_init(&r);
	for (int i = 0; i < BRLWE_N; i++) {
		if (b.polynomial[i] == 0x01) {
			for (int j = 0; j < BRLWE_N; j++) {
				if (i + j <= BRLWE_N - 1) 
					r.polynomial[i + j] = r.polynomial[i + j] + a.polynomial[j];
				else 
					r.polynomial[i + j - BRLWE_N] = r.polynomial[i + j - BRLWE_N] + 256 - a.polynomial[j];
			};
		};
	};
	return r;
};