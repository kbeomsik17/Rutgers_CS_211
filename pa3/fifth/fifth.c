#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
void prePrintBit(long int a, long int b){
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

void printBit(long int a){
	if(a>1){
		printBit(a>>1);
	}
	long int g=a&1;
	printf("%lu", g);
	return;
} 

int main(int argc, char* argv[argc+1]) {
	double a;
	long int b;
	long int d=-64;
	long int f;
	long int g;
	FILE* f1=fopen(argv[1], "r");
	while(fscanf(f1, "%lf %lu %lu %lu\n", &a, &f, &g, &b) !=EOF){
		if(a<0){
			printf("1");
			a=-a;
		}
		else{
			printf("0");
		}
		while(a>=pow(2,d)){
			d++;
		}
		double temptempa=a;
		long int temptempd=d-1;
		temptempa=temptempa-pow(2,temptempd);
		long int temptempb=b+1;
		while(temptempb>0){
			if(temptempa>=pow(2,temptempd)){
				temptempa=temptempa-pow(2,temptempd);
			}
			temptempd--;
			temptempb--;
		}
		if(pow(2,d)-a==pow(2,d-g-1) && temptempa!=0){
			a=a+pow(2,d-g-1);
			d++;
		}
		d--;
		a=a-pow(2,d);
		long int e=d;
		long int h=e+pow(2,g-1)-1;
		prePrintBit(h, g);
		printBit(h);
		d--;
		double tempa=a;
		long int tempd=d;
		long int tempb=b;
		int even=0;
		while(tempb>0){
			if(tempa>=pow(2,tempd)){
				tempa=tempa-pow(2,tempd);
				if(tempb==1){
					even=1;
				}
			}
			tempd--;
			tempb--;
		}
		if(pow(2,tempd+1)-tempa==tempa && even==1){
			a=a+tempa;
		}
		else if(pow(2,tempd+1)-tempa<tempa){
			a=a+pow(2,tempd+1)-tempa;
		}
		while(b>0){
			if(a>=pow(2,d)){
				a=a-pow(2,d);
				printf("1");
			}
			else{
				printf("0");
			}
			d--;
			b--;
		}
		printf("\n");
		d=-64;
		even=0;
	}
	fclose(f1);
	return 0;
}
