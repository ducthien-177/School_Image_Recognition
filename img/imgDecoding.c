//
//  4J41 LE DUC THIEN
//



#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define N 64

void getNextFileOf(char *filePath);
void render(char* filePath, char dest[N][N]);
void display(char dest[N][N]);
void outline(char img[64][64]);
void normalize(char img[64][64]);

int main(void){
	char img[64][64], out[64][64];
	char input,  filePath[256] = ".\\\\img\\\\05.img";

	printf("\nEnter your data directory to star:  ");
	printf("\nSample:  %s \n ", filePath);
	scanf("%s", filePath);
	render(filePath, img);

	display(img);
	normalize(img);

/*
	while(1){
		printf("\nPress ENTER to countinue or SPACE to quite");
		input = getch();
	
		if(input == 13 ){
			getNextFileOf(filePath);
			render(filePath);
		}
		if(input == ' '){
			return 0;
		}	
	}	
*/		

}
// render file to a 2D array
void render(char* filePath, char dest[N][N]){
	unsigned char d[512], mask;
	int i, x, y, z;                     // loop counter
	i=x=y=z=0;							
	
	FILE *file;	
	file = fopen(filePath, "rb");
	
	if(file == NULL){
		printf("\nfile can not open\n");
		exit(1);
	}else{
		fread(d, 512, 1, file);
		printf("\nread file done\n");
	}
	for( y=0; y<64; y++){					// print 64 rows
		for( x=0; x<8; x++){				// print 8 bytes = 64 bits
		mask = 0x80;
			for( i=0; i<8; i++){			// print 8 bits
				if( d[y*8+x] & mask ) {
				//	printf("*");
					dest[y][z] = 1;
				
				}
				else{
				//	printf("o");	
					dest[y][z] = 0;
				
				} 
				z++;
				mask = mask >> 1;
			}	
		}
		z=0;					// reset z when z go to 63
	//	printf("\n");	
	}

	fclose(file);
}

void display(char dest[N][N]){
	printf("\n");
	int x,y;
	for(y=0; y<64; y++){
		for(x=0; x<64; x++){
			printf("%c ", dest[y][x]);
		}
		printf("\n");
	}
}

void outline(char img[64][64]){
	int i,x,y;	x=y=0;
	
	for(y=1; y<63; y++){
		for(x=1; x<63; x++){
			if(img[y][x] == 1){	// check interior pixels or boundary pixels
				if( (img[y][x] == img[y][x+1] || img[y][x+1] == 2) &&	
					(img[y][x] == img[y][x-1] || img[y][x-1] == 2) &&	
					(img[y][x] == img[y+1][x] || img[y+1][x] == 2) &&		
					(img[y][x] == img[y-1][x] || img[y-1][x] == 2)   ){   
					
					img[y][x] = 2;	// interior pixels will be ' '
				}  
			}
		
		}
	}
	for(y=0; y<64; y++){
		for(x=0; x<64; x++){	// change value of all interior pixels
			if(img[y][x] == 2)  img[y][x] = 0;
		}
	}

}

void normalize(char img[64][64]){
	int x0, y0, w, h, i, j, k, u, v;
	int rightEdge, leftEdge, topEdge, botEdge;
	leftEdge = topEdge = rightEdge = botEdge = 64 ;
	 
	for(i=0; i<64; i++){							//   caclculate padding
		for(j=0; j<64; j++){
			if(img[i][j] == 1){
				if( i < topEdge  )  	topEdge  = i;
				if( j < leftEdge )  	leftEdge = j;
				if( 63-j < rightEdge)  	rightEdge = 63-j;
				if( 63-i < botEdge  )  	botEdge   = 63-i;
			}	 
		}
	}
	x0 = leftEdge;
	y0 = topEdge;
	w  = 64 - leftEdge - rightEdge;
	h  = 64 - topEdge - botEdge;
	
	printf("\ntop w left h = %d %d %d %d\n", topEdge, w, leftEdge, h);
	char out[64][64];
	for(v=0; v<64; v++){
		for(u=0; u<64; u++){
			
			j =  u*(w/64.0) + x0 + 0.5;				//  refer to pixel of original img[][]
			i =  v*(h/64.0) + y0 + 0.5;				//	formular to zoom image
	
			out[v][u] = img[i][j];
		}
	}
	for(v=0; v<64; v++){							//  re-render img[][] from out[][]
		for(u=0; u<64; u++){
			img[v][u] = out[v][u];
		}
	}
	display(img);
}

void getNextFileOf(char *filePath){
		
	char name[3];
	int length, nameInt;
	
	length = strlen(filePath);
	strncpy(name, filePath + (length-6), 2);

	nameInt = atoi(name); itoa(nameInt+1, name, 10);
	if (nameInt < 9) {
		memcpy(filePath + (length-5), name, 1);
		printf("\n next file = %s \n", filePath) ; 
	}else{
		memcpy(filePath + (length-6), name, 2);
		printf("\n next file = %s \n", filePath) ; 
	}
}

