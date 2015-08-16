#include <stdio.h>
#include <stdlib.h>

int main(int argc,char* argv[]){
	FILE *fptr_r;
	FILE *fptr_w;
	char *s;
	int i;
	int x = 1;
	int y = 2;
	s = (char*)malloc(10000*sizeof(char));
	fptr_r = fopen("dblp.xml","rb");
	fptr_w = fopen(argv[1],"w");
	while(!feof(fptr_r)){
		fread(s,1,10000,fptr_r);
		fprintf(fptr_w,"%d,%d",x,y);
	}
	fclose(fptr_r);
	fclose(fptr_w);
	return 0;
}
