#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h> 

int main(int argc, char* argv[argc+1]) {
	double a;
	long int b;
	long int c=0;
	long int d=-64;
	FILE* f1=fopen(argv[1], "r");
	while(fscanf(f1, "%lf %lu\n", &a, &b) !=EOF){
		printf("1.");
		while(a>=pow(2,d)){
			d++;
		}
		d--;
		a=a-pow(2,d);
		long int e=d;
		d--;
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
		printf(" ");
		c=e;
		printf("%li", c);
		printf("\n");
		c=0;
		d=-64;
	}
	fclose(f1);
	return 0;
}
