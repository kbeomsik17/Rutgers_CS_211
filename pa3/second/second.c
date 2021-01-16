#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h> 
void prePrintBit(long int a, long int b, long int z){
	double c=pow(2.0, b-2);
	if(a==0){
		for(long int i=1; i<b; i++){
			printf("0");
		}
		return;
	}
	if(a<0){
		while((abs(z)/c)<1){
			printf("1");
			c=c/2;
		}
		return;
	}
	while(a/c<1){
		printf("0");
		c=c/2;
	}
	return;
}

void printBit(long int a){
	a=abs(a);
	if(a>1){
		printBit(a>>1);
	}
	long int g=a&1;
	printf("%li", g);
	return;
}

int main(int argc, char* argv[argc+1]) {
	long int a;
	long int b;
	long int c=1;
	long int d=0;
	long int z;
	FILE* f1=fopen(argv[1], "r");
	while(fscanf(f1, "%li %li\n", &a, &b) !=EOF){
		z=a;
		if(pow(2, b-1)-1<a){
			printf("0");
			a=pow(2, b-1)-1;
			prePrintBit(a,b,z);
			printBit(a);
		}
		else if(-(pow(2, b-1))>a){
			a=-(pow(2, b-1));
			prePrintBit(a,b,z);
			printBit(a);
		}
		else{
		if(a<0){
			c=-1;
			d=0;
			while(c>=a){
				c=c*2;
				d++;
			}
		}
		if(a<0){
			a=-((pow(2, d))+a);
		}
		if(a<0){
			printf("1");
		}
		else{
			printf("0");
		}
		prePrintBit(a,b,z);
		if(a<0){
			while((-a)<pow(2,d-1)){
				printf("0");
				d--;
			}
		}
		if(a!=0){
			printBit(a);
		}
		}
		printf("\n");
		d=0;
		c=1;
	}
	fclose(f1);
	return 0;
}
