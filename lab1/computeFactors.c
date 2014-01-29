#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdint.h>
// Prototype of assembly function:
#include <stm32f4xx.h>
extern uint64_t Fermat(uint32_t N);
// N is passed in R0, and the return value will be read from R0--R1.

int checkSquare(int n){
    int temp;
		temp = sqrt(n);
    if(temp*temp == n) return 1;
    else return 0;
}

void computeFactors(int N, int *f1, int *f2){
	int x,y,yy;

	if (N<=0)
	{
		*f1=0;
		*f2=0;
		return;
	
	}else if (N%2==0){
		*f1=2;
		*f2=N/2;
		return;
		
	}else{
		x=ceil(sqrt(N));
		yy = x*x - N;
		while (checkSquare(yy)==0)  {		
			x=x+1;
			yy=x*x-N;
		}
	}
	y=sqrt(yy);
	*f1=x+y;
	*f2=x-y;
	

}
int main(){
    int cF1, cF2; 
	uint64_t a;
	uint32_t least, most;
  uint32_t n = 12345; 
 //compute factors

		computeFactors(n,&cF1,&cF2);
    printf("Compute Factors from c: F1= %i and F2= %i \n", cF1, cF2);
	a = Fermat(n);
	least = a;
most = (a & 0xFFFFFFFF) >> 32;

   printf("Compute Factors from assembly: F1= %i and F2= %i \n", least, most );
    return 0;
}

