#include <iostream>
#include <vector>
#include <chrono>
#include <time.h>
using namespace std;

class arraySort
{
public:

    arraySort()
    {
        arraySize = 0;
    }

    arraySort(size_t size)
    {
        arraySize = size;
    }

    bool isEmpty();

    void printArray();

    void fillArray(size_t count);

    void fillIncreasing(size_t count);
    void fillDecreasing(size_t count);

    void insertionSort();

    void getArrayFromUser();

    void quickSort(size_t first, size_t last);

    void GnomeSort(size_t i, size_t j);

    size_t arraySize;
    std::vector<int> Array;

};

bool arraySort::isEmpty()
{
    if (this->arraySize == 0)
    {
        return true;
    }
    return false;
}

void arraySort::printArray()
{
    if (this->isEmpty())
    {
        cout << "Array is empty" << endl;
    }
    else
    {
        for (size_t i = 0; i < this->arraySize; i++)
        {
            cout << this->Array[i] << " ";
        }
        cout << endl;
    }
}

void arraySort::fillArray(size_t count)
{
    arraySize = count;
    // arrayCreation
    for (size_t i = 0; i < count; i++)
    {
        Array.push_back(rand() % 100 + 1);
    }
}

void arraySort::fillIncreasing(size_t count)
{
    arraySize = count;
    // arrayCreation
    for (size_t i = 0; i < count; i++)
    {
        Array.push_back(i);
    }
}

void arraySort::fillDecreasing(size_t count)
{
    arraySize = count;
    // arrayCreation
    for (size_t i = count; i > 0; i--)
    {
        Array.push_back(i);
    }
}

void arraySort::insertionSort()
{
    for (size_t i = 0; i < this->arraySize - 1; i++)
    {
        int key = i + 1;
        int tmp = this->Array[key];
        size_t j = i + 1;
        while (j > 0 && tmp < this->Array[j - 1])
        {
            this->Array[j] = this->Array[j - 1];
            key = j - 1;
            j = j - 1;
        }
        this->Array[key] = tmp;
    }
}

void arraySort::getArrayFromUser()
{
    cout << "Enter nums: " << endl;
    for (size_t i = 0; i < this->arraySize; i++)
    {
        int a;
        cin >> a;
        this->Array.push_back(a);
    }
}

void arraySort::quickSort(size_t first, size_t last)
{
    if (first + 1 < last) // wtfd
    {
        int mid, tmp;
        size_t f = first, l = last;
        mid = this->Array[(f + l) / 2]; //вычисление опорного элемента
        do
        {
            while (this->Array[f] < mid && f <= last)
                f++;
            while (this->Array[l] > mid && l >= first)
                l--;
            if (f <= l) //перестановка элементов
            {
                tmp = this->Array[f];
                this->Array[f] = this->Array[l];
                this->Array[l] = tmp;
                f++;
                l--;
            }
        } while (f <= l);
        if (first < l) quickSort(first, l);
        if (f < last) quickSort(f, last);
    }
}

void arraySort::GnomeSort(size_t i, size_t j)
{
    if (i > this->arraySize || j > this->arraySize)
    {
        return;
    }
    while (i < this->arraySize)
    {
        if (this->Array[i - 1] <= this->Array[i])
        {
            i = j;
            j++;
        }
        else
        {
            int t = this->Array[i];
            this->Array[i] = this->Array[i - 1];
            this->Array[i - 1] = t;
            i--;
            if (i == 0)
            {
                i = j;
                j++;
            }
        }
    }
}

unsigned long long tick(void)
{
    unsigned long long d;
    __asm__ __volatile__ ("rdtsc" : "=A" (d) );

    return d;
}

void time()
{
    double sum = 0;
    int repeat = 50;
    int begin = 100;
    int step = 100;
    int last = 1000;
    for (int j = begin; j <= last; j += step)
    {
        cout << "==============for size " << j << " ==========" <<endl;
        {
            arraySort array, arrayTmp;
            array.fillDecreasing(j);
            arrayTmp.arraySize = array.arraySize;
            arrayTmp.Array = array.Array;
            for (int i = 0; i < repeat; i++)
            {
                const auto start = tick();
                array.insertionSort();
                const auto end = tick();
                const auto res = end - start;
                sum += res;
                array.Array = arrayTmp.Array;
            }
            sum = sum / repeat;
            cout << "===overall insertion is " << sum << endl;
        }
        sum = 0;
        {
            arraySort array, arrayTmp;
            array.fillDecreasing(j);
            arrayTmp.arraySize = array.arraySize;
            arrayTmp.Array = array.Array;
            for (int i = 0; i < repeat; i++)
            {
                const auto start = tick();
                array.quickSort(0, array.arraySize);
                const auto end = tick();
                const auto res = end - start;
                sum += res;
                array.Array = arrayTmp.Array;
            }
            sum = sum / repeat;
            cout << "===overall quick is " << sum << endl;
        }
        sum = 0;
        {
            arraySort array, arrayTmp;
            array.fillDecreasing(j);
            arrayTmp.arraySize = array.arraySize;
            arrayTmp.Array = array.Array;
            for (int i = 0; i < repeat; i++)
            {
                const auto start = tick();
                array.GnomeSort(1, 2);
                const auto end = tick();
                const auto res = end - start;
                sum += res;
                array.Array = arrayTmp.Array;
            }
            sum = sum / repeat;
            cout << "===overall gnome is " << sum << endl;
        }
        cout << endl;
    }


}

void getData()
{
    arraySort array(5), array1(5), array2(5);
    array.getArrayFromUser();
    array1.Array = array.Array;
    array2.Array = array.Array;

    cout << "=====Insertion sort=====" << endl;
    array.insertionSort();
    array.printArray();

    cout << "=====Quick sort=====" << endl;
    array1.quickSort(0, array1.arraySize - 1);
    array1.printArray();

    cout << "=====Gnome sort=====" << endl;
    array2.GnomeSort(1, 2);
    array2.printArray();

}

int main()
{
    time();
    //getData();
    return 0;
}
