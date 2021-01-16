#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void freeMatrix(int** mat, int m){
	for(int i=0; i<m; i++){
		free(mat[i]);
	}
	free(mat);
}

void printMatrix(int** mat, int m){
	for(int i=0; i<m; i++){
		for(int j=0; j<m; j++){
			printf("%d	", mat[i][j]);
		}
	printf("\n");
	}
}

int** craftSubMatrix(int** mat, int n, int sub){
	int **mat2=malloc((n-1)*sizeof(int*));
	for(int i=0; i<(n-1); i++){
		mat2[i]=malloc((n-1)*sizeof(int));
		for(int j=0; j<(n-1); j++){
			if(j>=sub){
				mat2[i][j]=mat[i+1][j+1];
			}
			else{
				mat2[i][j]=mat[i+1][j];
			}
		}
	}
	return mat2;
}

int calculateDeterminant(int** mat, int m){
	if(m==1){
		return mat[0][0];
	}
	else if(m==2){
		return mat[0][0]*mat[1][1]-mat[0][1]*mat[1][0];
	}
	else{
		int s=-1;
		int s2=-1;
		int total=0;
		for(int i=0; i<m; i++){
			s=s*s2;
			int** submat=craftSubMatrix(mat, m, i);
			total=total+(s*calculateDeterminant(submat, m-1)*mat[0][i]);
			freeMatrix(submat, m-1);
		}
		return total;
	}
}

int main(int argc, char* argv[argc+1]) {
	int n=0;
	int temp=0;
	FILE* f=fopen(argv[1], "r");
	fscanf(f, "%d", &n);
	int **mat=malloc(n*sizeof(int*));
	for(int i=0; i<n; i++){
		mat[i]=malloc(n*sizeof(int));
		for(int j=0; j<n; j++){
			fscanf(f, "%d\t", &temp);
			mat[i][j]=temp;
		}
	}
	int determinant=calculateDeterminant(mat, n);
	printf("%d\n", determinant);
	fclose(f);
	freeMatrix(mat, n);
	return 0;
}
