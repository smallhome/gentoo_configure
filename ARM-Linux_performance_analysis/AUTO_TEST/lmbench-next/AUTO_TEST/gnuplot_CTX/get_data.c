#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	FILE *fp_read;
	FILE *fp_write;
	char* extension = "k.txt";
	int i = 0;
	for (i = 0; i < 6; i++) {
		/*
		 * read from file(open)
		 */
	    int file_digit = 2;//more than 9 is 2 digit(such as 16 or 24 or 32)
		char file_number[file_digit+1];
		int f_num = switch_file(i);
		snprintf(file_number,sizeof(char[file_digit+1]), "%d", f_num);//make it to be string(char array)
		char* file = strcat(file_number , extension);
		fp_read = fopen(file, "r");
		/*
		 * write to file(open)
		 */
		char * f_result = (char *)malloc(sizeof(char)*20);//get enough buffer
		strcpy(f_result,"result_");
		strcat(f_result,file);
		fp_write = fopen(f_result, "w");

		if ((fp_read == NULL) || (fp_write == NULL)) { //if file open incorrectly
			printf("Open file Error!\n");
			exit(-1);
		} else {
			int line = 1;
			char buf[60];
			while (fgets(buf, sizeof(buf), fp_read) != NULL) {
				if (line % 3 == 0) {
					//fwrite(buf, sizeof(char), 60, fp_write);
					fprintf(fp_write,"%s",buf);
				}
				line++;
			}
		}
		fclose(fp_read);
		fclose(fp_write);
		free(f_result);
		//free(file_number);
	}
}

int switch_file(int num)
{
	switch (num) {
	case 0:
		return 2;
		break;
	case 1:
		return 4;
		break;
	case 2:
		return 8;
		break;
	case 3:
		return 16;
		break;
	case 4:
		return 24;
		break;
	case 5:
		return 32;
		break;
	default:
		return 0;
	}
}
