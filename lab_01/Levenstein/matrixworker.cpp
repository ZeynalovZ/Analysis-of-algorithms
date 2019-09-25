#include "matrixworker.h"
int** allocateMatrix(int n, int m)
{
    int **matrix = new int*[n + 1];
    for (int i = 0; i < n + 1; i++)
    {
        matrix[i] = new int[m + 1];
        for (int j = 0; j < m + 1; j++)
        {
            matrix[i][j] = 0;
            //cout << matrix[i][j] << " ";
        }
        //cout << endl;
    }
    return matrix;
}

void clearMatrix(int **matrix, int row_count)
{
    for (int i = 0; i < row_count; i++)
        delete matrix[i];
    delete matrix;
}

int minimum(int d1, int d2, int d3)
{
    if (d1 <= d2 && d1 <= d3)
        return d1;
    if (d2 <= d3 && d2 <= d1)
        return d2;
    if (d3 <= d2 && d3 <= d1)
        return d3;
    return -1;
}

// Заполняет первую строку и первый столбец
// s1 and s2 - count of symbols in strings
void fillBaseMatrix(int s1, int s2, int **matrix)
{
    for (int i = 1; i < s1 + 1; i++)
    {
        matrix[i][0] = i;
    }
    for (int j = 1; j < s2 + 1; j++)
    {
        matrix[0][j] = j;
    }
}

void printMatrix(int row_count, int column_count, int **matrix)
{
    for (int i = 0; i < row_count + 1; i++)
    {
        for (int j = 0; j < column_count + 1; j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}
