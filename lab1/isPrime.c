#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

extern uint32_t roundSqrt(uint32_t i);
extern uint64_t Fermat(uint32_t n);

uint8_t isPrime(uint32_t i)
{
		uint32_t j,k;
		k = roundSqrt(i);
		flag = 1;
		for (j = k; j > 1; j--)
		{
			if (i % j == 0)
			{
				flag = 0;
				break;
			}
		}
		return flag;
}


void getPrimeFactors()
{
	uint32_t n, f1, f2;
	uint32_t a[10000];
	uint64_t f;
	n = 12345;
	f = Fermat(n);
	f1 = f;
	f2 = f >> 32;
	n = f1 + f2;
}

void getAllPrimes()
{
	uint32_t n = 100; //8388608; // 2^23
	uint32_t i, number = 0;
	uint8_t flag;
	for (i = n; i > 1; i--)
	{
		uint32_t j,k;
		k = roundSqrt(i);
		flag = 1;
		for (j = k; j > 1; j--)
		{
			if (i % j == 0)
			{
				flag = 0;
				break;
			}
		}
		if (flag)
		{
			printf("Prime %d\n", i);
			number ++;
		}
	}
	printf("Total number is %d\n", number);
}