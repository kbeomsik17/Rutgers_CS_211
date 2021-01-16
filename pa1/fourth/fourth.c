#include <stdlib.h>
#include <stdio.h>

void printMatrix(int** mat, int m){
	for(int i=0; i<m; i++){
		for(int j=0; j<m; j++){
			printf("%d	", mat[i][j]);
		}
	printf("\n");
	}
}

void freeMatrix(int** mat, int m){
	for(int i=0; i<m; i++){
		free(mat[i]);
	}
	free(mat);
}

int main(int argc, char* argv[argc+1]) {
	int m=atoi(argv[1]);
	int firsthori=(m/2);
	int firstverti=0;
	if(m%2==0){
		printf("error\n");
		return 0;
	}
	int **mat=malloc(m*sizeof(int*));
	for(int i=0; i<m; i++){
		mat[i]=malloc(m*sizeof(int));
		for(int j=0; j<m; j++){
			mat[i][j]=-1;
		}
	}
	mat[firstverti][firsthori]=1;
	for(int i=2; i<=m*m; i++){
		firstverti--;
		firsthori++;
		if(firstverti<0){
			firstverti=firstverti+m;}
		if(firsthori>m-1){
			firsthori=firsthori-m;}
		if(mat[firstverti][firsthori]==-1){
			mat[firstverti][firsthori]=i;
		}
		else{
			firstverti++;
			firstverti++;
			firsthori--;
			if(firstverti<0){
				firstverti=firstverti+m;}
			if(firsthori>m-1){
				firsthori=firsthori-m;}
			if(firstverti>m-1){
				firstverti=firstverti-m;}
			if(firsthori<0){
				firsthori=firsthori+m;}
			mat[firstverti][firsthori]=i;
		}
	}
	printMatrix(mat, m);
	freeMatrix(mat, m);
	return 0;
}
