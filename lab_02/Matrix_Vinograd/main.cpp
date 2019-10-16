#include <iostream>
#include <time.h>
#include <windows.h>
#include <chrono>

#define OK 0
#define ERR_MEM             -1
#define ERR_EMPTY_MATRIX    -2
#define ERR_MTR_SIZE        -3
using namespace std;
unsigned long long tick(void)
{
    unsigned long long d;
    __asm__ __volatile__ ("rdtsc" : "=A" (d) );

    return d;
}

int** allocateMatrix(int n, int m)
{
    int **matrix = new int*[n];
    for (int i = 0; i < n; i++)
    {
        matrix[i] = new int[m];
        for (int j = 0; j < m; j++)
        {
            matrix[i][j] = 0;
        }
    }
    return matrix;
}
void clearMatrix(int **matrix, int row_count)
{
    for (int i = 0; i < row_count; i++)
        delete matrix[i];
    delete matrix;
}


void printMatrix(int **matrix, int row_count, int column_count)
{
    cout << "Matrix:" << row_count << column_count << endl;
    for (int i = 0; i < row_count; i++)
    {
        for (int j = 0; j < column_count; j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}
int fillMatrix(int **matrix, int rowCount, int columnCount)
{
    if (matrix == NULL)
    {
        return ERR_EMPTY_MATRIX;
    }
    if (rowCount == 0 || columnCount == 0)
    {
        return ERR_MTR_SIZE;
    }
    for (int i = 0; i < rowCount; i++)
    {
        for (int j = 0; j < columnCount; j++)
        {
            matrix[i][j] = rand() % 100 + 1;
        }
    }
    return OK;
}

int createMatrix(int ***matrix, int rowCount, int columnCount)
{
    int code_error = OK;
    int **mtr = allocateMatrix(rowCount, columnCount);
    if (mtr == NULL)
    {
        code_error = ERR_MEM;
    }
    else
    {
        code_error = fillMatrix(mtr, rowCount, columnCount);
        *matrix = mtr;
    }
    return code_error;
}
#pragma optimize("", off)
int multiplicateMatrix(int ***result, int &rc, int &cc, int** src1, int **src2, int rc1, int cc1, int rc2, int cc2)
{
    if (cc1 != rc2)
    {
        return ERR_MTR_SIZE;
    }
    if (src1 == NULL || src2 == NULL)
    {
        return ERR_EMPTY_MATRIX;
    }
    rc = rc1;
    cc = cc2;
    int **tmpMtr = allocateMatrix(rc, cc);
    for (int i = 0; i < rc1; i++)
    {
        for (int j = 0; j < cc2; j++)
        {
            for (int k = 0; k < rc1; k++)
            {
                tmpMtr[i][j] += src1[i][k] * src2[k][j];
            }
        }
    }
    *result = tmpMtr;
}
#pragma optimize("", off)
int multiplicateMatrixVinograd(int ***result, int &rc, int &cc, int** src1, int **src2, int rc1, int cc1, int rc2, int cc2)
{
    if (cc1 != rc2)
    {
        return ERR_MTR_SIZE;
    }
    if (src1 == NULL || src2 == NULL)
    {
        return ERR_EMPTY_MATRIX;
    }
    rc = rc1;
    cc = cc2;
    int **tmpMtr = allocateMatrix(rc, cc);
    int *MulH = (int*)calloc(rc1, sizeof(int));
    int *MulV = (int*)calloc(cc2, sizeof(int));
    for (int i = 0; i < rc1; i++)
    {
        for (int j = 0; j < rc2 / 2; j++)
        {

            MulH[i] = MulH[i] + src1[i][j * 2] * src1[i][2 * j + 1];
            //cout << MulH[i] << endl;
        }
    }
    for (int i = 0; i < cc2; i++)
    {
        for (int j = 0; j < rc2 / 2; j++)
        {
            MulV[i] = MulV[i] + src2[j * 2][i] * src2[2 * j + 1][i];
            //cout << src2[j * 2][i] * src2[2 * j + 1][i] << endl;
        }
    }
    for (int i = 0; i < rc1; i++)
    {
        for (int j = 0; j < cc2; j++)
        {
            tmpMtr[i][j] = - (MulH[i] + MulV[j]);

            for (int k = 0; k < rc2 / 2; k++)
            {
                tmpMtr[i][j] = tmpMtr[i][j] + (src1[i][2 * k] + src2[2 * k + 1][j]) * (src1[i][2 * k + 1] + src2[2 * k][j]);
            }

        }
    }
    if (rc2 % 2)
    {
        for (int i = 0; i < rc1; i++)
        {
            for (int j = 0; j < cc2; j++)
            {
                tmpMtr[i][j] += src1[i][rc2 - 1] * src2[rc2 - 1][j];
            }
        }
    }
    *result = tmpMtr;
}
#pragma optimize("", off)
int optimizedMultiplication(int ***result, int &rc, int &cc, int** src1, int **src2, int rc1, int cc1, int rc2, int cc2)
{

    if (cc1 != rc2)
    {
        return ERR_MTR_SIZE;
    }
    if (src1 == NULL || src2 == NULL)
    {
        return ERR_EMPTY_MATRIX;
    }
    int rc2_2 = rc2 >> 1;
    rc = rc1;
    cc = cc2;
    int **tmpMtr = allocateMatrix(rc, cc);
    int *MulH = (int*)calloc(rc1, sizeof(int));
    int *MulV = (int*)calloc(cc2, sizeof(int));
    for (int i = 0; i < rc1; i++)
    {
        for (int j = 0; j < rc2_2; j++)
        {

            MulH[i] = MulH[i] + src1[i][j * 2] * src1[i][2 * j + 1];
            //cout << MulH[i] << endl;
        }
    }
    for (int i = 0; i < cc2; i++)
    {
        for (int j = 0; j < rc2_2; j++)
        {
            MulV[i] = MulV[i] + src2[j * 2][i] * src2[2 * j + 1][i];
            //cout << src2[j * 2][i] * src2[2 * j + 1][i] << endl;
        }
    }
    int N = rc2 - 1;
    bool flag = rc2 % 2;
    for (int i = 0; i < rc1; i++)
    {
        for (int j = 0; j < cc2; j++)
        {
            tmpMtr[i][j] = -MulH[i] - MulV[j];
            for (int k = 0; k < rc2_2; k++)
            {
                tmpMtr[i][j] = tmpMtr[i][j] + (src1[i][2 * k] + src2[2 * k + 1][j]) * (src1[i][2 * k + 1] + src2[2 * k][j]);
            }
        }
    }


    for (int i = 0; i < rc1; i++)
    {
        for (int j = 0; j < cc2; j++)
        {
            if (flag)
            {
                tmpMtr[i][j] += src1[i][N] * src2[N][j];
            }
        }
    }
    *result = tmpMtr;
}

int main()
{
    int start = 100;
    int end = 900;
    int step = 100;
    int rep = 10;

    unsigned long long int tBegin = 0, tEnd = 0;
    double tSum = 0;
    int **matrix1 = NULL;
    int **matrix2 = NULL;
    int **result = NULL;
    int res_r = 0, res_c = 0;
    int code_error = OK;


    //printMatrix(matrix1, rowCount1, columnCount1);
    //printMatrix(matrix2, rowCount2, columnCount2);
    //multiplicateMatrix(&result, res_r, res_c, matrix1, matrix2, rowCount1, columnCount1, rowCount2, columnCount2);
    //multiplicateMatrixVinograd(&result, res_r, res_c, matrix1, matrix2, rowCount1, columnCount1, rowCount2, columnCount2);
    //optimizedMultiplication(&result, res_r, res_c, matrix1, matrix2, rowCount1, columnCount1, rowCount2, columnCount2);
    //cout << endl;
    //printMatrix(result, res_r, res_c);
    for (int i = start; i < end; i += step)
    {
        cout << endl;
        cout << "===== for " << i << " size ========" << endl;
        cout << endl;
        code_error = createMatrix(&matrix1, i, i);
        if (code_error == OK)
        {
            code_error = createMatrix(&matrix2, i, i);
            if (code_error == OK)
            {
                tSum = 0;
                for (int j = 0; j < rep; j++)
                {
                    const auto start = std::chrono::system_clock::now();
                    multiplicateMatrix(&result, res_r, res_c, matrix1, matrix2, i, i, i, i);
                    const auto end = std::chrono::system_clock::now();
                    const auto res = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / 1.0e9;
                    tSum += res;
                }
                tSum /= rep;

                cout << "time simple Matrix is " << tSum << endl;

                tSum = 0;
                for (int j = 0; j < rep; j++)
                {
                    const auto start = std::chrono::system_clock::now();
                    multiplicateMatrixVinograd(&result, res_r, res_c, matrix1, matrix2, i, i, i, i);
                    const auto end = std::chrono::system_clock::now();
                    const auto res = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / 1.0e9;
                    tSum += res;
                }
                //cout << tSum << " ==== " << tBegin << " ====B=== " << tEnd << " ===E==== " << endl;
                tSum /= rep;
                cout << "time Matrix Vinograd is " << tSum << endl;

                tSum = 0;
                for (int j = 0; j < rep; j++)
                {
                    const auto start = std::chrono::system_clock::now();
                    optimizedMultiplication(&result, res_r, res_c, matrix1, matrix2, i, i, i, i);
                    const auto end = std::chrono::system_clock::now();
                    const auto res = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / 1.0e9;
                    tSum += res;
                }
                tSum /= rep;
                cout << "time Matrix Vinograd Optimized is " << tSum << endl;

            }
        }
    }
    return 0;
}
