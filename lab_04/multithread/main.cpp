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

void first_part(Matrix src1, int rows_begin ,int rc1, int rc2_2, std::vector<int> &MulH)
{
    for (int i = rows_begin; i < rc1; i++)
    {
        for (int j = 0; j < rc2_2; j++)
        {
            MulH[i] -= src1.matrix[i][j * 2] * src1.matrix[i][2 * j + 1];
        }
    }
}

void second_part(Matrix src2, int columns_begin, int cc2, int rc2_2, std::vector<int> &MulV)
{
    for (int i = columns_begin; i < cc2; i++)
    {
        for (int j = 0; j < rc2_2; j++)
        {
            MulV[i] -= src2.matrix[j * 2][i] * src2.matrix[2 * j + 1][i];
        }
    }
}

void last_part(Matrix src1, Matrix src2, std::vector<int> &MulH, std::vector<int> &MulV, int rows_begin, int rc1, int cc2, int rc2_2, Matrix &mtr_res)
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

int multi16(Matrix mtr1, Matrix mtr2, Matrix &mtr_res)
{
    if (mtr1.columns_count != mtr2.rows_count)
    {
        return CANT_MULTIPLY;
    }

    int rows_count_first = mtr1.rows_count;
    int rows_count_second_2 = mtr2.rows_count / 2;
    int columns_count_second = mtr2.columns_count;

    std::vector<int> MulH(mtr1.rows_count);
    std::vector<int> MulV(mtr2.columns_count);

    thread thread_1_1(first_part, mtr1, 0,
                      rows_count_first / 8,
                      rows_count_second_2,
                      ref(MulH));

    thread thread_1_2(first_part, mtr1, rows_count_first / 4,
                      rows_count_first / 8 * 2,
                      rows_count_second_2,
                      ref(MulH));

    thread thread_1_3(first_part, mtr1, rows_count_first / 8 * 2,
                      rows_count_first / 8 * 3,
                      rows_count_second_2,
                      ref(MulH));

    thread thread_1_4(first_part, mtr1, rows_count_first / 8 * 3,
                      rows_count_first / 8 * 4,
                      rows_count_second_2,
                      ref(MulH));
    thread thread_1_5(first_part, mtr1, rows_count_first / 8 * 4,
                      rows_count_first / 8 * 5,
                      rows_count_second_2,
                      ref(MulH));

    thread thread_1_6(first_part, mtr1, rows_count_first / 8 * 5,
                      rows_count_first / 8 * 6,
                      rows_count_second_2,
                      ref(MulH));

    thread thread_1_7(first_part, mtr1, rows_count_first / 8 * 6,
                      rows_count_first / 8 * 7,
                      rows_count_second_2,
                      ref(MulH));

    thread thread_1_8(first_part, mtr1, rows_count_first / 8 * 7,
                      rows_count_first,
                      rows_count_second_2,
                      ref(MulH));

    thread thread_2_1(second_part, mtr1, 0,
                      columns_count_second / 8,
                      rows_count_second_2,
                      ref(MulV));

    thread thread_2_2(second_part, mtr2, columns_count_second / 8,
                      columns_count_second / 8 * 2,
                      rows_count_second_2,
                      ref(MulV));

    thread thread_2_3(second_part, mtr2, columns_count_second / 8 * 2,
                      columns_count_second / 8 * 3,
                      rows_count_second_2,
                      ref(MulV));

    thread thread_2_4(second_part, mtr2, columns_count_second / 8 * 3,
                      columns_count_second / 8 * 4,
                      rows_count_second_2,
                      ref(MulV));
    thread thread_2_5(second_part, mtr1, columns_count_second / 8 * 4,
                      columns_count_second / 8 * 5,
                      rows_count_second_2,
                      ref(MulV));

    thread thread_2_6(second_part, mtr2, columns_count_second / 8 * 5,
                      columns_count_second / 8 * 6,
                      rows_count_second_2,
                      ref(MulV));

    thread thread_2_7(second_part, mtr2, columns_count_second / 8 * 6,
                      columns_count_second / 8 * 7,
                      rows_count_second_2,
                      ref(MulV));

    thread thread_2_8(second_part, mtr2, columns_count_second / 8 * 7,
                      columns_count_second,
                      rows_count_second_2,
                      ref(MulV));



    if (thread_1_1.joinable() && thread_2_1.joinable())
    {
        thread_1_1.join();
        thread_1_2.join();
        thread_1_3.join();
        thread_1_4.join();
        thread_1_5.join();
        thread_1_6.join();
        thread_1_7.join();
        thread_1_8.join();
        thread_2_1.join();
        thread_2_2.join();
        thread_2_3.join();
        thread_2_4.join();
        thread_2_5.join();
        thread_2_6.join();
        thread_2_7.join();
        thread_2_8.join();
    }

    thread thread_3_1(last_part, mtr1,
                      mtr2,
                      ref(MulH),
                      ref(MulV),
                      0, rows_count_first / 16, columns_count_second, rows_count_second_2,
                      ref(mtr_res));
    thread thread_3_2(last_part, mtr1,
                      mtr2,
                      ref(MulH),
                      ref(MulV),
                      rows_count_first / 16, rows_count_first / 16 * 2, columns_count_second, rows_count_second_2,
                      ref(mtr_res));
    thread thread_3_3(last_part, mtr1,
                      mtr2,
                      ref(MulH),
                      ref(MulV),
                      rows_count_first / 16 * 2, rows_count_first / 16 * 3, columns_count_second, rows_count_second_2,
                      ref(mtr_res));
    thread thread_3_4(last_part, mtr1,
                      mtr2,
                      ref(MulH),
                      ref(MulV),
                      rows_count_first / 16 * 3, rows_count_first / 16 * 4, columns_count_second, rows_count_second_2,
                      ref(mtr_res));
    thread thread_3_5(last_part, mtr1,
                      mtr2,
                      ref(MulH),
                      ref(MulV),
                      rows_count_first / 16 * 4, rows_count_first / 16 * 5, columns_count_second, rows_count_second_2,
                      ref(mtr_res));
    thread thread_3_6(last_part, mtr1,
                      mtr2,
                      ref(MulH),
                      ref(MulV),
                      rows_count_first / 16 * 5, rows_count_first / 16 * 6, columns_count_second, rows_count_second_2,
                      ref(mtr_res));
    thread thread_3_7(last_part, mtr1,
                      mtr2,
                      ref(MulH),
                      ref(MulV),
                      rows_count_first / 16 * 6, rows_count_first / 16 * 7, columns_count_second, rows_count_second_2,
                      ref(mtr_res));
    thread thread_3_8(last_part, mtr1,
                      mtr2,
                      ref(MulH),
                      ref(MulV),
                      rows_count_first / 16 * 7, rows_count_first / 16 * 8, columns_count_second, rows_count_second_2,
                      ref(mtr_res));
    thread thread_3_9(last_part, mtr1,
                      mtr2,
                      ref(MulH),
                      ref(MulV),
                      rows_count_first / 16 * 8, rows_count_first / 16 * 9, columns_count_second, rows_count_second_2,
                      ref(mtr_res));
    thread thread_3_10(last_part, mtr1,
                       mtr2,
                       ref(MulH),
                       ref(MulV),
                       rows_count_first / 16 * 9, rows_count_first / 16 * 10, columns_count_second, rows_count_second_2,
                       ref(mtr_res));
    thread thread_3_11(last_part, mtr1,
                       mtr2,
                       ref(MulH),
                       ref(MulV),
                       rows_count_first / 16 * 10, rows_count_first / 16 * 11, columns_count_second, rows_count_second_2,
                       ref(mtr_res));
    thread thread_3_12(last_part, mtr1,
                       mtr2,
                       ref(MulH),
                       ref(MulV),
                       rows_count_first / 16 * 11, rows_count_first / 16 * 12, columns_count_second, rows_count_second_2,
                       ref(mtr_res));
    thread thread_3_13(last_part, mtr1,
                       mtr2,
                       ref(MulH),
                       ref(MulV),
                       rows_count_first / 16 * 12, rows_count_first / 16 * 13, columns_count_second, rows_count_second_2,
                       ref(mtr_res));
    thread thread_3_14(last_part, mtr1,
                       mtr2,
                       ref(MulH),
                       ref(MulV),
                       rows_count_first / 16 * 13, rows_count_first / 16 * 14, columns_count_second, rows_count_second_2,
                       ref(mtr_res));
    thread thread_3_15(last_part, mtr1,
                       mtr2,
                       ref(MulH),
                       ref(MulV),
                       rows_count_first / 16 * 14, rows_count_first / 16 * 15, columns_count_second, rows_count_second_2,
                       ref(mtr_res));
    thread thread_3_16(last_part, mtr1,
                       mtr2,
                       ref(MulH),
                       ref(MulV),
                       rows_count_first / 16 * 15, rows_count_first, columns_count_second, rows_count_second_2,
                       ref(mtr_res));

    thread_3_1.join();
    thread_3_2.join();
    thread_3_3.join();
    thread_3_4.join();
    thread_3_5.join();
    thread_3_6.join();
    thread_3_7.join();
    thread_3_8.join();
    thread_3_9.join();
    thread_3_10.join();
    thread_3_11.join();
    thread_3_12.join();
    thread_3_13.join();
    thread_3_14.join();
    thread_3_15.join();
    thread_3_16.join();
}

