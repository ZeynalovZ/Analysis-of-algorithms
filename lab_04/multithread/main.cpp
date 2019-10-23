#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#define OK 0

#define CANT_MULTIPLY       -1
#define ERR_MEM             -2
#define ERR_EMPTY_MATRIX    -3
#define ERR_MTR_SIZE        -4
using namespace std;
struct Matrix
{
    Matrix(int rc, int cc)
    {
        rows_count = rc;
        columns_count = cc;
    }
    Matrix(){}
    int rows_count;
    int columns_count;
    int **matrix;
};

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
int createMatrix(Matrix &matr)
{
    int code_error = OK;
    int **mtr = allocateMatrix(matr.rows_count, matr.columns_count);
    if (mtr == NULL)
    {
        code_error = ERR_MEM;
    }
    else
    {
        code_error = fillMatrix(mtr, matr.rows_count, matr.columns_count);
        matr.matrix = mtr;
    }
    return code_error;
}


void printMatrix(int **matrix, int row_count, int column_count)
{
    cout << "Matrix:" << row_count << " " << column_count << endl;
    for (int i = 0; i < row_count; i++)
    {
        for (int j = 0; j < column_count; j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}


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
            MulH[i] -= src1[i][j * 2] * src1[i][2 * j + 1];
        }
    }
    for (int i = 0; i < cc2; i++)
    {
        for (int j = 0; j < rc2_2; j++)
        {
            MulV[i] -= src2[j * 2][i] * src2[2 * j + 1][i];
        }
    }
    int N = rc2 - 1;
    bool flag = rc2 % 2;
    for (int i = 0; i < rc1; i++)
    {
        for (int j = 0; j < cc2; j++)
        {
            tmpMtr[i][j] = MulH[i] + MulV[j];
            for (int k = 0; k < rc2_2; k++)
            {
                tmpMtr[i][j] += (src1[i][k * 2] + src2[2 * k + 1][j]) * (src1[i][2 * k + 1] + src2[2 * k][j]);
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

void first_part(Matrix src1,int rc1, int rc2_2, std::vector<int> &MulH)
{
    for (int i = 0; i < rc1; i++)
    {
        for (int j = 0; j < rc2_2; j++)
        {
            MulH[i] -= src1.matrix[i][j * 2] * src1.matrix[i][2 * j + 1];
        }
    }
}

void second_part(Matrix src2, int cc2, int rc2_2, std::vector<int> &MulV)
{
    for (int i = 0; i < cc2; i++)
    {
        for (int j = 0; j < rc2_2; j++)
        {
            MulV[i] -= src2.matrix[j * 2][i] * src2.matrix[2 * j + 1][i];
        }
    }
}

void last_part(Matrix src1, Matrix src2, std::vector<int> &MulH, std::vector<int> &MulV, int rc1, int cc2, int rc2_2, Matrix &mtr_res)
{
    int N = src2.rows_count - 1;
    bool flag =  src2.rows_count % 2;
    int rc = rc1;
    int cc = cc2;
    mtr_res.rows_count = rc1;
    mtr_res.columns_count = cc2;
    int **tmpMtr = allocateMatrix(rc, cc);
    for (int i = 0; i < rc1; i++)
    {
        for (int j = 0; j < cc2; j++)
        {
            tmpMtr[i][j] = MulH[i] + MulV[j];
            for (int k = 0; k < rc2_2; k++)
            {
                tmpMtr[i][j] += (src1.matrix[i][k * 2] + src2.matrix[2 * k + 1][j]) * (src1.matrix[i][2 * k + 1] + src2.matrix[2 * k][j]);
            }
        }
    }

    for (int i = 0; i < rc1; i++)
    {
        for (int j = 0; j < cc2; j++)
        {
            if (flag)
            {
                tmpMtr[i][j] += src1.matrix[i][N] * src2.matrix[N][j];
            }
        }
    }
    mtr_res.matrix = tmpMtr;
}

int multi2(Matrix mtr1, Matrix mtr2, Matrix &mtr_res)
{
    if (mtr1.columns_count != mtr2.rows_count)
    {
        return CANT_MULTIPLY;
    }

    int rows_count_first = mtr1.rows_count;
    int rows_count_second_2 = mtr2.rows_count / 2;
    int columns_count_second = mtr2.columns_count;

    mutex m1rc, m2rc_2, m2cc;
    mutex m1, m2, m_out;
    mutex row, column;
    mutex outout;

    std::vector<int> MulH(mtr1.rows_count);
    std::vector<int> MulV(mtr2.columns_count);

    thread thread_1_1(first_part, mtr1,
                      rows_count_first,
                      rows_count_second_2,
                      ref(MulH));
    thread thread_2_1(second_part, mtr2,
                      columns_count_second,
                      rows_count_second_2,
                      ref(MulV));

    if (thread_1_1.joinable() && thread_2_1.joinable())
    {
        thread_1_1.join();
        thread_2_1.join();
    }

    thread thread_3_1(last_part, mtr1,
                      mtr2,
                      ref(MulH),
                      ref(MulV),
                      rows_count_first,
                      columns_count_second,
                      rows_count_second_2,
                      ref(mtr_res));
    thread_3_1.join();
    /*
     * thread thread_1_1(first_part, mtr1, ref(m1),
                      rows_count_first, ref(m1rc),
                      rows_count_second_2, ref(m2rc_2),
                      ref(MulH), ref(row));
    thread thread_2_1(second_part, mtr2, ref(m2),
                      columns_count_second, ref(m2cc),
                      rows_count_second_2, ref(m2rc_2),
                      MulV, ref(column));

    if (thread_1_1.joinable() && thread_2_1.joinable())
    {
        thread_1_1.join();
        thread_2_1.join();
    }
    thread thread_3_1(last_part, mtr1, ref(m1),
                      mtr2, ref(mtr2),
                      MulH, (row),
                      MulV, (row),
                      rows_count_first, (m1rc),
                      columns_count_second, (m2cc),
                      rows_count_second_2, (m2rc_2),
                      mtr_res, ref(m_out));
    thread_3_1.join();
    */
    //printMatrix(mtr_res.matrix, mtr_res.rows_count, mtr_res.columns_count);
}



int main()
{
    Matrix mtr1(2, 2);
    createMatrix(mtr1);
    printMatrix(mtr1.matrix, mtr1.rows_count, mtr1.columns_count);
    Matrix mtr2(2, 2);
    createMatrix(mtr2);
    printMatrix(mtr2.matrix, mtr2.rows_count, mtr2.columns_count);
    Matrix mtr3;
    multi2(mtr1, mtr2, mtr3);
    printMatrix(mtr3.matrix, mtr3.rows_count, mtr3.columns_count);
    return 0;
}
