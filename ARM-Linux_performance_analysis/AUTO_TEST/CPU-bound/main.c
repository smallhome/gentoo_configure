#include <stdio.h>
#include <stdlib.h>

int main(){
	int A[1000][1000] = {1};
	int B[1000][1000] = {1};

	int i,j,k,sum;
	for(i = 0 ; i < 1000 ; i++){
		for(j = 0 ;j < 1000;j++){
			sum = 0;
			for(k = 0 ; k < 1000; k++){
				sum+=A[i][k]*B[k][j];
			}
		}
	}

	return 0;
}
