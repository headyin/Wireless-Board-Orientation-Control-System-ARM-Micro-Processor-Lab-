#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stm32f4xx.h>

//Greatest Common Divisor algorithm
//Following Euclidean algorithm from wikipedia
uint32_t gcd(uint32_t a, uint32_t b)
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

//Least Common Multiple algorithm
uint32_t lcm(uint32_t a, uint32_t b)
{
	//Since GCD(a,b)LCM(a,b)=a*b
	return (a*b/gcd(a,b));
}

int gcdlcmMain(){

	uint32_t a =15;
	uint32_t b = 35;
	uint32_t c,d;
	c=gcd(a,b);
	d=lcm(a,b);

	printf("GCD = %i LCM= %i \n", c, d);
	return 0;
}