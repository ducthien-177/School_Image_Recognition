/*
 *	画像認識工学　課題７
 *	情報工学科　４年41番　LE DUC THIEN
 */
#include <stdio.h>
#include <stdbool.h>

#define	IMGSIZE		512	/* １文字分の文字画像サイズ */
#define	IMGX		64	/* １文字分の画像の横のピクセル数 */
#define	IMGY		64	/* １文字分の画像の縦のピクセル数 */
#define	NOISESIZE	5	/* ノイズとみなす黒画素の塊の閾値 */
#define	SEGS		7	/* 特徴量抽出のための縦・横の画像分割数 */

/* 方向線素特徴量の型定義 */
typedef struct _FTR {
	short int	v ;		/* 縦線素数 */
	short int	h ;		/* 横線素数 */
	short int	ur ;	/* 右上がり線素数 */
	short int	dr ;	/* 右下がり線素数 */
} FTR ;




/*
 *		64x64ドットの文字画像を画面に表示する関数
 */
void PrintPtn( char img[IMGY][IMGX] ) {
	/*
	 *	二次元配列imgに格納されている文字データを表示する。
	 *	白画素を「o」、黒画素を「*」で表示すること。
	 */
	printf("\n");
	int x,y;
	for(y=0; y<64; y++) {
		for(x=0; x<64; x++) {
			if(img[y][x] >= 1) printf("* ");
			else			   printf("o ");
		}
		printf("\n");
	}

}

/*
 *		512バイトの文字画像を2次元配列に格納する関数
 */
void ExpandImg( unsigned char data[IMGSIZE], char img[IMGY][IMGX] ) {
	/*
	 *	一次元配列dataに格納された文字データを、二次元配列imgに展開する。
	 *	img[y][x]は、画像の左上を減点とし、下方向にyドット、右方向にxドットの
	 *	場所の画素値を表すものとする。黒画素であれば1、白画素であれば0とする。
	 *	文字の"0","1"ではなく数値の0,1を入れること。
	 *	課題２のプログラムが参考になる。
	 */

	unsigned char mask;
	int i, x, y, z;                     // loop counter
	i=x=y=z=0;

	for( y=0; y<64; y++) {					// print 64 rows
		for( x=0; x<8; x++) {				// print 8 bytes = 64 bits
			mask = 0x80;
			for( i=0; i<8; i++) {			// print 8 bits
				if( data[y*8+x] & mask ) {
					//	printf("*");
					img[y][z] = 1;

				} else {
					//	printf("o");
					img[y][z] = 0;

				}
				z++;
				mask = mask >> 1;
			}
		}
		z=0;					// reset z when z go to 63
		//	printf("\n");
	}
}

/*
 *		ノイズを除去する関数
 */
 

void display(char dest[IMGY][IMGX]){
	printf("\n");
	int x,y;
	for(y=0; y<64; y++){
		for(x=0; x<64; x++){
			printf("%2d", dest[y][x]);
		}
		printf("\n");
	}
}

int hasLabel(char img[IMGY][IMGX], int y, int x){
	int i, j;
	bool stop = false;
	for(i=y; i<IMGY && !stop; i++){
		for(j=x; j<IMGX && !stop; j++){
			if(img[i][j] == 1 && (img[i-1][j] > 1 )) {
				return img[i-1][j];
			}
			if(img[i][j] == 0) stop = true;		
		}
	}
	return 0;
}

void relabel( char img[IMGY][IMGX], int oldLabel, int y, int x){
	int newLabel = img[y][x];
	int i,j;
	for(i=0; i<y; i++){
		for(j=0; j<IMGX; j++){
			if(img[i][j] == oldLabel)	img[i][j] = newLabel;
		}
	}
}

void label(char img[IMGY][IMGX]){
	int y, x, oldLabel, label = 1;
	bool stop = false;
	for(y=0; y<IMGY && !stop; y++){
		for(x=0; x<IMGX && !stop; x++){
			if(img[y][x] == 1){
				// when above and behind have not labels ->  add new label
				if(img[y-1][x] == 0 && img[y][x-1] == 0){
					 
					if(hasLabel(img, y, x)){		
						img[y][x] =  hasLabel(img, y, x);
					}else{
						label++;	img[y][x] = label;
					} 					
				}
				// when above or behind has a label -> use this label
				if( (img[y][x-1] > 1 || img[y-1][x] > 1) ){
					img[y][x] = (img[y-1][x] > 1) ? img[y-1][x] : img[y][x-1];
				// when above and behind have labels	
				if( img[y-1][x] > 1 && img[y][x-1] > 1 && (img[y-1][x] != img[y][x-1])){
					img[y][x] = (img[y-1][x] > img[y][x-1]) ? img[y][x-1] : img[y-1][x];
					oldLabel = (img[y-1][x] > img[y][x-1]) ? img[y-1][x] : img[y][x-1];
					relabel(img, oldLabel, y, x);	
				}
				}
			}			
		}
	}
}

