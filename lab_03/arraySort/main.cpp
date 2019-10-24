#include <iostream>
#include <vector>
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
        // arrayCreation
        for (size_t i = 0; i < arraySize; i++)
        {
            Array.push_back(rand() % 100 + 1);
        }
    }

    bool isEmpty();

    void printArray();

    void insertionSort();

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

void arraySort::quickSort(size_t first, size_t last)
{
    if (first > this->arraySize || last > this->arraySize)
    {
        return;
    }
    int mid, temp;
    size_t f = first;
    size_t l = last;
    mid = this->Array[(f + l) / 2];
    do
    {
        while (this->Array[f] < mid) f++;
        while (this->Array[l] > mid) l--;
        if (f <= l)
        {
            temp = this->Array[f];
            this->Array[f] = this->Array[l];
            this->Array[l] = temp;
            f++;
            l--;
        }
    }
    while (f < l);

    if (first < l) this->quickSort(first, l);
    if (f < last) this->quickSort(f, last);

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


void time()
{
    ;
}

int main()
{
    arraySort array(10);
    cout << "=====Insertion sort=====" << endl;
    array.printArray();
    array.insertionSort();
    cout << "after sort" << endl;
    array.printArray();

    arraySort array2(10);
    cout << "=====Quick sort=====" << endl;
    array2.printArray();
    array2.quickSort(0, array.arraySize - 1);
    cout << "after sort" << endl;
    array2.printArray();

    arraySort array3(10);
    cout << "=====Gnome sort=====" << endl;
    array3.printArray();
    array3.GnomeSort(1, 2);
    cout << "after sort" << endl;
    array3.printArray();
    return 0;
}
