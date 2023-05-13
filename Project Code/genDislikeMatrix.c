#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(int argc,char *argv[]){

	int random;
	int size = atoi(argv[1]);
        
	for(int i=0;i<size;i++){
		for(int j=0;j<size;j++){
			srand(time(NULL));
			random = rand() % 11;
			fprintf(fp,"%d ",random);
}
