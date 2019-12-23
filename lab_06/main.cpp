#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <string>
#include <iomanip>
#include <chrono>
#include <matrix.h>

using namespace std;

#define ALPHA_BETTA_CONST 1

const double teta_start = 10;
const double tmin = 5;

static int s(0);
void hamilton(const Matrix<int> &distances, vector<int> &min_path, int &min_distance, vector<int> &cur_path, vector<bool> &visited, int &cur_len)
{
    if (cur_path.size() == distances.n)
    {
        s++;
        int tmp = distances[cur_path.back()][cur_path[0]];
        if (cur_len + tmp < min_distance)
        {
            min_path = cur_path;
            min_distance = cur_len + tmp;
        }
        return;
    }
    for (int i = 0; i < distances.n; i++)
    {
        if (!visited[i])
        {
            int tmp = distances[cur_path.back()][i];
            if (cur_len + tmp > min_distance)
                continue;
            cur_len += tmp;
            cur_path.push_back(i);
            visited[i] = 1;
            hamilton(distances, min_path, min_distance, cur_path, visited, cur_len);
            visited[i] = 0;
            cur_path.pop_back();
            cur_len -= tmp;
        }
    }
}

pair<int, vector<int> > brute_force(const Matrix<int> &distances)
{
    int n = distances.n;
    vector<bool> visited(n, 0);
    vector<int> cur_path;
    vector<int> min_path;
    int cur_len = 0;
    int min_path_len = INT_MAX;
    for (int i = 0; i < n; i++)
    {
        cur_path.clear();
        cur_path.push_back(i);
        fill(visited.begin(), visited.end(), 0);
        visited[i] = 1;
        cur_len = 0;
        hamilton(distances, min_path, min_path_len, cur_path, visited, cur_len);
    }
    //cout << "brute force method found " << s << " routes" << endl;
    return pair<int, vector<int>>(min_path_len, min_path);
}



void findNotVisited(vector<int> &dst, const vector<int> &src, int count)
{
    bool exist = false;
    for (int i = 0; i < count; i++)
    {
        exist = false;
        for (int j = 0; j < src.size(); j++)
        {
            if (src[j] == i)
            {
                exist = true;
                break;
            }
        }
        if (!exist)
        {
            dst.push_back(i);
        }
    }
}

int getIndexOutOfValue(const vector<int> src, int value)
{
    for (int i = 0; i < src.size(); i++)
    {
        if (src[i] == value)
        {
            return i;
        }
    }
    return 0;
}

double rand_double(void)
{
    int tmp = rand() % 100;
    return (tmp / 100.0);
}

pair<int, vector<int>> aco(Matrix<int> towns, const int &tMax, const double &ro, const double alpha)
{
    int townsCount = towns.n;
    double Q = towns.compute_avg() * townsCount;
    double beta = ALPHA_BETTA_CONST - alpha;

    // Это значения "ЭТА"
    Matrix<int> townAttraction(townsCount);
    townAttraction.reverse_values(towns);

    Matrix<int> teta(towns);
    teta.fill_value(teta_start);

    Matrix<int> deltaTeta(towns);

    vector<double> probabilities(townsCount, 0);

    vector<int> resPath;
    int min_path_length = -1;
    vector<int> min_path;

    double coin;
    for (int t = 0; t < tMax; t++)
    {
        deltaTeta.zero();
        for (int k = 0; k < townsCount; k++)
        {
            vector<int> currentPath = {k};
            int currentPathLen = 0;
            int i = k;
            while (currentPath.size() != townsCount)
            {
                vector<int> notVisited;
                findNotVisited(notVisited, currentPath, townsCount);
                fill(probabilities.begin(), probabilities.end(), 0.0);
                for (auto j : notVisited)
                {
                    int currentIndex = getIndexOutOfValue(notVisited, i);
                    if (towns[i][j] != 0)
                    {
                        double sum = 0;
                        for (auto n : notVisited)
                        {
                            sum += pow(teta[i][n], alpha) * pow(townAttraction[i][n], beta);
                        }
                        probabilities[currentIndex] = pow(teta[i][j], alpha) * pow(townAttraction[i][j], beta) / sum;
                    }
                    else
                    {
                        probabilities[currentIndex] = 0;
                    }
                }
                coin = rand_double();
                unsigned int best_p = 0;
                double sum_p = 0;
                for (unsigned int s = 0; s < townsCount; s++)
                {
                    sum_p += probabilities[s];
                    if (coin < sum_p)
                    {
                        best_p = s;
                        break;
                    }
                }
                int add = notVisited[best_p];
                currentPath.push_back(add);
                currentPathLen += towns[i][add];
                deltaTeta[i][add] += Q / currentPathLen;
                i = add;
                notVisited.erase(notVisited.begin() + best_p);
            }
            currentPathLen += towns[currentPath[currentPath.size() - 1]][currentPath[0]];
            if (min_path_length == -1 || (currentPathLen < min_path_length))
            {

                min_path_length = currentPathLen;
                min_path = currentPath;
            }
        }
        teta *= (1.0 - ro);
        teta += deltaTeta;
        teta.check_zero(tmin);
    }
    return pair<int, vector<int>>(min_path_length, min_path);
}


int main()
{
    srand(time(0));
    //system("chcp 65001");
    //    Matrix<int> mtr1(5);
    //    mtr1.zero();
    //    mtr1.fill_int_random();
    //    mtr1.print();
    //    vector<int> current = {7, 8, 9};
    //    vector<int> res;
    //    findNotVisited(res, current, 10);
    //    for (auto j : res)
    //    {
    //        cout << j << " ";
    //    }
    Matrix<int> mtr1(10);
    mtr1.fill_int_random();
    mtr1.print();
    //cout << "je";
//    pair<int, vector<int>> results = aco(mtr1, 10, 0.3, 0.5);
//    cout << "aco results: " << results.first << " length" << endl;
//    for (auto ptr = results.second.begin(); ptr != results.second.end(); ptr++)
//        cout << *ptr << " ";
//    cout << endl;
    pair<int, vector<int>> brute_results = brute_force(mtr1);
    cout << "brute_force results: " << brute_results.first << " length" << endl;
    for (auto ptr = brute_results.second.begin(); ptr != brute_results.second.end(); ptr++)
        cout << *ptr << " ";
    cout << endl;
    return 0;
}
