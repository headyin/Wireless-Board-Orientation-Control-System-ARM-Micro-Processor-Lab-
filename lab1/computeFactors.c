#include <stdio.h>
#include <math.h>
#include <stdint.h>

// Prototype of assembly function:
#include <stm32f4xx.h>
extern uint64_t fermat(uint32_t N);
// N is passed in R0, and the return value will be read from R0--R1.


//Check if number is a square
int checkSquare(int n){
	int temp;
	//Take the square root the number
	temp = sqrt(n);
	//If the squaring the square root returns the original number
	//then return true, false otherwise
	if(temp*temp == n) return 1;
	else return 0;
}

//Compute Factors algorithm
void computeFactors(uint32_t N, uint32_t *f1, uint32_t *f2){
	int x,y,yy;

	//If N is zero, then the factors will also be zero
	if (N<=0)
	{
		*f1=0;
		*f2=0;
		return;
	}
	//If N is even, then we know that one factor can be 2
	//and the other number is just the half of N
	else if (N%2==0){
		*f1=2;
		*f2=N/2;
		return;

	}
	//If N is odd, use Fermat's Factorization Algorithm
	else{
		x=ceil(sqrt(N));
		yy = x*x - N;
		while (checkSquare(yy)==0)  {		
			x=x+1;
			yy=x*x-N;
		}
		y=sqrt(yy);
		*f1=x+y;
		*f2=x-y;
	}



}
int computeFactorsMain(){

	//Define variables
	uint32_t cF1, cF2; 
	uint64_t a;
	uint32_t least;
	uint32_t most;
	uint32_t n = 0x7FFFFF; 

	//Compute factors from c and assembly code
	computeFactors(n,&cF1,&cF2);
	printf("Compute Factors from c: F1= %i and F2= %i \n", cF1, cF2);


	a = fermat(n);
	//
	least = a;
	most = a >> 32;

	printf("Compute Factors from assembly: F1= %i and F2= %i \n", least, most );
	return 0;
}