int multi8(Matrix mtr1, Matrix mtr2, Matrix &mtr_res)
{
    if (mtr1.columns_count != mtr2.rows_count)
    {
        return CANT_MULTIPLY;
    }

    int rows_count_first = mtr1.rows_count;
    int rows_count_second_2 = mtr2.rows_count / 2;
    int columns_count_second = mtr2.columns_count;

    std::vector<int> MulH(mtr1.rows_count);
    std::vector<int> MulV(mtr2.columns_count);

    thread thread_1_1(first_part, mtr1, 0,
                      rows_count_first / 4,
                      rows_count_second_2,
                      ref(MulH));

    thread thread_1_2(first_part, mtr1, rows_count_first / 4,
                      rows_count_first / 4 * 2,
                      rows_count_second_2,
                      ref(MulH));

    thread thread_1_3(first_part, mtr1, rows_count_first / 4 * 2,
                      rows_count_first / 4 * 3,
                      rows_count_second_2,
                      ref(MulH));

    thread thread_1_4(first_part, mtr1, rows_count_first / 4 * 3,
                      rows_count_first,
                      rows_count_second_2,
                      ref(MulH));

    thread thread_2_1(second_part, mtr1, 0,
                      columns_count_second / 4,
                      rows_count_second_2,
                      ref(MulV));

    thread thread_2_2(second_part, mtr2, columns_count_second / 4,
                      columns_count_second / 4 * 2,
                      rows_count_second_2,
                      ref(MulV));

    thread thread_2_3(second_part, mtr2, columns_count_second / 4 * 2,
                      columns_count_second / 4 * 3,
                      rows_count_second_2,
                      ref(MulV));

    thread thread_2_4(second_part, mtr2, columns_count_second / 4 * 3,
                      columns_count_second,
                      rows_count_second_2,
                      ref(MulV));



    if (thread_1_1.joinable() && thread_2_1.joinable())
    {
        thread_1_1.join();
        thread_1_2.join();
        thread_1_3.join();
        thread_1_4.join();
        thread_2_1.join();
        thread_2_2.join();
        thread_2_3.join();
        thread_2_4.join();
    }

    thread thread_3_1(last_part, mtr1,
                      mtr2,
                      ref(MulH),
                      ref(MulV),
                      0, rows_count_first / 8, columns_count_second, rows_count_second_2,
                      ref(mtr_res));
    thread thread_3_2(last_part, mtr1,
                      mtr2,
                      ref(MulH),
                      ref(MulV),
                      rows_count_first / 8, rows_count_first / 8 * 2, columns_count_second, rows_count_second_2,
                      ref(mtr_res));
    thread thread_3_3(last_part, mtr1,
                      mtr2,
                      ref(MulH),
                      ref(MulV),
                      rows_count_first / 8 * 2, rows_count_first / 8 * 3, columns_count_second, rows_count_second_2,
                      ref(mtr_res));
    thread thread_3_4(last_part, mtr1,
                      mtr2,
                      ref(MulH),
                      ref(MulV),
                      rows_count_first / 8 * 3, rows_count_first / 8 * 4, columns_count_second, rows_count_second_2,
                      ref(mtr_res));
    thread thread_3_5(last_part, mtr1,
                      mtr2,
                      ref(MulH),
                      ref(MulV),
                      rows_count_first / 8 * 4, rows_count_first / 8 * 5, columns_count_second, rows_count_second_2,
                      ref(mtr_res));
    thread thread_3_6(last_part, mtr1,
                      mtr2,
                      ref(MulH),
                      ref(MulV),
                      rows_count_first / 8 * 5, rows_count_first / 8 * 6, columns_count_second, rows_count_second_2,
                      ref(mtr_res));
    thread thread_3_7(last_part, mtr1,
                      mtr2,
                      ref(MulH),
                      ref(MulV),
                      rows_count_first / 8 * 6, rows_count_first / 8 * 7, columns_count_second, rows_count_second_2,
                      ref(mtr_res));
    thread thread_3_8(last_part, mtr1,
                      mtr2,
                      ref(MulH),
                      ref(MulV),
                      rows_count_first / 8 * 7, rows_count_first, columns_count_second, rows_count_second_2,
                      ref(mtr_res));


    thread_3_1.join();
    thread_3_2.join();
    thread_3_3.join();
    thread_3_4.join();
    thread_3_5.join();
    thread_3_6.join();
    thread_3_7.join();
    thread_3_8.join();
}

