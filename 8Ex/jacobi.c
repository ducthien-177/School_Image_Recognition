
#include <stdio.h>
#include <math.h>

#define N 4

// ヤコビ法
void jacobi(double a[N][N], double v[N][N]);
// 対角要素を表示
void disp_eigenvalue(double a[N][N]);
// 固有ベクトルを表示
void disp_eigenvector(double matrix[N][N]);

void disp_matrix(double matrix[N][N]);
// １次元配列を表示
void disp_vector(double row[N]);
// 正規化 (ベクトルの長さを１にする)
void normarize(double x[N]);

void multiply(double a[N][N], double x[N]);
// ヤコビ法で固有値を求める
int main()
{
    double a[N][N] = {{5.0, 4.0, 1.0, 1.0},
                      {4.0, 5.0, 1.0, 1.0},
                      {1.0, 1.0, 4.0, 2.0},
                      {1.0, 1.0, 2.0, 4.0}};
    double v[N][N] = {{1.0 ,0.0 ,0.0 ,0.0},
                      {0.0 ,1.0 ,0.0 ,0.0},
                      {0.0 ,0.0 ,1.0 ,0.0},
                      {0.0 ,0.0 ,0.0 ,1.0}};

	double x[N] = {  0.6324555320,    0.6324555320,    0.3162277660 ,   0.3162277660};
    // ヤコビ法
    multiply(a,x);
    printf("---------------\n");
    
    jacobi(a, v);

    printf("\neigenvalue\n");
    disp_eigenvalue(a);

    printf("\neigenvector\n");
    disp_eigenvector(v);

    return 0;
}

void multiply(double a[N][N], double x[N]){
	int i,j; 
	double sum = 0.0;
	for(i=0; i<N; i++){
		for(j=0; j<N; j++){
			sum += a[i][j]*x[j];
			printf("%f (+) ", sum);
		}
		printf(" = %f\n", sum);
		sum = 0.0;
	}
}

// ヤコビ法
void jacobi(double a[N][N], double v[N][N])
{
    int i, j, k;
    for (k = 1; k <= N*N/2; k++)
    {
        // 最大値を探す
        int p = 0;
        int q = 0;
        double max_val = 0.0;
        for (i = 0; i < N; i++)
        {
            for (j = i + 1; j < N; j++)
            {
                if (max_val < fabs(a[i][j]))
                {
                    max_val = fabs(a[i][j]);
                    p = i;
                    q = j;
                }
            }
        }

        // θ を求める
        double t = 0.0;
        if (fabs(a[p][p] - a[q][q]) < 0.00000000001)
        {
            // a_{pp} ＝ a_{qq} のとき、回転角ｔをπ/4にする
            t = M_PI / 4.0;
            if (a[p][p] < 0)
                t = -t;
        }
        else
        {
            // a_{pp} ≠ a_{qq} のとき
            t = atan(2.0 * a[p][q] / (a[p][p] - a[q][q])) / 2.0;
        }

        // θ を使って 行列 U を作成し、A = U^t × A × U
        double c = cos(t);
        double s = sin(t);
        // U^t × A
        double t1 = 0.0;
        double t2 = 0.0;
        for (i = 0; i < N; i++)
        {
            t1      =  a[p][i] * c + a[q][i] * s;
            t2      = -a[p][i] * s + a[q][i] * c;
            a[p][i] = t1;
            a[q][i] = t2;
            // 固有ベクトル
            t1      =  v[p][i] * c + v[q][i] * s;
            t2      = -v[p][i] * s + v[q][i] * c;
            v[p][i] = t1;
            v[q][i] = t2;
        }
       
        // A × U
        for (i = 0; i < N; i++)
        {
            t1      =  a[i][p] * c + a[i][q] * s;
            t2      = -a[i][p] * s + a[i][q] * c;
            a[i][p] = t1;
            a[i][q] = t2;
        }

        // 対角要素を表示
        printf("%3d\t", k);
        disp_eigenvalue(a);
        
       

        // 収束判定
        if (max_val < 0.00000000001) break;
    }
    
//    printf("final matrix a \n", k);
//    disp_matrix(a);
//
//    printf("final matrix v \n", k);
//    disp_matrix(v);
}
// 対角要素を表示

void disp_eigenvalue(double a[N][N])
{
    int i;
    for (i = 0; i < N; i++)
        printf("%14.10f\t", a[i][i]);
    printf("\n");
}
// 固有ベクトルを表示
void disp_eigenvector(double matrix[N][N])
{
    int i, j;
    for (i = 0; i < N; i++)
    {
        double row[N];
        for (j = 0; j < N; j++)
            row[j] = matrix[i][j] ;
        normarize(row);
        disp_vector(row);
    }
}
// １次元配列を表示
void disp_vector(double row[N])
{
    int i;
    for (i = 0; i < N; i++)
        printf("%14.10f\t", row[i]);
    printf("\n");
}

void disp_matrix(double matrix[N][N]){
	int i,j;
	for(i=0; i<N; i++){
		for(j=0; j<N; j++){
			printf("%3.3f\t", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
// 正規化 (ベクトルの長さを１にする)
void normarize(double x[N])
{
    int i;
    double s = 0.0;

    for (i = 0; i < N; i++)
        s += x[i] * x[i];
    
	
    for (i = 0; i < N; i++)
        x[i] /= sqrt(s);
}