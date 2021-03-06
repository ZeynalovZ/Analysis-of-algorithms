#include "algorithms.h"
#include "matrixworker.h"
#include "iostream"



int LevensteinMatrix(string s1, string s2)
{
    int row_count = s1.length();
    int column_count = s2.length();
    int **matrix = allocateMatrix(row_count, column_count);
    fillBaseMatrix(s1.length(), s2.length(), matrix);
    int cost = 1;
    for (int i = 1; i < row_count + 1; i++)
    {
        for (int j = 1; j < column_count + 1; j++)
        {
            if (s1[i - 1] == s2[j - 1])
            {
                cost = 0;
            }
            else
            {
                cost = 1;
            }
            matrix[i][j] = minimum(matrix[i - 1][j] + 1, matrix[i][j - 1] + 1, matrix[i - 1][j - 1] + cost);
        }
    }
    //cout << "=== Levenstein Matrix ===" << endl;
    //printMatrix(row_count, column_count, matrix);
    int result = matrix[s1.length()][s2.length()];
    clearMatrix(matrix, row_count);
    return result;
}

int DamerauLevensteinMatrix(string s1, string s2)
{
    int row_count = s1.length();
    int column_count = s2.length();
    int **matrix = allocateMatrix(row_count, column_count);
    fillBaseMatrix(s1.length(), s2.length(), matrix);
    int cost = 1;
    for (int i = 1; i < row_count + 1; i++)
    {
        for (int j = 1; j < column_count + 1; j++)
        {
            if (s1[i - 1] == s2[j - 1])
            {
                cost = 0;
            }
            else
            {
                cost = 1;
            }
            matrix[i][j] = minimum(matrix[i - 1][j] + 1, matrix[i][j - 1] + 1, matrix[i - 1][j - 1] + cost);
            if ((i > 1 && j > 1) && s1[i - 1] == s2[j - 2] && s2[j - 1] == s1[i - 2])
            {
                matrix[i][j] = __min(matrix[i][j], matrix[i - 2][j - 2] + cost);
            }
        }
    }
    //cout << "=== Damerau Levenstein Matrix ===" << endl;
    //printMatrix(row_count, column_count, matrix);
    int result = matrix[s1.length()][s2.length()];
    clearMatrix(matrix, row_count);
    return result;
}

int DamerauLevensteinRecursion(string s1, string s2)
{
    if (s1.length() == 0)
        return s2.length();
    if (s2.length() == 0)
        return s1.length();

    int cost = 1;
    if (s1[s1.length() - 1] == s2[s2.length() - 1])
        cost = 0;
    else
        cost = 1;
    int result = minimum(DamerauLevensteinRecursion(s1.substr(0, s1.length() - 1), s2) + 1, DamerauLevensteinRecursion(s1, s2.substr(0, s2.length() - 1)) + 1,
                         DamerauLevensteinRecursion(s1.substr(0, s1.length() - 1), s2.substr(0, s2.length() - 1)) + cost);
    if (((s1.length() > 1 && s2.length() > 1) && s1[s1.length() - 1] == s2[s2.length() - 2]
         && s2[s2.length() - 1] == s1[s1.length() - 2]))
    {
        result = __min(result, DamerauLevensteinRecursion(s1.substr(0, s1.length() - 2), s2.substr(0, s2.length() - 2)) + cost);
    }
    //unsigned tick2 = tick();
    //cout << tick1 << " " << tick2 << endl;
    return result;
}
