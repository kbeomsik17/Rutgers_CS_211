#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h> 
void prePrintBit(unsigned long int a, unsigned long int b){
	double c=pow(2.0, b-1);
	if(a==0){
		for(int i=1; i<b; i++){
			printf("0");
		}
		return;
	}
	while(a/c<1){
		printf("0");
		c=c/2;
	}
	return;
}

void printBit(unsigned long int a){
	if(a>1){
		printBit(a>>1);
	}
	unsigned long int g=a&1;
	printf("%lu", g);
	return;
}

int main(int argc, char* argv[argc+1]) {
	unsigned long int a;
	unsigned long int b;
	FILE* f1=fopen(argv[1], "r");
	while(fscanf(f1, "%lu %lu\n", &a, &b) !=EOF){
		if(pow(2, b)-1>=a){
			prePrintBit(a,b);
			printBit(a);
		}
		else{
			unsigned long int t=pow(2, b);
			a=a%t;
			prePrintBit(a,b);
			printBit(a);
		}
		printf("\n");
	}
	fclose(f1);
	return 0;
}
