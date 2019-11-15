#include<stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "brlwe.h"

static void phex(uint8_t* str);

int main(void) {
	int end = 0;

	//testbench of each functions
	struct BRLWE_Ring_polynomials a, m, pk, sk;
	uint8_t test_1[4] = { (uint8_t)30, (uint8_t)20, (uint8_t)150 , (uint8_t)80 };
	uint8_t test_2[4] = { (uint8_t)1, (uint8_t)0, (uint8_t) 1, (uint8_t)1 };
	/*uint8_t test_3[4] = { (uint8_t)130, (uint8_t)140, (uint8_t)210 , (uint8_t)156 };
	uint8_t test_4[4] = { (uint8_t)40, (uint8_t)80, (uint8_t)100 , (uint8_t)10 };*/

	//test: Polynomial initialization step
	BRLWE_init_hex(&a, test_1, 0);
	BRLWE_init_hex(&m, test_2, 0);
	printf("test1 = ");
	phex(a.polynomial);
	printf("test2 = ");
	phex(m.polynomial);

	//test: Math operation subfunctions
	printf("test1 + test2 = ");
	phex(Ring_add(a, m).polynomial);
	printf("test1 - test2 = ");
	phex(Ring_sub(a, m).polynomial);
	printf("test1 * test2 = ");
	phex(Simple_Ring_mul(a, m).polynomial);

	//test: Key Generation step
	printf("\nKey Generation:\n");
	BRLWE_Key_Gen(&a, &pk, &sk);
	printf("public key = ");
	phex(pk.polynomial);
	printf("secret key = ");
	phex(sk.polynomial);
	
	//test: Encryption step
	printf("\nEncryption:\n");
	printf("a = ");
	phex(a.polynomial);
	printf("public key = ");
	phex(pk.polynomial);
	printf("original message = ");
	phex(m.polynomial);

	struct BRLWE_Ring_polynomials2 c;
	c = BRLWE_Encry(&a, &pk, &m);

	printf("secret message = \n");
	phex(c.c1.polynomial);
	phex(c.c2.polynomial);

	//test: Decryption step
	printf("\nDecryption:\n");
	printf("a = ");
	phex(a.polynomial);
	printf("secret key = ");
	phex(sk.polynomial);
	printf("secret message = \n");
	phex(c.c1.polynomial);
	phex(c.c2.polynomial);

	uint8_t* dm = BRLWE_Decry(&c,&sk);

	printf("decrypted message =");
	phex(dm);

	system("pause");
	return end;
}

static void phex(uint8_t* str)
{
	int i;
	for (i = 0; i < BRLWE_N; ++i)
		printf("%02x", str[i]);//updated, original:printf("%.2x", str[i]);
	printf("\n");
}