int multi4(Matrix mtr1, Matrix mtr2, Matrix &mtr_res)
{
    if (mtr1.columns_count != mtr2.rows_count)
    {
        return CANT_MULTIPLY;
    }

    int rows_count_first = mtr1.rows_count;
    int rows_count_second_2 = mtr2.rows_count / 2;
    int columns_count_second = mtr2.columns_count;

    std::vector<int> MulH(mtr1.rows_count);
    std::vector<int> MulV(mtr2.columns_count);

    thread thread_1_1(first_part, mtr1, 0,
                      rows_count_first / 2,
                      rows_count_second_2,
                      ref(MulH));

    thread thread_1_2(first_part, mtr1, rows_count_first / 2,
                      rows_count_first,
                      rows_count_second_2,
                      ref(MulH));

    thread thread_2_1(second_part, mtr1, 0,
                      columns_count_second / 2,
                      rows_count_second_2,
                      ref(MulV));
    thread thread_2_2(second_part, mtr2, columns_count_second / 2,
                      columns_count_second,
                      rows_count_second_2,
                      ref(MulV));



    if (thread_1_1.joinable() && thread_2_1.joinable())
    {
        thread_1_1.join();
        thread_1_2.join();
        thread_2_1.join();
        thread_2_2.join();
    }

    thread thread_3_1(last_part, mtr1,
                      mtr2,
                      ref(MulH),
                      ref(MulV),
                      0, rows_count_first / 4, columns_count_second, rows_count_second_2,
                      ref(mtr_res));
    thread thread_3_2(last_part, mtr1,
                      mtr2,
                      ref(MulH),
                      ref(MulV),
                      rows_count_first / 4, rows_count_first / 2, columns_count_second, rows_count_second_2,
                      ref(mtr_res));
    thread thread_3_3(last_part, mtr1,
                      mtr2,
                      ref(MulH),
                      ref(MulV),
                      rows_count_first / 2, rows_count_first / 4 * 3, columns_count_second, rows_count_second_2,
                      ref(mtr_res));
    thread thread_3_4(last_part, mtr1,
                      mtr2,
                      ref(MulH),
                      ref(MulV),
                      rows_count_first / 4 * 3, rows_count_first, columns_count_second, rows_count_second_2,
                      ref(mtr_res));
    thread_3_1.join();
    thread_3_2.join();
    thread_3_3.join();
    thread_3_4.join();

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

    thread thread_1_1(first_part, mtr1, 0,
                      rows_count_first,
                      rows_count_second_2,
                      ref(MulH));
    thread thread_2_1(second_part, mtr2, 0,
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
                      ref(MulV), 0,
                      rows_count_first / 2,
                      columns_count_second,
                      rows_count_second_2,
                      ref(mtr_res));
    thread thread_3_2(last_part, mtr1,
                      mtr2,
                      ref(MulH),
                      ref(MulV), rows_count_first / 2,
                      rows_count_first,
                      columns_count_second,
                      rows_count_second_2,
                      ref(mtr_res));
    thread_3_1.join();
    thread_3_2.join();
}



