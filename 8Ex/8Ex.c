/*
 *		画像認識工学　課題８
 *		情報工学科　4J41 LE DUC THIEN
 *
 */
#include <stdio.h>
#include <math.h>

#define COVFILE		"cov.dic"		/* 共分散行列のファイル名 */
#define	EIGENVVFILE	"eigenvv.dat"	/* 固有値・固有ベクトルのファイル */
#define CTGRY		46				/* 字種数 */
#define FTRDIM		196				/* 特徴量の次元 */

/*
 *		①　共分散行列の読み込み
 */
void ReadCov( FILE *fp, double Cov[FTRDIM][FTRDIM] )
{

//	fpから1字種分の共分散行列を読み込み、Cov[][]に代入する関数を作成する。
	int i,j;  
	for(i=0; i<FTRDIM; i++){
		for(j=0; j<FTRDIM; j++){
			fscanf(fp, "%lf", &Cov[i][j]);
		}
	}


}


void disp_matrix(double matrix[FTRDIM][FTRDIM]){
	int i,j;
	for(i=0; i<FTRDIM; i++){
		printf("row %d\n", i);
		for(j=0; j<FTRDIM; j++){
			printf("%5.3f\t", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
/*
 *		②　ヤコビ法
 */
void Jacobi( double Cov[FTRDIM][FTRDIM], double EigenVal[FTRDIM], double EigenVec[FTRDIM][FTRDIM] )
{

//	ヤコビ法により、共分散行列Cov[][]の固有値及び固有ベクトルをもとめ、
//	EigenVal[]及びEigenVec[][]にそれぞれ代入する。

//	固有値EigenVal[k]に対する固有ベクトルはEigenVec[k][0]～EigenVec[k][195]に
//	記録し、対応がとれるようにする。
//	固有値の大きい順に並べることが望ましい。（必須ではない）
	int i, j, k;
	//init EigenVec
	//単位行列になるように初期化
	for(i=0; i< FTRDIM; i++){
		for(j=0; j<FTRDIM; j++){
			if(i == j) EigenVec[i][j] = 1.0;
			else 	   EigenVec[i][j] = 0.0;
		}
	}

    for (k = 1; k <= FTRDIM*FTRDIM/2; k++)
    {
        // 最大値を探す
        int p = 0;
        int q = 0;
        double max_val = 0.0;
        for (i = 0; i < FTRDIM; i++)
        {
            for (j = i + 1; j < FTRDIM; j++)
            {
                if (max_val < fabs(Cov[i][j]))
                {
                    max_val = fabs(Cov[i][j]);
                    p = i;
                    q = j;
                }
            }
        }

        // θ を求める
        double t = 0.0;
        if (fabs(Cov[p][p] - Cov[q][q]) < 0.00000000001)
        {
            // a_{pp} ＝ a_{qq} のとき、回転角ｔをπ/4にする
            t = M_PI / 4.0;
            if (Cov[p][p] < 0)
                t = -t;
        }
        else
        {
            // a_{pp} ≠ a_{qq} のとき
            t = atan(2.0 * Cov[p][q] / (Cov[p][p] - Cov[q][q])) / 2.0;
        }

        // θ を使って 行列 U を作成し、A = U^t × A × U
        double c = cos(t);
        double s = sin(t);
        // U^t × A
        double t1 = 0.0;
        double t2 = 0.0;
        for (i = 0; i < FTRDIM; i++)
        {
            t1      =  Cov[p][i] * c + Cov[q][i] * s;
            t2      = -Cov[p][i] * s + Cov[q][i] * c;
            Cov[p][i] = t1;
            Cov[q][i] = t2;
            // 固有ベクトル
            t1      =  EigenVec[p][i] * c + EigenVec[q][i] * s;
            t2      = -EigenVec[p][i] * s + EigenVec[q][i] * c;
            EigenVec[p][i] = t1;
            EigenVec[q][i] = t2;
        }

        // A × U
        for (i = 0; i < FTRDIM; i++)
        {
            t1      =  Cov[i][p] * c + Cov[i][q] * s;
            t2      = -Cov[i][p] * s + Cov[i][q] * c;
            Cov[i][p] = t1;
            Cov[i][q] = t2;
        }

       
        for(i=0; i<FTRDIM; i++){
        	EigenVal[i] = Cov[i][i];
        //	printf("\n eigen value : %10.3f", EigenVal[i]);
		}
        

        // 収束判定
        if (max_val < 0.00000000001) break;
    }

}

/*
 *		③　計算結果の書き出し
 */
void WriteEigenVV( double EigenVal[FTRDIM], double EigenVec[FTRDIM][FTRDIM], FILE *fp )
{

//	固有値EigenVal[]と固有ベクトルEigenVec[][]をファイルfpに書き込む。
//	書式は問わないが、固有値と固有ベクトルは対応が取れるようにすること。
	int i,j;
	for(i=0; i<FTRDIM; i++){
		fprintf(fp, "No.%d Eigen value = %7.3f, eigen vector:\n",i, EigenVal[i]);
		for(j=0; j<FTRDIM; j++){
			fprintf(fp, "%7.7f\t", EigenVec[i][j]);	
		}	
		fprintf(fp,"\n");
	}
	fprintf(fp,"-------------DONE---------------\n\n");
}


int main( )
{
	FILE	*fpcov ;				/* ファイルポインタ */
	FILE	*fpout ;				/* 出力ファイル用ポインタ */
	double	Cov[FTRDIM][FTRDIM] ;	/* 共分散行列 */
	double	EigenVal[FTRDIM] ;		/* 固有値 */
	double	EigenVec[FTRDIM][FTRDIM] ;	/* 固有ベクトル */
	int		i, j, k ;

	if( ( fpcov = fopen( COVFILE, "r" ) ) == NULL ) {
		fprintf( stderr, "cannot open %s\n", COVFILE ) ;
		return 1 ;
	}

	if( ( fpout = fopen( EIGENVVFILE, "w" ) ) == NULL ) {
		fprintf( stderr, "cannot open %s\n", EIGENVVFILE ) ;
		return 1 ;
	}

	for( i = 0 ; i < CTGRY ; i++ ) {

		/*
		 *	1字種分の共分散行列を読み込む．
		 */
		ReadCov( fpcov, Cov ) ;
		printf("ReadCov ------------------------------No.%3d--------\n", i);
		/*
		 *	ある字種の共分散行列Covの固有値・固有ベクトルを求める．
		 */
		Jacobi( Cov, EigenVal, EigenVec ) ;
		printf("Calculated Jacobi\n");
		/*
		 *	1字種分の固有値・固有ベクトルをファイルに書き込む．
		 */
		fprintf(fpout, "Write file No.%d\n", i);
		WriteEigenVV( EigenVal, EigenVec, fpout ) ;
		printf("Write Eigen value and Eigen vector-----DONE---------\n\n\n");
		
	}

	fclose( fpcov ) ;
	fclose( fpout ) ;
	return 0 ;
}