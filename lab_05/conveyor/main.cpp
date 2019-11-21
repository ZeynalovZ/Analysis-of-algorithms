#include <iostream>
#include <mutex>
#include <thread>
#include <queue>
#include <ctime>
#include <windows.h>
#include <fstream>
#include <mutex>

#define N 100
using namespace std;
static mutex mtx1, mtx2, mtx3, resmtx;



struct inputObject
{
    int index;
    int itemindex;
    time_t time;
    string str = " time is ";
};

static queue <inputObject> qFirst;
static queue <inputObject> qSecond;
static queue <inputObject> qThird;

static std::vector<inputObject> objectPool;
static std::vector<inputObject> resultPool;

void sortOutput()
{
    inputObject temp;
    for (int i = 0; i < resultPool.size() - 1; i++) {
        for (int j = 0; j < resultPool.size() - i - 1; j++) {
            if (resultPool[j].time > resultPool[j + 1].time)
            {
                // меняем элементы местами
                temp = resultPool[j];
                resultPool[j] = resultPool[j + 1];
                resultPool[j + 1] = temp;
            }
        }
    }

}

void fiilObjectPool(int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        inputObject obj;
        time_t t;
        time(&t);
        obj.time = t;
        obj.itemindex = i;
        objectPool.push_back(obj);
    }
    inputObject obj;
    time_t t;
    time(&t);
    obj.time = t;
    obj.itemindex = -1;
    objectPool.push_back(obj);
}

void firstStep(void)
{
    bool finish = false;
    while (!finish)
    {
        mtx1.lock();
        if(!qFirst.empty())
        {
            inputObject obj = qFirst.front();
            if (obj.itemindex != -1)
            {
                time(&obj.time);
                //std::cout << " 1 " << obj.str << " " << ctime(&obj.time) << " item index: " << obj.itemindex << endl;
                obj.index = 1;
                obj.str = " == start: ";
                resmtx.lock();
                resultPool.push_back(obj);
                resmtx.unlock();
                qFirst.pop();
                Sleep(1000);
                mtx2.lock();
                qSecond.push(obj);
                mtx2.unlock();
                obj.str = " == end: ";
                resmtx.lock();
                resultPool.push_back(obj);
                resmtx.unlock();

            }
            else
            {
                mtx2.lock();
                qSecond.push(obj);
                mtx2.unlock();
                finish = true;
            }
        }
        mtx1.unlock();
    }
}

void secondStep(void)
{
    bool finish = false;
    while (!finish)
    {
        mtx2.lock();
        if (!qSecond.empty())
        {
            inputObject obj = qSecond.front();
            if (obj.itemindex != -1)
            {
                time(&obj.time);
                obj.index = 2;
                obj.str = " == start: ";
                resmtx.lock();
                resultPool.push_back(obj);
                resmtx.unlock();
                qSecond.pop();
                Sleep(1000);
                mtx3.lock();
                qThird.push(obj);
                mtx3.unlock();
                obj.str = " == end: ";
                resmtx.lock();
                resultPool.push_back(obj);
                resmtx.unlock();
            }
            else
            {
                mtx3.lock();
                qThird.push(obj);
                mtx3.unlock();
                finish = true;
            }
        }
        mtx2.unlock();
    }
}

void thirdStep(void)
{
    bool finish = false;
    while (!finish)
    {
        mtx3.lock();
        if (!qThird.empty())
        {
            inputObject obj = qThird.front();
            if (obj.itemindex != -1)
            {
                time(&obj.time);
                obj.index = 3;
                obj.str = " == start: ";
                resmtx.lock();
                resultPool.push_back(obj);
                resmtx.unlock();
                qThird.pop();
                Sleep(1000);
                obj.str = " == end: ";
                resmtx.lock();
                resultPool.push_back(obj);
                resmtx.unlock();
                //std::cout << c.resultPool.size() << " size" << endl;
            }
            else
            {
                finish = true;
            }

        }
        mtx3.unlock();
    }
}

int main()
{
    thread tr1 (firstStep);
    thread tr2 (secondStep);
    thread tr3 (thirdStep);
    fiilObjectPool(1);
    for (int i = 0; i < objectPool.size(); i++)
    {
        mtx1.lock();
        qFirst.push(objectPool[i]);
        mtx1.unlock();
        Sleep(50);
    }
    std::cout << objectPool.size() << " is isize" << endl;
    tr1.join();
    tr2.join();
    tr3.join();
    sortOutput();
    ofstream file;
    file.open("results.txt");
    for (int i = 0; i < resultPool.size(); i++)
    {
        file << i << " " << resultPool[i].index << resultPool[i].str << ctime(&(resultPool[i].time));
    }
    file.close();
    std::cout << "all products were pushed into ResultObjectPool";
    return 0;
}


