#include <stdio.h>
#include <math.h>
#include <time.h>

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
			x+=1;
			yy=x*x-N;
		}
	}
	y=sqrt(yy);
	*f1=x+y;
	*f2=x-y;
	

}int main(){
    int cF1, cF2; 
//int aF1, aF2;
  int n = 129;
  
    clock_t start_c = clock();
   
 //compute factors

	computeFactors(n,&cF1,&cF2);
    clock_t end_c = clock();
	double elapsed_time_c = (end_c-start_c)/(double)CLOCKS_PER_SEC ;

    printf("Compute Factors from c: F1= %i and F2= %i \n", cF1, cF2);
		printf("Execution time from c = %f \n", elapsed_time_c);
    

	//clock_t start_a = clock();
   
 //compute factors

	//at = computeFactors(n);
 //   clock_t end_a = clock();
	//double elasped_time_assembly = (end_a-start_a)/(double)CLOCKS_PER_SEC ;

    //printf("Compute Factors from assembly = %lu \n", at);

    //printf("Execution time from a = %f \n", elapsed_time_a);
 
    return 0;
}

