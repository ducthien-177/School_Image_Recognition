//
//	read file in c
//
 #include<stdio.h>
 #include<stdlib.h>
 
 int main(void){
 	
 	int i,j;  i=j=0;
 	double numArr[2];
 	FILE *file;
 	
 	
 	file =  fopen("data.txt", "r");
	
	if(file == NULL){
		printf("Err\n");
		exit(EXIT_FAILURE);
	}  
	
	for(j=0; j<5; j++){
		for(i=0; i<2; i++){
			fscanf(file, "%lf", &numArr[i]);	
		}	
		printf("%f %f \n", numArr[0], numArr[1]);
	} 
	
	fclose(file);
	
 	return 0;	
 }
 
 	
 