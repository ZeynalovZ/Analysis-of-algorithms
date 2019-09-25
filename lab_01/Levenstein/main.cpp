#include <iostream>
#include <string.h>
#include <algorithm>
#include "algorithms.h"
using namespace std;

void DamerauLevensteinRTesting(string *strings)
{
    // first test
    {
        cout << " Test1:..." << endl;
        int expected_result = 5;
        int result = DamerauLevensteinRecursion(strings[0], strings[1]);
        if (expected_result == result)
        {
            cout << "   SUCCESSED" << endl;
        }
        else
        {
            cout << "   FAILED" << endl;
        }
    }
    // first test
    {
        cout << " Test2:..." << endl;
        int expected_result = 2;
        int result = DamerauLevensteinRecursion(strings[2], strings[3]);
        if (expected_result == result)
        {
            cout << "   SUCCESSED" << endl;
        }
        else
        {
            cout << "   FAILED" << endl;
        }
    }
    // third test
    {
        cout << " Test3:..." << endl;
        int expected_result = 0;
        int result = DamerauLevensteinRecursion(strings[4], strings[5]);
        if (expected_result == result)
        {
            cout << "   SUCCESSED" << endl;
        }
        else
        {
            cout << "   FAILED" << endl;
        }
    }
    // fourht test
    {
        cout << " Test4:..." << endl;
        int expected_result = 5;
        int result = DamerauLevensteinRecursion(strings[6], strings[7]);
        if (expected_result == result)
        {
            cout << "   SUCCESSED" << endl;
        }
        else
        {
            cout << "   FAILED" << endl;
        }
    }
    // fifth test
    {
        cout << " Test5:..." << endl;
        int expected_result = 0;
        int result = DamerauLevensteinRecursion(strings[8], strings[9]);
        if (expected_result == result)
        {
            cout << "   SUCCESSED" << endl;
        }
        else
        {
            cout << "   FAILED" << endl;
        }
    }
}

void DamerauLevensteinMTesting(string *strings)
{
    // first test
    {
        cout << " Test1:..." << endl;
        int expected_result = 5;
        int result = DamerauLevinsteinMatrix(strings[0], strings[1]);
        if (expected_result == result)
        {
            cout << "   SUCCESSED" << endl;
        }
        else
        {
            cout << "   FAILED" << endl;
        }
    }
    // first test
    {
        cout << " Test2:..." << endl;
        int expected_result = 2;
        int result = DamerauLevinsteinMatrix(strings[2], strings[3]);
        if (expected_result == result)
        {
            cout << "   SUCCESSED" << endl;
        }
        else
        {
            cout << "   FAILED" << endl;
        }
    }
    // third test
    {
        cout << " Test3:..." << endl;
        int expected_result = 0;
        int result = DamerauLevinsteinMatrix(strings[4], strings[5]);
        if (expected_result == result)
        {
            cout << "   SUCCESSED" << endl;
        }
        else
        {
            cout << "   FAILED" << endl;
        }
    }
    // fourht test
    {
        cout << " Test4:..." << endl;
        int expected_result = 5;
        int result = DamerauLevinsteinMatrix(strings[6], strings[7]);
        if (expected_result == result)
        {
            cout << "   SUCCESSED" << endl;
        }
        else
        {
            cout << "   FAILED" << endl;
        }
    }
    // fifth test
    {
        cout << " Test5:..." << endl;
        int expected_result = 0;
        int result = DamerauLevinsteinMatrix(strings[8], strings[9]);
        if (expected_result == result)
        {
            cout << "   SUCCESSED" << endl;
        }
        else
        {
            cout << "   FAILED" << endl;
        }
    }
}

void LevensteinMTesting(string *strings)
{
    // first test
    {
        cout << " Test1:..." << endl;
        int expected_result = 5;
        int result = LevensteinMatrix(strings[0], strings[1]);
        if (expected_result == result)
        {
            cout << "   SUCCESSED" << endl;
        }
        else
        {
            cout << "   FAILED" << endl;
        }
    }
    // first test
    {
        cout << " Test2:..." << endl;
        int expected_result = 2;
        int result = LevensteinMatrix(strings[2], strings[3]);
        if (expected_result == result)
        {
            cout << "   SUCCESSED" << endl;
        }
        else
        {
            cout << "   FAILED" << endl;
        }
    }
    // third test
    {
        cout << " Test3:..." << endl;
        int expected_result = 0;
        int result = LevensteinMatrix(strings[4], strings[5]);
        if (expected_result == result)
        {
            cout << "   SUCCESSED" << endl;
        }
        else
        {
            cout << "   FAILED" << endl;
        }
    }
    // fourht test
    {
        cout << " Test4:..." << endl;
        int expected_result = 5;
        int result = LevensteinMatrix(strings[6], strings[7]);
        if (expected_result == result)
        {
            cout << "   SUCCESSED" << endl;
        }
        else
        {
            cout << "   FAILED" << endl;
        }
    }
    // fifth test
    {
        cout << " Test5:..." << endl;
        int expected_result = 0;
        int result = LevensteinMatrix(strings[8], strings[9]);
        if (expected_result == result)
        {
            cout << "   SUCCESSED" << endl;
        }
        else
        {
            cout << "   FAILED" << endl;
        }
    }
}


int mainTest(void)
{
    string strings[10] = {"Razvlecheniya", "Uvlechenie", "Skat", "kot",
                         " ", " ", "Darek", "", "", ""};
    cout << "\n______________Testing Damerau Recurs_______________\n" << endl;
    DamerauLevensteinRTesting(strings);
    cout << "\n______________Testing Damerau Matrix_______________\n" << endl;
    DamerauLevensteinMTesting(strings);
    cout << "\n______________Testing Levenstein_______________\n" << endl;
    LevensteinMTesting(strings);
    return 0;
}


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
    cout << "\n========================Testing======================" << endl;
    mainTest();
    return 0;
}
