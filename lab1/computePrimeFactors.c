#include <stdint.h>

extern uint64_t fermat(uint32_t n);

uint32_t* computePrimeFactors(uint32_t n);
void heapSort(uint32_t a[], uint32_t m);

#define MAX_FACTOR_NUMBER 1000
//an array used to store all the factors
uint32_t _factor[MAX_FACTOR_NUMBER];
//the number of factors
uint32_t _factorNumber = 0;

/**
* Main function of lab1 part 2
*/
void computePrimeFactorsMain()
{
	uint32_t* factors;

	factors = computePrimeFactors(286561800);
	heapSort(factors, _factorNumber);
}



/**
*	get all prime factors of usigned 32bit integer: n
* @param n, unsigned 32-bit interger
* @return the start address of the array of all the prime factors, ends by 0
**/
uint32_t* computePrimeFactors(uint32_t n)
{
	//f, f1, f2 used to get the return value from fermat function
	uint32_t f1, f2, tempN;
	uint64_t f;
	//used to store the intermidate non-prime factors as a stack
	uint32_t stackTop = 0;
	uint32_t iFactorsStack[MAX_FACTOR_NUMBER];

	
	//clear prime factor array
	_factorNumber = 0;
	
	// put n to stack
	iFactorsStack[0] = n;
	stackTop ++;
	//find all prime factors until intermidate non-prime factors stack empty
	while (stackTop > 0)
	{
		stackTop --;
		tempN = iFactorsStack[stackTop];
		f = fermat(tempN);
		f1 = f;  //the lower 32 bits of unsigned 64-bit f
		f2 = f >> 32; //the higher 32 bits of unsigned 64-bit f
		if (f2 != 1)
		{
			//f1 and f2 may not be prime factors
			iFactorsStack[stackTop] = f1;
			stackTop ++;
			iFactorsStack[stackTop] = f2;
			stackTop ++;
		}	else
		{
			//f1 is a prime factors when f2 is 1
			_factor[_factorNumber] = f1;
			_factorNumber ++;
		}
	}
	_factor[_factorNumber] = 0;
	return _factor;
}


//put a[i] in correct position in the heap of length m
//root > left child and root > right child
void percolateDown(uint32_t a[], uint32_t m, uint32_t i)
{
	uint32_t maxj;
	//while node i is not leaf
	while (2 * i + 1 < m)
	{
		maxj = 2 * i + 1;
		if (2 * i + 2 < m)
			maxj = a[2 * i + 2] < a[maxj] ? maxj: 2 * i + 2;
		if (a[i] < a[maxj])
		{
			int temp = a[i];
			a[i] = a[maxj];
			a[maxj] = temp;
			i = maxj;
		}
		else
			break;
	}
}
//create a heap of m element
void buildHeap(uint32_t a[], uint32_t m)
{
	uint32_t k = m / 2 - 1;
	int i;
	for (i = k; i >= 0; i--)
	{
		percolateDown(a, m, i);
	}
}
//sort array a
void heapSort(uint32_t a[], uint32_t m)
{
	int i;
	buildHeap(a, m);
	for (i = m - 1; i > 0; i--)
	{
		int temp = a[0];
		a[0] = a[i];
		a[i] = temp;
		percolateDown(a, i, 0);
	}
}