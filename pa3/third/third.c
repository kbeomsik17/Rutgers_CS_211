#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h> 

int main(int argc, char* argv[argc+1]) {
	long int a;
	long int b;
	long int c;
	char a1;
	char a2;
	char a3='u';
	char a4='s';
	FILE* f1=fopen(argv[1], "r");
	while(fscanf(f1, "%li %li %c %c\n", &a, &b, &a1, &a2) !=EOF){
		if(a1==a3 && pow(2,b-1)-1>=a){
			printf("%li", a);
		}
		else if(a1==a4 && pow(2,b-1)-1>=a && a>=0){
			printf("%li", a);
		}
		else{
			c=pow(2,b)-abs(a);
			if(a>=0){
				printf("%li", -c);
			}
			else{
				printf("%li", c);
			}
		}
		printf("\n");
	}
	fclose(f1);
	return 0;
}