void noise(char img[64][64]){
	int x,y,label;
	label = 2;
	
	for(y=1; y<63; y++){		// not check at border of image 
		for(x=1; x<63; x++){
			if( 
				img[y][x]==1 &&
			   (img[y-1][x] > 1 	||	// top
				img[y][x-1]	> 1		||	// left
				img[y-1][x-1] > 1	||	// top right
				img[y-1][x+1] > 1)	// top left
				){
				img[y][x]=
			}
		}
	}
}

/*sample of output

oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
oooooooooooooooooooooooooooooo******oooooooooooooooooooooooooooo
ooooooooooooooooooooooooooo**********ooooooooooooooooooooooooooo
oooooooooooooooooooooo**************oooooooooooooooooooooooooooo
oooooooooooooooooooo****************oooooooooooooooooooooooooooo
oooooooooooooooooooo***************ooooooooooooooooooooooooooooo
ooooooooooooooooooooo*****ooo*****oooooooooooooooooooooooooooooo
ooooooooooooooooooooooooooooo****ooooooooooooooooooooooooooooooo
oooooooooooooooooooooooooooo****oooooooooooooooooooooooooooooooo
ooooooooooooooooooooooooooo*****oooooooooooooooooooooooooooooooo
ooooooooooooooooooooooooooo****ooooooooooooooooooooooooooooooooo
oooooooooooooooooooooooooo****ooooooooo****ooooooooooooooooooooo
ooooooooooooooooooooooooo*****ooooooo******ooooooooooooooooooooo
ooooooooooooooooooooooooo****ooooooo*******ooooooooooooooooooooo
oooooooooooooooooooooooo*****ooooo*********ooooooooooooooooooooo
ooooooooooooooooooooooo*****oooo**********oooooooooooooooooooooo
oooooooooooooooooooooo*****ooo***********ooooooooooooooooooooooo
oooooooooooooooooooooo****ooo************ooooooooooooooooooooooo
ooooooooooooooooooooo*************oo****oooooooooooooooooooooooo
oooooooooooooooooooo***********ooooo****oooooooooooooooooooooooo
ooooooooooooooooooo***********oooooo****oooooooooooooooooooooooo
ooooooooooooooooooo*********ooooooo****ooooooooooooooooooooooooo
ooooooooooooooooo*********ooooooooo****ooooooooooooooooooooooooo
ooooooooooooooooo*******ooooooooooo***oooooooooooooooooooooooooo
ooooooooooooooooo*****ooooooooooooo***oooooooooooooooooooooooooo
oooooooooooooooooo**oooooooooooooo****oooooooooooooooooooooooooo
oooooooooooooooooooooooooooooooooo****oooooooooooooooooooooooooo
oooooooooooooooooooooooooooooooooo****oooooooooooooooooooooooooo
oooooooooooooooooooooooooooooooooo****oooooooooooooooooooooooooo
oooooooooooooooooooooooooooooooooo***ooooooooooooooooooooooooooo
oooooooooooooooooooooooooooooooooo***ooooooooooooooooooooooooooo
oooooooooooooooooooooooooooooooooo***ooooooooooooooooooooooooooo
oooooooooooooooooooooooooooooooooo****oooooooooooooooooooooooooo
oooooooooooooooooooooooooooooooooo****oooooooooooooooooooooooooo
oooooooooooooooooooooooooooooooooo****oooooooooooooooooooooooooo
oooooooooooooooooooooooooooooooooo****oooooooooooooooooooooooooo
oooooooooooooooooooooooooooooooooo****oooooooooooooooooooooooooo
ooooooooooooooooooooooooooooooooooo***oooooooooooooooooooooooooo
ooooooooooooooooooooooooooooooooooo****ooooooooooooooooooooooooo
ooooooooooooooooooooooooooooooooooo****ooooooooooooooooooooooooo
ooooooooooooooooooooooooooooooooooo****ooooooooooooooooooooooooo
ooooooooooooooooooooooooooooooooooo****ooooooooooooooooooooooooo
oooooooooooooooooooooooooooooooooooo****oooooooooooooooooooooooo
oooooooooooooooooooooooooooooooooooo*****ooooooooooooooooooooooo
ooooooooooooooooooooooooooooooooooooo*****oooooooooooooooooooooo
oooooooooooooooooooooooooooooooooooooo*****ooooooooooooooooooooo
ooooooooooooooooooooooooooooooooooooooo******ooooooooooooooooooo
oooooooooooooooooooooooooooooooooooooooo******oooooooooooooooooo
oooooooooooooooooooooooooooooooooooooooooo****oooooooooooooooooo
oooooooooooooooooooooooooooooooooooooooooooo*ooooooooooooooooooo
oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
*/

