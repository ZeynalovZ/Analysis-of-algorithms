#ifndef MATRIXWORKER_H
#define MATRIXWORKER_H
#include <iostream>
using namespace std;
int** allocateMatrix(int n, int m);
void clearMatrix(int **matrix, int row_count);
int minimum(int d1, int d2, int d3);
void fillBaseMatrix(int s1, int s2, int **matrix);
void printMatrix(int row_count, int column_count, int **matrix);
int minimum4(int d1, int d2, int d3, int d4);
#endif // MATRIXWORKER_H
