#pragma once

#include <iostream>
#include "currentEventsChain.h"
#include "futureEventsChain.h"
#include "generateRandom.h"
#include "storage.h"
#include <fstream>

class Simulate {
private:
    CurrentEventsChain CEC;
    FutureEventsChain FEC;
    Workers* storage;
    RandomNumber* RandNumber;
    //std::ofstream file;
    int countTypeTransact;
    int countTypeWorker;
    double avgLenQueue = 0;
    double ModelTime = 0;
    unsigned Id = 0;
    bool flagReset = false;

    int choiseTypeWorker(Transact& transact);
    void print(bool flagViewingMode);
    void print(std::ofstream& file, bool flagViewingMode);
    void printStatic(int timer);
    void printStatic(int timer, std::ofstream& file);

    void InputPhase();
    void TimeCorrectPhase();
    void ViewingPhase();

    void runTransact();
public:
    Simulate() {}
    Simulate(int countTransact, int countWorker, int* meanTransact,
             int* count_workers, int** meanProcessingTranzakt);
    ~Simulate();
    void start(std::ofstream& file);
};

int Simulate::choiseTypeWorker(Transact& transact) {
    if(transact.getTypeWorker() == -1) {
        unsigned type = transact.getType();
        if(type <= countTypeWorker) {
            transact.setTypeWorker(type);   
            return type;
        }
        unsigned minFreeWorkers = 2048;
        for(int i = 0; i < countTypeWorker; i++) {
            if(storage[i].getQueue() < minFreeWorkers) {
                minFreeWorkers = storage[i].getQueue();
                type = i + 1;
            }
        }
        transact.setTypeWorker(type);
        return type;
    }
    return transact.getTypeWorker();
}

void Simulate::InputPhase()
{
    unsigned type;
    for(int i = 0; i < countTypeTransact; i++) {
        Transact transact = Transact(++Id, ModelTime + RandNumber[i].expDisGen(), i+1);
        FEC.push(transact);
    }
}

void Simulate::TimeCorrectPhase() {
    ModelTime = FEC.top().getModelTime();
    for(int i = 0; i < FEC.GetCount(); i++) {
        if(ModelTime != FEC.top().getModelTime())
            break;
        CEC.pushBack(FEC.top());
        FEC.pop();
    }
    CEC.resetExecutingTransact();
}

void Simulate::ViewingPhase() {
    CEC.resetExecutingTransact();
    while(CEC.getExecutingTransact() != nullptr) {
        int typeWorker = choiseTypeWorker(CEC.getExecutingTransact()->data) - 1;
        if(CEC.canRun(storage[typeWorker]))
            runTransact();
        else {
            CEC.nextExecutingTransact();
        }
    }
}

void Simulate::runTransact() {
    int typeTransact = CEC.getExecutingTransact()->data.getType();

    if(CEC.getExecutingTransact()->data.getNextBlock() == 1) { // GENERATE
        CEC.getExecutingTransact()->data.promotion();
        FEC.push(Transact(++Id, ModelTime + RandNumber[typeTransact - 1].expDisGen(), typeTransact));
            storage[CEC.getExecutingTransact()->data.getTypeWorker() - 1].addQueue();
    }
    else if(CEC.getExecutingTransact()->data.getNextBlock() == 2) { // ENTER
        unsigned typeWorker = choiseTypeWorker(CEC.getExecutingTransact()->data) - 1;
        if(storage[typeWorker].isFree()){
            storage[typeWorker].takeWorker(&CEC.getExecutingTransact()->data);
            CEC.getExecutingTransact()->data.promotion();
        }
        else 
            CEC.nextExecutingTransact();
    }
    else if(CEC.getExecutingTransact()->data.getNextBlock() == 3) { // ADVANCE
        CEC.getExecutingTransact()->data.promotion();
        avgLenQueue+=ModelTime - CEC.getExecutingTransact()->data.getModelTime();
        storage[CEC.getExecutingTransact()->data.getTypeWorker() - 1].setAvgQueue(storage[CEC.getExecutingTransact()->data.getTypeWorker() - 1].getAvgQueue() + ModelTime - CEC.getExecutingTransact()->data.getModelTime());
        CEC.getExecutingTransact()->data.setModelTime(ModelTime + RandNumber[CEC.getExecutingTransact()->data.getType() - 1].expDisAdv(CEC.getExecutingTransact()->data));
        FEC.push(CEC.getExecutingTransact()->data);
        storage[CEC.getExecutingTransact()->data.getTypeWorker() - 1].removeQueue();
        CEC.deleteExecutingTransact();
    }
    else if(CEC.getExecutingTransact()->data.getNextBlock() == 4) { // LEAVE
        CEC.getExecutingTransact()->data.promotion();
        storage[CEC.getExecutingTransact()->data.getTypeWorker() - 1].freeWorker(&CEC.getExecutingTransact()->data);
        flagReset = true;
    }
    else if(CEC.getExecutingTransact()->data.getNextBlock() == 5) { // TERMINATE
        CEC.deleteExecutingTransact();
        if(flagReset) {
            CEC.resetExecutingTransact();
            flagReset = false;
        }
        else
            CEC.nextExecutingTransact();
    }
}

