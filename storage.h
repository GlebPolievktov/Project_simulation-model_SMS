#pragma once

#include "List.h"
#include "transact.h"

class Workers {
private:
    unsigned freeCountWorkers;
    unsigned maxCountWorkers;
    unsigned queue;
    unsigned maxQueue;
    double avgQueue;
public:
    Workers(): freeCountWorkers(0), maxCountWorkers(0), queue(0) {}
    unsigned getFreeCountWorkers();
    unsigned getMaxCountWorkers();
    unsigned getQueue();
    unsigned getMaxQueue();
    double getAvgQueue(); 
    bool isFree();
    void addQueue();
    void removeQueue();
    void setAvgQueue(int n);
    void updageMaxQueue();
    void resetQueue();
    void takeWorker(Transact* transact);
    void freeWorker(Transact* transact);
    void setCountWorkers(int n);
};

unsigned Workers::getFreeCountWorkers() {
    return freeCountWorkers;
}

unsigned Workers::getMaxCountWorkers() {
    return maxCountWorkers;
}

unsigned Workers::getQueue() {
    return queue;
}

unsigned Workers::getMaxQueue() {
    return maxQueue;
}

double Workers::getAvgQueue() {
    return avgQueue;
}

bool Workers::isFree()
{
    if(freeCountWorkers > 0)
        return true;
    return false;
}

void Workers::addQueue() {
    queue++;
}

void Workers::removeQueue() {
    queue--;
}

void Workers::setAvgQueue(int n) {
    avgQueue = n;
}

void Workers::updageMaxQueue() {
    if(queue > maxQueue)
        maxQueue = queue;
}

void Workers::resetQueue() {
    queue = 0;
}

void Workers::takeWorker(Transact *transact) {
    freeCountWorkers--;
}

void Workers::freeWorker(Transact *transact) {
    freeCountWorkers++;
}

void Workers::setCountWorkers(int n) {
    freeCountWorkers = n;
    maxCountWorkers = n;
}