void EraseByLabel(char img[IMGY][IMGX], int label, int size){
	int y, x, count = 0;

	for(y=0; y<IMGY ; y++){
		for(x=0; x<IMGX ; x++){
			if(img[y][x] == label) count++;								
		}	
	}
	if(count <= size) {
		for(y=0; y<IMGY  ; y++){
			for(x=0; x<IMGX ; x++){
				if(img[y][x] == label) img[y][x] = 0;	
			}
		}
	}
}

void NoiseErase( char img[IMGY][IMGX], int size ) {
	/*
	 *	画像に対してラベリング処理を行い、size個以下の黒画素の塊をノイズと
	 *	みなして削除する。
	 */
	int y, x;
	label(img);
	
	
	for(y=0; y<IMGY; y++){
		for(x=0; x<IMGX; x++){
			if(img[y][x] > 1){
			//	Label = img[y][x];
				EraseByLabel(img, img[y][x], size);
			}
		}
	}
	
	for(y=0; y<IMGY; y++){
		for(x=0; x<IMGX; x++){
			if( img[y][x] > 0 ) img[y][x] = 1;
		}
	}
}


/*
 *		文字画像を正規化する関数
 */
void Normalize( char img[IMGY][IMGX] ) {
	/*
	 *	課題４で作成したプログラム。
	 */
	int x0, y0, w, h, i, j, k, u, v;
	int rightEdge, leftEdge, topEdge, botEdge;
	leftEdge = topEdge = rightEdge = botEdge = 64 ;
	 
	for(i=0; i<64; i++){							//   caclculate padding
		for(j=0; j<64; j++){
			if(img[i][j] >= 1){
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
	
//	printf("\ntop w left h = %d %d %d %d\n", topEdge, w, leftEdge, h);
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
	
}

/*
 *		文字の輪郭を抽出する関数
 */
void Outline( char img[IMGY][IMGX] ) {
	/*
	 *	課題３で作成したプログラム。
	 */
	int i,x,y;	x=y=0;
	
	for(y=1; y<63; y++){
		for(x=1; x<63; x++){
			if(img[y][x] >= 1){	// check interior pixels or boundary pixels
				if( (img[y][x] == img[y][x+1] || img[y][x+1] == 99) &&	
					(img[y][x] == img[y][x-1] || img[y][x-1] == 99) &&	
					(img[y][x] == img[y+1][x] || img[y+1][x] == 99) &&		
					(img[y][x] == img[y-1][x] || img[y-1][x] == 99)   ){   
					
					img[y][x] = 99;	// interior pixels will be 20
				}  
			}
		
		}
	}
	for(y=0; y<64; y++){
		for(x=0; x<64; x++){	// change value of all interior pixels
			if(img[y][x] == 99)  img[y][x] = 0;
		}
	}
}

/*
 *		特徴を抽出する関数
 */
 
 FTR countInArea(int y, int x , char img[IMGY][IMGX] ){
 	int i, j, horizontal , vertical, upDirect, downDirect ;
 	horizontal = vertical = upDirect = downDirect = 0;
 	FTR ftr;
	 for(i=y; i < y+15; i++){
	 	for(j=x; j < x+15; j++){
	 		if(img[i][j] > 0){
	 			if(img[i][j+1] > 0) vertical++;
	 			if(img[i][j-1] > 0) vertical++;
	 			if(img[i-1][j] > 0) horizontal++;
	 			if(img[i+1][j] > 0) horizontal++;
	 			if(img[i-1][j+1] > 0) upDirect++;
	 			if(img[i+1][j-1] > 0) upDirect++;
	 			if(img[i-1][j-1] > 0) downDirect++;
	 			if(img[i+1][j+1] > 0) downDirect++;
			 }
		 }
	 }
	 ftr.v = vertical;
	 ftr.h = horizontal;
	 ftr.ur = upDirect;
	 ftr.dr = downDirect;
//	 printf("[v = %3d, h = %3d, ur =%3d, dr = %3d]\n", ftr.v, ftr.h, ftr.ur, ftr.dr);
	 return ftr;
 }
 
void ExtFtr( char img[IMGY][IMGX], FTR ftr[SEGS][SEGS] ) {
	/*
	 *	文字画像img[][]から方向線素特徴量を抽出しftr[][]に格納する。
	 */
	int Xarea[7] = {0, 16, 32, 48, 8, 24, 40};
	int Yarea[7] = {0, 16, 32, 48, 8, 24, 40};
	
	int i,j; 
	for(i=0; i<7; i++){
		for(j=0; j<7; j++){
			ftr[i][j] = countInArea(Yarea[i], Xarea[j], img);
		}
	}
	
}

/*
 *		特徴を画面に出力する関数
 */
void PrintFtr( FTR ftr[SEGS][SEGS] ) {
	/*
	 *	ftr[][]に格納された方向線素特徴量を画面に表示する。
	 *	特徴量は196個の正数から成るため、それらをカンマで区切って出力する。
	 *	数値の出力順番は問わない。
	 */
	int i,j;
	for(i=0; i<SEGS; i++){
		for(j=0; j<SEGS; j++){
			printf("[v = %3d, h = %3d, ur =%3d, dr = %3d]\n", ftr[i][j].v, ftr[i][j].h, ftr[i][j].ur, ftr[i][j].dr);
		}
	}
}



int main( ) {
	FILE			*fp ;				/* ファイルポインタ */
	char			imgfile[0x80] ;		/* ファイル名 */
	unsigned char	data[IMGSIZE] ;		/* 1文字分の画像データ */
	char			img[IMGY][IMGX] ;	/* 1文字分の画像(2次元配列版) */
	FTR				ftr[SEGS][SEGS] ;	/* 1文字分の特徴量 */

	/*
	 *		画像ファイル名の入力
	 */
	fprintf( stderr,"画像ファイル名を入力してください：" ) ;
	scanf( "%s", imgfile ) ;

	/*
	 *		画像ファイルのオープン
	 */
	if( ( fp = fopen( imgfile, "rb" ) ) == NULL ) {
		fprintf( stderr, "cannot open %s\n", imgfile ) ;
		return 1 ;
	}

	/*
	 *		ファイルに文字画像データがある限り繰り返し以下の操作を行う
	 */
	int count = 1;
	while( fread( data, IMGSIZE, 1, fp ) ) {
		printf("print %dth\n", count);
		/*
		 *	512バイトデータをビットパタンに展開
		 */
		ExpandImg( data, img ) ;
		PrintPtn( img ) ;/*	結果を表示するデバッグ用関数 */
		
		/*
		 *	ノイズ除去
		 */
		NoiseErase( img, NOISESIZE ) ;
	//	PrintPtn( img ) ;	/*結果を表示するデバッグ用関数 */
	//	display(img);
		/*
		 *	正規化
		 */
		Normalize( img ) ;
	//	PrintPtn( img ) ;/*	結果を表示するデバッグ用関数 */
		
		/*
		 *	輪郭抽出
		 */
		Outline( img ) ;
	//	PrintPtn( img ) ; /*	結果を表示するデバッグ用関数 */
		
	
		/*
		 *	特徴量抽出
		 */
		ExtFtr( img, ftr ) ;
	//	PrintFtr( ftr ) ;
		
		count++;
		printf("\n-------------------\n");
	
	}

	fclose( fp ) ;
	return 0 ;
}

/*
 01.imgの方向線素特徴量
 7x7 = 49 area, each area we have number of 4 direction.
 	
[v =  46, h =   6, ur = 12, dr =   2]
[v =  33, h =  24, ur = 20, dr =   7]
[v =  26, h =   2, ur =  8, dr =   4]
[v =   0, h =   0, ur =  0, dr =   0]
[v =  38, h =  22, ur = 20, dr =   2]
[v =  45, h =   2, ur = 10, dr =   7]
[v =   2, h =   2, ur =  2, dr =   2]
[v =   1, h =  26, ur =  3, dr =   1]
[v =  18, h =  27, ur = 13, dr =   2]
[v =  47, h =  10, ur =  6, dr =   5]
[v =  22, h =   3, ur =  0, dr =  10]
[v =  10, h =  51, ur = 10, dr =   3]
[v =  25, h =  12, ur = 14, dr =   4]
[v =  48, h =   0, ur =  0, dr =   6]
[v =  14, h =  24, ur = 29, dr =   1]
[v =  21, h =  20, ur = 20, dr =  11]
[v =   2, h =   0, ur =  0, dr =   2]
[v =  10, h =  31, ur = 12, dr =  15]
[v =  21, h =  21, ur = 33, dr =   3]
[v =  14, h =  12, ur = 13, dr =  11]
[v =   0, h =   1, ur =  1, dr =   2]
[v =  15, h =   5, ur =  4, dr =   6]
[v =   7, h =   5, ur =  2, dr =   4]
[v =  21, h =   9, ur = 24, dr =   3]
[v =  14, h =  15, ur = 30, dr =   0]
[v =  15, h =   3, ur =  6, dr =   0]
[v =  12, h =   7, ur =  6, dr =   8]
[v =  25, h =  12, ur = 40, dr =   0]
[v =  24, h =  17, ur = 10, dr =   2]
[v =  10, h =  33, ur = 14, dr =   1]
[v =   4, h =   6, ur =  4, dr =   2]
[v =   0, h =   0, ur =  0, dr =   0]
[v =  19, h =  44, ur = 19, dr =   2]
[v =  10, h =   6, ur =  6, dr =   2]
[v =   0, h =   0, ur =  0, dr =   0]
[v =   6, h =  21, ur =  9, dr =   2]
[v =  22, h =  19, ur = 20, dr =   3]
[v =  45, h =   1, ur =  3, dr =   3]
[v =  29, h =  19, ur =  0, dr =  29]
[v =  14, h =  32, ur = 11, dr =   5]
[v =  27, h =   9, ur = 23, dr =   2]
[v =  50, h =   0, ur =  0, dr =  10]
[v =  24, h =  24, ur = 22, dr =   8]
[v =  24, h =  18, ur = 11, dr =  15]
[v =   9, h =   4, ur =  4, dr =   5]
[v =  14, h =  30, ur = 36, dr =   0]
[v =  30, h =  17, ur = 29, dr =   2]
[v =  24, h =  11, ur =  4, dr =  21]
[v =   6, h =  10, ur = 15, dr =   0]
[v =  46, h =   6, ur = 12, dr =   2]
[v =  33, h =  24, ur = 20, dr =   7]
[v =  26, h =   2, ur =  8, dr =   4]
[v =   0, h =   0, ur =  0, dr =   0]
[v =  38, h =  22, ur = 20, dr =   2]
[v =  45, h =   2, ur = 10, dr =   7]
[v =   2, h =   2, ur =  2, dr =   2]
[v =   1, h =  26, ur =  3, dr =   1]
[v =  18, h =  27, ur = 13, dr =   2]
[v =  47, h =  10, ur =  6, dr =   5]
[v =  22, h =   3, ur =  0, dr =  10]
[v =  10, h =  51, ur = 10, dr =   3]
[v =  25, h =  12, ur = 14, dr =   4]
[v =  48, h =   0, ur =  0, dr =   6]
[v =  14, h =  24, ur = 29, dr =   1]
[v =  21, h =  20, ur = 20, dr =  11]
[v =   2, h =   0, ur =  0, dr =   2]
[v =  10, h =  31, ur = 12, dr =  15]
[v =  21, h =  21, ur = 33, dr =   3]
[v =  14, h =  12, ur = 13, dr =  11]
[v =   0, h =   1, ur =  1, dr =   2]
[v =  15, h =   5, ur =  4, dr =   6]
[v =   7, h =   5, ur =  2, dr =   4]
[v =  21, h =   9, ur = 24, dr =   3]
[v =  14, h =  15, ur = 30, dr =   0]
[v =  15, h =   3, ur =  6, dr =   0]
[v =  12, h =   7, ur =  6, dr =   8]
[v =  25, h =  12, ur = 40, dr =   0]
[v =  24, h =  17, ur = 10, dr =   2]
[v =  10, h =  33, ur = 14, dr =   1]
[v =   4, h =   6, ur =  4, dr =   2]
[v =   0, h =   0, ur =  0, dr =   0]
[v =  19, h =  44, ur = 19, dr =   2]
[v =  10, h =   6, ur =  6, dr =   2]
[v =   0, h =   0, ur =  0, dr =   0]
[v =   6, h =  21, ur =  9, dr =   2]
[v =  22, h =  19, ur = 20, dr =   3]
[v =  45, h =   1, ur =  3, dr =   3]
[v =  29, h =  19, ur =  0, dr =  29]
[v =  14, h =  32, ur = 11, dr =   5]
[v =  27, h =   9, ur = 23, dr =   2]
[v =  50, h =   0, ur =  0, dr =  10]
[v =  24, h =  24, ur = 22, dr =   8]
[v =  24, h =  18, ur = 11, dr =  15]
[v =   9, h =   4, ur =  4, dr =   5]
[v =  14, h =  30, ur = 36, dr =   0]
[v =  30, h =  17, ur = 29, dr =   2]
[v =  24, h =  11, ur =  4, dr =  21]
[v =   6, h =  10, ur = 15, dr =   0]
*/
