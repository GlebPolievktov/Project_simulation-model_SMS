#pragma once

#include "heap.h"
#include "transact.h"
#include <fstream>

struct FECComparator {
    bool operator()(const Transact& l, const Transact& r) const {
        return l < r;
    }
};

class FutureEventsChain : public Heap<Transact, FECComparator> {
public:
    FutureEventsChain() {}
    void print(std::ofstream& file) const;
    void print() const;
};

void FutureEventsChain::print(std::ofstream& file) const {
    Transact* buffer = getBuffer();
    for(int i = 0; i < GetCount(); i++) {
        buffer[i].print(file);
        file << endl;
    }
}

void FutureEventsChain::print() const {
    Transact* buffer = getBuffer();
    for(int i = 0; i < GetCount(); i++) {
        buffer[i].print();
        cout << endl;
    }
}