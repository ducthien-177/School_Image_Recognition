#include <stdio.h>

#define AVEFILE		"ave.dic"		/* 辞書(平均特徴量)のファイル名 */
#define	EIGENVV		"eigenvv.dic"	/* 辞書(固有値・固有ベクトル)のファイル名 */
#define CTGRY		46				/* 字種数 */
#define FTRDIM		196				/* 特徴量の次元 */
#define	CHARNUM		20				/* 認識対象文字数(字種あたり) */
#define	BIAS		0.0				/* マハラノビス距離で使うバイアス(自分で最適値を決める) */

/*
 *		①平均特徴量ファイルの読み込み
 */
void ReadEigenVV( FILE *fp)
{
	double value[2];
	double vector[2][4];
	char   string[10];
	int i,j;  
	for(i=0; i<2; i++){
		fscanf(fp, "%6c\t%lf\t%6c", string, &value[i], string);
		printf("value[%d]= %lf\n", i, value[i]);
		for(j=0; j<4; j++){
			fscanf(fp, "%lf\t", &vector[i][j]);
			printf("value[%d][%d]= %lf\n", i, j, vector[i][j] );
		}
	}

}

int main(void){
	FILE *fp;
	
	fp = fopen("testEigenvv.txt", "r");
	if(fp == NULL) return -1;
	
	ReadEigenVV(fp);
}