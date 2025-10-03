#include <iostream>
#include "futureEventsChain.h"
#include "currentEventsChain.h"
#include "simulate.h"
#include <fstream>

#define COUNT_TYPE_TRANSACT 3
#define COUNT_TYPE_WORKER 2

using namespace std;

int* get_mean_born_transact(int countTypeTransact) {
    int* meanBornTransact = new int[countTypeTransact];

    if (countTypeTransact == 3) {
        meanBornTransact[0] = 5;
        meanBornTransact[1] = 7;
        meanBornTransact[2] = 10;
    }
    else {
        cout << "Напишите среднее значения для для экспоненциального закона: ";
        for(int i = 0; i < countTypeTransact; i++)
            cin >> meanBornTransact[i];
    }
    return meanBornTransact;
}

int* get_count_workers(int countTypeWorker, bool flagDefault) {
    int* countWorkers = new int[countTypeWorker];
    if(countTypeWorker == 2) {
        if(flagDefault) {
            countWorkers[0] = 1; // Количество рабочих первого типа = 1.
            countWorkers[1] = 1; // Количество рабочих второго типа = 1.
        }
        else {
            cout << "Введите количество рабочих 1 типа" << endl;
            cin >> countWorkers[0];
            cout << "Введите количество рабочих 2 типа" << endl;
            cin >> countWorkers[1];
        }
    }
    else {
        for(int i = 0; i < countTypeWorker; i++) {
            cout << "Введите количество рабочих " << i << " типа" << endl;
            cin >> countWorkers[i];
        }
    }
    return countWorkers;
}

int** get_mean_processing_transact(int countTypeTransact, int countTypeWorker) {
    int** meanProcessingTransact = new int*[countTypeTransact];
    
    for(int i = 0; i < countTypeTransact; i++)
        meanProcessingTransact[i] = new int[countTypeWorker];
    
    if((countTypeTransact == 3) && (countTypeWorker == 2)) {
        meanProcessingTransact[0][0] = 22;
        meanProcessingTransact[0][1] = 0;
        
        meanProcessingTransact[1][0] = 0;
        meanProcessingTransact[1][1] = 17;

        meanProcessingTransact[2][0] = 32;
        meanProcessingTransact[2][1] = 35;
    }

    else {
        cout << "Введите значения для рабочего i и транзакта j" << endl;
        for(int i = 0; i < countTypeTransact; i++) {
            for(int j = 0; j < countTypeWorker; j++) {
                cout << "i = " << i << ", j = " << j << endl;
                cin >> meanProcessingTransact[i][j];
            }
        }
    }
    return meanProcessingTransact;
}

void clear(int* MNT, int* CW, int** MPT, int CTW) {
    delete[] MNT;
    delete[] CW;
    
    for(int i = 0; i < CTW; i++)
        delete[] MPT[i];
    delete[] MPT;
}



int main() {
    std::ofstream outputFile("output.txt");
    int countTypeTransact = COUNT_TYPE_TRANSACT;
    int countTypeWorker = COUNT_TYPE_WORKER;

    int* meanBornTransact = get_mean_born_transact(countTypeTransact);
    int* countWorkers = get_count_workers(countTypeWorker, 0);
    int** meanProcessingTransact = get_mean_processing_transact(countTypeTransact,countTypeWorker);

    Simulate sim(3, 2, meanBornTransact, countWorkers, meanProcessingTransact);
    sim.start(outputFile);
    clear(meanBornTransact, countWorkers, meanProcessingTransact, countTypeTransact);
    outputFile.close();
    return 0;
}