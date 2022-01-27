/*
 *		画像認識工学　課題９
 *		情報工学科　4J41 LE DUC THIEN
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define AVEFILE		"ave.dic"		/* 辞書(平均特徴量)のファイル名 */
#define	EIGENVV		"eigenvv.dic"	/* 辞書(固有値・固有ベクトル)のファイル名 */
#define CTGRY		46				/* 字種数 */
#define FTRDIM		196				/* 特徴量の次元 */
#define	CHARNUM		20				/* 認識対象文字数(字種あたり) */
#define	BIAS		0.0				/* マハラノビス距離で使うバイアス(自分で最適値を決める) */

/*
 *		①平均特徴量ファイルの読み込み
 */
void ReadAve( double ave[CTGRY][FTRDIM])
{

	//AVEFILEから平均特徴量を読み込みave[][]に代入する。
	FILE *fp;
	fp = fopen(AVEFILE, "r");
	if(fp == NULL){
		printf("load ave.dic failed!!\n");
		exit(1);
	} 

	int i,j;  
	for(i=0; i<CTGRY; i++){
		for(j=0; j<FTRDIM; j++){
			fscanf(fp, "%lf", &ave[i][j]);
		//	printf(" %lf\n", ave[i][j]);		
		}
	}
	 
	fclose(fp);
}

/*
 *		②辞書(固有値・固有ベクトル)のファイルの読み込み
 */

void ReadEigenVV( double EigenVal[CTGRY][FTRDIM], double EigenVec[CTGRY][FTRDIM][FTRDIM] )
{

//	EIGENVVから固有値・固有ベクトルを読み込み、EigenVal[][],EigenVec[][][]に代入する。
	FILE *fp;
	fp = fopen(EIGENVV, "r");
	if(fp == NULL){
		printf("load eigenvv.dic failed!!\n");
		exit(1);
	}else printf("reading file\n");
	
	int k,i,j;
	for(k=0; k<CTGRY; k++){
		for(i=0; i<FTRDIM; i++){
			fscanf(fp, "%lf\n", EigenVal[k][i]);
			printf("value = %lf\n", EigenVal[k][i]);
			for(j=0; j<FTRDIM; j++){
				fscanf(fp, "%lf\t", EigenVec[k][i][j]);
			}
		}
	
	}
	
	fclose(fp);
	
//	printf("value[0][1] = %lf\n", EigenVal[0][1]);
//	printf("vector = %lf\n", EigenVal[0][0][0]);
	
	
}

void subVector( int ftr[FTRDIM], double ave[CTGRY][FTRDIM], double subAns[CTGRY][FTRDIM]){
	int i,j;
	for(i=0; i<CTGRY; i++){
		for(j=0; j<FTRDIM; j++){
			subAns[i][j] = ftr[j] - ave[i][j];
		}
	}	
}

double innerProduct(double subAns[CTGRY][FTRDIM], double EigenVec[CTGRY][FTRDIM][FTRDIM], int CTGRY_index, int FTRDIM_index){
	double ans = 0;
	for(int i=0; i<FTRDIM; i++){
		ans += subAns[CTGRY_index][i]*EigenVec[CTGRY_index][FTRDIM_index][i];	
	}
	return ans;
}
/*
 *		③マハラノビス距離の計算
 */
void Mahalanobis( int ftr[FTRDIM], double ave[CTGRY][FTRDIM], double EigenVal[CTGRY][FTRDIM],
		double EigenVec[CTGRY][FTRDIM][FTRDIM], double b, double dist[CTGRY], double subAns[CTGRY][FTRDIM] )
{
/*
	未知の特徴量ftr[]と、各字種の標準データとのマハラノビス距離を計算し、
	dist[]に格納する。距離計算に必要な平均特徴量はave[][]、固有値はEigenVal[][]、
	固有ベクトルはEigenVec[][][]に入っている。
	bはバイアスである。
*/	
	int i,j;
	for(i=0; i<CTGRY; i++){
		for(j=0; j<FTRDIM; j++){
			dist[i] += pow( innerProduct( subAns, EigenVec, i, j ), 2.0 )/( EigenVal[i][j] + b );	
		}
	}

		
}

/*
 *		認識候補の算出　　dist[]の中から距離最小のインデックスを求めて返す。
 */
int RecogRst( double dist[CTGRY] )
{
	int i, id = 0 ;

	for( i = 0 ; i < CTGRY ; i++ ) {
		if( dist[i] < dist[id] ) {
			id = i ;
		}
	}
	return  id ;
}

int main( )
{

	FILE	*fp ;						/* ファイルポインタ */
	char	fname[0x80] ;				/* 特徴ファイル名 */
	int		ftr[FTRDIM] ;				/* 特徴量 */
	double	ave[CTGRY][FTRDIM]; 	/* 平均特徴量 */
	double	EigenVal[CTGRY][FTRDIM] ;	/* 固有値 */
	double	EigenVec[CTGRY][FTRDIM][FTRDIM] ;	/* 固有ベクトル */
	double  subAns[CTGRY][FTRDIM]; 		/*vector 引き算の結果*/
	double	dist[CTGRY] ;				/* ユークリッド距離 */
	double	b = BIAS ;					/* マハラノビス距離で使うバイアス */
	int		num, crct ;					/* 認識対象・正解文字数 */
	int		total=0, csum=0 ;			/* 認識対象・正解文字数(全字種) */
	int		i, j, k ;

	
	

	ReadAve( ave ) ;
	
	ReadEigenVV( EigenVal, EigenVec ) ;
	

	for( i = 0 ; i < CTGRY ; i++ ) {
		sprintf( fname, "unknown%02d.txt", i+1 ) ;

		
		//	特徴ファイルのオープン
		 
		if( ( fp = fopen( fname, "r" ) ) == NULL ) {
			fprintf( stderr, "cannot open %s\n", fname ) ;
			return 1 ;
		}

		num = 0 ;
		crct = 0 ;

		for( j = 0 ; j < CHARNUM ; j++ ) {
			for( k = 0 ; k < FTRDIM ; k++ ) {
				fscanf( fp, "%d", &ftr[k] ) ;
			}
			num++ ;
			total++ ;
			Mahalanobis( ftr, ave, EigenVal, EigenVec, b, dist, subAns ) ;
			if( RecogRst( dist ) == i ) {
				crct++ ;
				csum++ ;
			}
		}
		printf( "no.%02d   %d / %d = %.2f%%\n", i+1, crct, num, (double)crct/(double)num*100 ) ;
	}

	printf( "TOTAL   %d / %d = %.2f%%\n", csum, total, (double)csum/(double)total*100 ) ;

	fclose( fp ) ;

	return 0 ;
}

