#pragma once

#include <iostream>
#include "transact.h"
#include <random>

class RandomNumber {
private:
    int meanBornTransact;
    int* meanProcessingTransact;
    std::random_device rd;
	std::mt19937 generator;
public:
    RandomNumber(): meanBornTransact(0), meanProcessingTransact(nullptr), rd(), generator(rd()) {}
    void setMeanBornTransact(int mean);
    void setMeanProcessingTransact(int* mean);
    double expDisGen();
    double expDisAdv(Transact& transact);
};

void RandomNumber::setMeanBornTransact(int mean) {
    meanBornTransact = mean;
}

void RandomNumber::setMeanProcessingTransact(int *mean) {
    meanProcessingTransact = mean;
}

double RandomNumber::expDisGen() {
    std::exponential_distribution<double> expDis(1.0/meanBornTransact);
    return expDis(generator);
}

double RandomNumber::expDisAdv(Transact& transact) {
    std::exponential_distribution<double> expDis(1.0/meanProcessingTransact[transact.getTypeWorker() - 1]);
    return expDis(generator);
}