Simulate::Simulate(int countTypeTransact, int countTypeWorker, int* meanTransact,
                   int* countWorkers, int** meanProcessingTransact)
: countTypeTransact(countTypeTransact), countTypeWorker(countTypeWorker) {
    RandNumber = new RandomNumber[countTypeTransact];
    for(int i = 0; i < countTypeTransact; i++) {
        RandNumber[i].setMeanBornTransact(meanTransact[i]);
        RandNumber[i].setMeanProcessingTransact(meanProcessingTransact[i]);
    }
    storage = new Workers[countTypeWorker];
    for(int i = 0; i < countTypeWorker; i++)
        storage[i].setCountWorkers(countWorkers[i]);
}

Simulate::~Simulate() {
    delete[] RandNumber;
    delete[] storage;
}

void Simulate::start(std::ofstream& file) {
    InputPhase();
    int i = 0;
    unsigned timer = 15;

    while(ModelTime <= timer) {
        cout << "Итерация " << i << endl;
        file << "Итерация " << i++ << endl;
        TimeCorrectPhase();
        print(true);
        print(file, true);
        ViewingPhase();
        print(false);
        print(file, false);
        for(int i = 0; i < countTypeWorker; i++)
            storage[i].updageMaxQueue();
    }
    printStatic(timer);
    printStatic(timer,file);
}

void Simulate::print(bool flagViewingMode) {
    if(flagViewingMode) 
        cout << "=======================================================================================" << endl;
    else
        for(int i = 0; i < countTypeWorker; i++) {
            cout << "Рабочий типа " << i + 1 << " обрабатывает на данный момент: " << storage[i].getMaxCountWorkers() - storage[i].getFreeCountWorkers() << endl;
            cout << "Очередь у рабочего типа " << i + 1 << " = " << storage[i].getMaxQueue() << endl;
        }
    cout << "После TimeCorrect" << endl;
    cout << "AllModelTime: " << ModelTime << endl;
    cout << "CEC" << endl;
    CEC.print();
    cout << "FEC" << endl;
    FEC.print();
    if(flagViewingMode)
        cout << "---------------------------------------------------------------------------------------" << endl;
    else
        cout << "=======================================================================================" << endl;
}

void Simulate::print(std::ofstream& file, bool flagViewingMode) {
    if(flagViewingMode) 
        file << "=======================================================================================" << endl;
    else
        for(int i = 0; i < countTypeWorker; i++) {
            file << "Рабочий типа " << i + 1 << " обрабатывает на данный момент: " << storage[i].getMaxCountWorkers() - storage[i].getFreeCountWorkers() << endl;
            file << "Очередь у рабочего типа " << i + 1 << " = " << storage[i].getMaxQueue() << endl;
        }
    file << "После TimeCorrect" << endl;
    file << "AllModelTime: " << ModelTime << endl;
    file << "CEC" << endl;
    CEC.print(file);
    file << "FEC" << endl;
    FEC.print(file);
    if(flagViewingMode)
        file << "---------------------------------------------------------------------------------------" << endl;
    else
        file << "=======================================================================================" << endl;
}

void Simulate::printStatic(int timer) {
    for(int i = 0; i < countTypeWorker; i++) {
        cout << "Средняя длина очереди рабочего типа " << i + 1 << " = " << storage[i].getAvgQueue() / timer<< endl;
        cout << "Максимальная длина очереди рабочего типа " << i + 1 << " = " << storage[i].getMaxQueue() << endl;
    }
}

void Simulate::printStatic(int timer, std::ofstream& file) {
    for(int i = 0; i < countTypeWorker; i++) {
        file << "Средняя длина очереди рабочего типа " << i + 1 << " = " << storage[i].getAvgQueue() / timer<< endl;
        file << "Максимальная длина очереди рабочего типа " << i + 1 << " = " << storage[i].getMaxQueue() << endl;
        
    }
}
