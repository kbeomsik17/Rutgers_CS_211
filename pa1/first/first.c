#include <stdlib.h>
#include <stdio.h>

int isItPrime(int n){
	int i=0;
	for(i=2; i<=n/2; i++){
		if(n%i==0){
			return 0;
		}
	}
	return 1;
}

int main(int argc, char* argv[argc+1]) {
	FILE* f=fopen(argv[1], "r");
	int a=0;
	while(fscanf(f, "%d", &a) !=EOF){
		if(a<3){
			printf("no\n");
		}
		else if(isItPrime(a)==1){
			if(isItPrime(a+2)==1 || isItPrime(a-2)==1){
				printf("yes\n");}
			else{
				printf("no\n");}
		}
		else{
			printf("no\n");
		}
	}
	fclose(f);
	return 0;
}
