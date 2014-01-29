#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stm32f4xx.h>



int gcd(int a, int b)
{
	while(a!=b){
		if(a>b){
			a -=b;
		}else{
			b -=a;
		}
	}
	return a;
}

//Since GCD(a,b)LCM(a,b)=a*b
int lcm(int a, int b)
{
		return (a*b/gcd(a,b));
}
int testmain(){
	
	int a =12;
	int b = 13;
	int c,d;
	c=gcd(a,b);
	d=lcm(a,b);
	
	printf("GCD = %i LCM= %i \n", c, d);
	return 0;
}