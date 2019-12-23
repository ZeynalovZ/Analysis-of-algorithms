#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <chrono>
using namespace std;


# define NO_OF_CHARS 256

int baseAlgo(const string substr, const string str)
{
    unsigned int strLen = str.size();
    unsigned int substrLen = substr.size();
    int index = -1;
    if (strLen >= substrLen)
    {
        int count = 0;
        for (int i = 0; i < strLen; i++)
        {
            count = 0;
            for (int j = 0; j < substrLen; j++)
            {
                if (str[i + j] == substr[j])
                {
                    count++;
                }
            }
            if (count == substrLen)
            {
                index = i;
                break;
            }
        }
    }
    return index;
}


vector<size_t> prefix_function (string s)
{
    size_t n =  s.length();
    vector<size_t> pi(n); // в i-м элементе (его индекс i-1) количество совпавших символов в начале и конце для подстроки длины i.
    // p[0]=0 всегда, p[1]=1, если начинается с двух одинаковых
    for (size_t i = 1; i < n; ++i)
    {
        // ищем, какой префикс-суффикс можно расширить
        size_t j = pi[i - 1]; // длина предыдущего префикса-суффикса, возможно нулевая
        while ((j > 0) && (s[i] != s[j])) // этот нельзя расширить,
            j = pi[j - 1];   // берем длину меньшего префикса-суффикса

        if (s[i] == s[j])
            ++j;  // расширяем найденный (возможно пустой) префикс-суффикс
        pi[i] = j;
    }
    return pi;
}


int KMP(const string substr, const string str)
{
    vector<size_t> v = prefix_function(str);
    int result = -1;
    int i = 0;
    int j = 0;
    while (i < str.length())
    {
        if (substr[j] == str[i])
        {
            j++;
            i++;
        }
        if (j == substr.length())
        {
            result = i - j;
            j = v[j - 1];
        }
        else if (i < str.length() && substr[j] != str[i])
        {
            if (j != 0)
            {
                j = v[j - 1];
            }
            else
            {
                i++;
            }
        }
    }
    return result;
}



// A utility function to get maximum of two integers
int max(int a, int b)
{
    return (a > b) ? a : b;
}

// The preprocessing function for Boyer Moore's bad character heuristic
void badCharHeuristic(string str, int badchar[NO_OF_CHARS])
{
    int i;
    int size = str.length();
    // Initialize all occurrences as -1
    for (i = 0; i < NO_OF_CHARS; i++)
        badchar[i] = -1;

    // Fill the actual value of last occurrence of a character
    for (i = 0; i < size; i++)
        badchar[(int) str[i]] = i;
}

int BM(const string pat, const string txt)
{
    int m = pat.length();
    int n = txt.length();
    int badchar[NO_OF_CHARS];
    badCharHeuristic(pat, badchar);
    int s = 0; // s is shift of the pattern with respect to text
    while (s <= (n - m))
    {
        int j = m - 1;
        while (j >= 0 && pat[j] == txt[s + j])
            j--;

        if (j < 0)
        {
            return s;
            s += (s + m < n) ? m - badchar[txt[s + m]] : 1;
        }
        else
            s += max(1, j - badchar[txt[s + j]]);
    }
}



std::map<char, size_t> get_shift(const std::string &substr)
{
    size_t alphabet_size = 256; // словарь символов
    auto sub_size = substr.length(); // длина строки

    std::map<char, size_t> shift;

    for (size_t symb = 0; symb < alphabet_size; ++symb) // заполняем словарь алфавитом
        shift[static_cast<char>(symb)] = sub_size;

    for (size_t symb = 0; symb < sub_size-1; ++symb) // изменяем значения для символов из шаблона
        shift[static_cast<char>(substr[symb])] = sub_size - symb - 1;

    return shift;
}

int bm(std::string substr, std::string str)
{
    auto str_len = str.length();
    auto sub_len = substr.length();

    if (str_len < sub_len)
        return -1;

    auto shift = get_shift(substr);

    int start = sub_len - 1; // последний символ искомой подстроки
    int i = start;
    int j = i;
    int k = i;

    while (j >= 0 && i < str_len)
    {
        j = start;
        k = i;

        while (j >= 0 && str[k] == substr[j]) { --k; --j;}

        i += shift[str[i]];
    }
    return k >= str_len-sub_len ? -1 : k+1;
}

int main()
{
    string str = "aaaabb";
    string substr = "abab";
    cout.width(20);
    cout << "s:" << str << endl;
    cout.width(20);
    cout << "substr:" << substr << endl;
    int index1 = baseAlgo(substr, str);
    int index2 = KMP(substr, str);
    int index3 = bm(substr, str);
    cout.width(15);
    cout << "index Base: " << index1 << endl;
    cout.width(15);
    cout << "index KMP:  " << index2 << endl;
    cout.width(15);
    cout << "index BM:  " << index3 << endl;

//    for (int i = 0; i < 50; i++)
//    {
//        auto start =
//    }
//    auto t1 = chrono::duration_cast<chrono::duration<double>>(end - start).count() / 10.0;
//    auto start = chrono::high_resolution_clock::now();
//    for (auto i = 0; i < 10; i++)
//    {
//    }
//    auto end = chrono::high_resolution_clock::now();
//    auto t2 = chrono::duration_cast<chrono::duration<double>>(end - start).count() / 10.0;
    return 0;
}
