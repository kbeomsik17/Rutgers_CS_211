#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(int argc, char* argv[argc+1]) {
	long int a;
	long int b;
	int c;
	long int d;
	int e;
	FILE* f1=fopen(argv[1], "r");
	while(fscanf(f1, "%li %li %d %lx %d\n", &a, &b, &c, &d, &e) !=EOF){
		long int f=0;
		long int tempa=a;
		double one=1;
		double final=0;
		//long int tempb=b;
		long int h=0;
		long int nuu=0;
		//long double g=0;
		if(d>pow(2,a)-1){
			while(d>pow(2,a)-1){
				a++;
				nuu++;
			}
			a=tempa;
			for(int i=4-(a%4)+nuu; i>0; i--){
				if(d>=pow(2,a+i-1)){
					d=d-pow(2,a+i-1);
				}
			}
		}
		if(d>=pow(2,a-1) && d-pow(2,a-1)<pow(2,a-1-b)){
			h=1;
		}
		if(d<pow(2,a-1-b)){
			h=1;
		}
		if(d>=pow(2,a-1)){
			printf("-");
			d=d-pow(2,a-1);
		}
		long tempd=d;
		a--;
		a--;
		while(b>0){
			if(d>=pow(2,a)){
				d=d-pow(2,a);
				f=f+pow(2,a-c);
			}
			b--;
			a--;
		}
		if(h==1){
			f=1-(pow(2,tempa-c-2)-1);
		}
		else{
			if(f<pow(2,tempa-c-2)-1){
				f=-(pow(2,tempa-c-2)-1-f);
			}
			else{
				f=f-(pow(2,tempa-c-2)-1);
			}
		}
		/*if(h==1){
			g=0;
		}
		else{
			g=g+pow(2,f);
		}
		printf("  %li  ",f);
		f--;
		printf("  %li  ",d);
		while(c>0){
			if(d>=pow(2,c-1)){
				g=g+pow(2,f);
				d=d-pow(2,c-1);
			}
			f--;
			c--;
		}
		printf("  %Lf  ",g);
		printf("%.0lf",floor(g));
		printf(".");
		g=g-floor(g);
		long int count=0;
		long int value;
		while(g>floor(g)){
			g=g*10;
			count++;
		}
		count--;
		while(e>0){
			value=g/pow(10,count);
			g=g-value*pow(10,count);
			printf("%li",value);
			e--;
			count--;
		}*/
		if(h==0){
			final++;
		}
		for(int i=c-1; i>=0; i--){
			one=one/2;
			long temp=pow(2, c)-1;
			long temp2=(tempd & temp)>>i;
			if((temp2) & 1){
				final=final+one;
			}
		}
		final*=pow(2, f);
		printf("%.*lf", e, final);
		printf("\n");
	}
	fclose(f1);
	return 0;
}
