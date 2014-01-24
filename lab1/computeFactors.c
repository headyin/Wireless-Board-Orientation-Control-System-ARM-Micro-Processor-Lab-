#include <stdio.h>
#include <iostream.h>
#include <math.h>
using namespace std;

int checkSquare(int n){
    int temp;
	temp = sqrt(n);
    if(temp*temp == n) return 1;
    else return 0;
}

void computeFactors(int N){
	int number, f1, f2;
	int x,y,xx,yy;

	if (N<=0)
	{
		f1=0;
		f2=0;
		return;
	}
	else if (N%2==0){
		f1=2;
		f2=N/2;
		return;
	}else{
		x=ceil(math.sqrt(N));
		yy = x*x - N;
		while (checkSquare(yy)==0)  {		
			x=x+1;
			yy=x*x-N;
		}
	}
	y=math.sqrt(yy);
	f1=x+y;
	f2=x-y;
	return;

}

