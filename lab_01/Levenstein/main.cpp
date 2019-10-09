#include <iostream>
#include <string.h>
#include <algorithm>
#include "algorithms.h"
enum AlgoChoice {Recurs, Matrix};
using namespace std;
unsigned long long tick(void)
{
    unsigned long long d;
    __asm__ __volatile__ ("rdtsc" : "=A" (d) );

    return d;
}

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
        int expected_result = 4;
        int result = DamerauLevensteinMatrix(strings[0], strings[1]);
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
        int result = DamerauLevensteinMatrix(strings[2], strings[3]);
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
        int result = DamerauLevensteinMatrix(strings[4], strings[5]);
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
        int result = DamerauLevensteinMatrix(strings[6], strings[7]);
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
        int result = DamerauLevensteinMatrix(strings[8], strings[9]);
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

    cout << "\n========================Testing======================" << endl;
    string strings[10] = {"Razvlecheniya", "Uvlechenie", "Skat", "kot",
                         " ", " ", "Darek", "", "", ""};
    cout << "\n______________Testing Damerau Recurs_______________\n" << endl;
    DamerauLevensteinRTesting(strings);
    cout << "\n______________Testing Damerau Matrix_______________\n" << endl;
    DamerauLevensteinMTesting(strings);
    cout << "\n______________Testing Levenstein___________________\n" << endl;
    LevensteinMTesting(strings);
    return 0;
}

void timeTestingMatrixRecurs(string str1, string str2)
{
    unsigned int overal_int = 0;
    int distanceDLM = 0;
    for (int i = 0; i < 50; i++)
    {
        unsigned int tick1 = tick();
        distanceDLM = DamerauLevensteinMatrix(str1, str2);
        unsigned int tick2 = tick();
        overal_int += tick2 - tick1;
    }
    cout << overal_int / 50 << "====== overal Damerau Levenstein" << endl;
    cout << endl;
    overal_int = 0;
    int distanceDLR = 0;
    for (int i = 0; i < 50; i++)
    {
        unsigned int tick1 = tick();
        distanceDLR = DamerauLevensteinRecursion(str1, str2);
        unsigned int tick2 = tick();
        overal_int += tick2 - tick1;
    }
    cout << overal_int / 50 << "====== overal Damerau Levenstein Recurs" << endl;
    cout << endl;
}

void timeTestingMatrix(string str1, string str2)
{
    int distanceLM = 0;
    unsigned int overal_int = 0;
    for (int i = 0; i < 50; i++)
    {
        unsigned int tick1 = tick();
        distanceLM = LevensteinMatrix(str1, str2);
        unsigned int tick2 = tick();
        overal_int += tick2 - tick1;
    }
    cout << endl;
    cout << overal_int / 50 << "====== overal Levenstein" << endl;
    overal_int = 0;
    int distanceDLM = 0;
    for (int i = 0; i < 50; i++)
    {
        unsigned int tick1 = tick();
        distanceDLM = DamerauLevensteinMatrix(str1, str2);
        unsigned int tick2 = tick();
        overal_int += tick2 - tick1;
    }
    cout << overal_int / 50 << "====== overal Damerau Levenstein" << endl;
    cout << endl;
}

void timeTest(AlgoChoice choice)
{
    if (choice == Matrix)
    {
        int n = 500;
        int step = 100;
        string s1 = "aaaaaaaaaa"; // 10 symbols
        string s2 = "aaaaaaaaaa";
        for (int i = 0; i <= n; i += step)
        {
            // Matrix testing 0-500
            timeTestingMatrix(s1, s2);
            // because of 10 symbols j < step - 10
            for (int j = 0; j < step - 10; j++)
            {
                s1 += "a";
                s2 += "a";
            }
        }
    }
    else
    {
        int n = 10;
        int step = 1;
        string s1 = "";
        string s2 = "";
        for (int i = 0; i < n; i += step)
        {
            // Matrix testing 0-10
            timeTestingMatrixRecurs(s1, s2);
            for (int j = 0; j < step; j++)
            {
                s1 += "a";
                s2 += "a";
            }
        }
    }
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

    int distanceLM = 0;
    distanceLM = LevensteinMatrix(str1, str2);

    int distanceDLR = 0;
    distanceDLR = DamerauLevensteinRecursion(str1, str2);

    int distanceDLM = 0;
    distanceDLM = DamerauLevensteinMatrix(str1, str2);

    cout << "Result of Levenstein : " << distanceLM << endl;
    cout << "Result of Damerau_Levenstein_Matrix : " << distanceDLM << endl;
    cout << "Result Damerau_Levenstein_recurs : " << distanceDLR << endl;

    // Testing func
    //mainTest();

    // Choose Matrix or MatrixRecurs here to get different time
    AlgoChoice choice = Recurs;
    timeTest(choice);
    return 0;
}
