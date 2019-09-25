#include <iostream>
#include <string.h>
#include <algorithm>
#include "algorithms.h"
using namespace std;
int main()
{
    cout << "Input 2 strings to get min distance" << endl;
    cout << "Input first string" << endl;
    string str1 = "";
    string str2 = "";
    cin >> str1;
    cout << "Okeeeeey! Now input second string" << endl;
    cin >> str2;
    cout << "let's see the answer" << endl;
    int distanceLM = LevensteinMatrix(str1, str2);
    cout << endl;
    int distanceDLR = DamerauLevensteinRecursion(str1, str2);
    cout << endl;
    int distanceDLM = DamerauLevinsteinMatrix(str1, str2);
    cout << endl;
    cout << "Result of Levenstein : " << distanceLM << endl;
    cout << "Result of Damerau_Levenstein_Matrix : " << distanceDLM << endl;
    cout << "Result Damerau_Levenstein_recurs : " << distanceDLR << endl;
    return 0;
}