int main()
{
    //Matrix mtr1(2, 2);
    //createMatrix(mtr1);
    //printMatrix(mtr1.matrix, mtr1.rows_count, mtr1.columns_count);
    //Matrix mtr2(2, 2);
    //createMatrix(mtr2);
    //printMatrix(mtr2.matrix, mtr2.rows_count, mtr2.columns_count);
    //Matrix mtr3;
    //multi16(mtr1, mtr2, mtr3);
    //printMatrix(mtr3.matrix, mtr3.rows_count, mtr3.columns_count);
    int start = 100;
    int end = 500;
    int step = 100;
    int rep = 10;

    unsigned long long tSum = 0;

    for (int i = start; i <= end; i += step)
    {
        cout << endl;
        cout << "===== for " << i << " size ========" << endl;
        cout << endl;
        {
            Matrix mtr1(i, i);
            createMatrix(mtr1);
            //printMatrix(mtr1.matrix, mtr1.rows_count, mtr1.columns_count);
            Matrix mtr2(i, i);
            createMatrix(mtr2);
            Matrix mtr3;
            //printMatrix(mtr2.matrix, mtr2.rows_count, mtr2.columns_count);
            tSum = 0;
            for (int j = 0; j < rep; j++)
            {
                const unsigned long long start = tick();
                multi2(mtr1, mtr2, mtr3);
                const unsigned long long end = tick();
                const unsigned long long res = (end - start);
                tSum += res;
            }
            tSum /= rep;
            cout << "multi2 " << tSum << endl;
        }
        tSum = 0;
        {
            Matrix mtr1(i, i);
            createMatrix(mtr1);
            //printMatrix(mtr1.matrix, mtr1.rows_count, mtr1.columns_count);
            Matrix mtr2(i, i);
            createMatrix(mtr2);
            Matrix mtr3;
            //printMatrix(mtr2.matrix, mtr2.rows_count, mtr2.columns_count);
            tSum = 0;
            for (int j = 0; j < rep; j++)
            {
                const unsigned long long start = tick();
                multi4(mtr1, mtr2, mtr3);
                const unsigned long long end = tick();
                const unsigned long long res = (end - start);
                tSum += res;
            }
            tSum /= rep;
            cout << "multi4 " << tSum << endl;
        }
        tSum = 0;
        {
            Matrix mtr1(i, i);
            createMatrix(mtr1);
            //printMatrix(mtr1.matrix, mtr1.rows_count, mtr1.columns_count);
            Matrix mtr2(i, i);
            createMatrix(mtr2);
            Matrix mtr3;
            //printMatrix(mtr2.matrix, mtr2.rows_count, mtr2.columns_count);
            tSum = 0;
            for (int j = 0; j < rep; j++)
            {
                const unsigned long long start = tick();
                multi8(mtr1, mtr2, mtr3);
                const unsigned long long end = tick();
                const unsigned long long res = (end - start);
                tSum += res;
            }
            tSum /= rep;
            cout << "multi8 " << tSum << endl;
        }
        tSum = 0;
        {
            Matrix mtr1(i, i);
            createMatrix(mtr1);
            //printMatrix(mtr1.matrix, mtr1.rows_count, mtr1.columns_count);
            Matrix mtr2(i, i);
            createMatrix(mtr2);
            Matrix mtr3;
            //printMatrix(mtr2.matrix, mtr2.rows_count, mtr2.columns_count);
            tSum = 0;
            for (int j = 0; j < rep; j++)
            {
                unsigned long long start = tick();
                multi16(mtr1, mtr2, mtr3);
                unsigned long long end = tick();
                const unsigned long long res = (end - start);
                tSum += res;
            }
            tSum /= rep;
            cout << "multi16 " << tSum << endl;
        }

    }


    return 0;
}
