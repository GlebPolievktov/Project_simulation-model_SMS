#pragma once

#include <fstream>

class Transact {
private:
    int id;
    int currentBlock;
    int nextBlock;
    double modelTime;
    int type;
    int typeWorker = -1;
public:
    Transact();
    Transact(int, double, int);
    void setModelTime(const double);
    int getId () const;
    int getCurrentBlock () const;
    int getNextBlock () const;
    double getModelTime () const;
    int getType() const;
    void promotion();
    int getTypeWorker();
    void setTypeWorker(int type);
    bool operator<(const Transact& r) const;
    void print(std::ofstream& file) const;
    void print() const;
};

Transact::Transact()
: id(-1), currentBlock(-1), nextBlock(-1), modelTime(-1), type(-1) {
}

Transact::Transact(int id_, double modelTime_, int type_)
    : id(id_), currentBlock(-1), nextBlock(1), modelTime(modelTime_), type(type_) {
}

void Transact::setModelTime(const double time) {
    modelTime = time;
}

int Transact::getId() const {
    return id;
}

int Transact::getCurrentBlock() const {
    return currentBlock;
}

int Transact::getNextBlock() const {
    return nextBlock;
}

double Transact::getModelTime() const {
    return modelTime;
}

int Transact::getType() const {
    return type;
}

void Transact::promotion() {
    currentBlock = nextBlock;
    nextBlock = currentBlock + 1;
}

int Transact::getTypeWorker() {
    return typeWorker;
}

void Transact::setTypeWorker(int type) {
    typeWorker = type;
}

bool Transact::operator<(const Transact &r) const {
    return this->modelTime < r.modelTime;
}

void Transact::print(std::ofstream& file) const{
    file << '\t';
    file << "ID: " << id;
    file << ", TYPE: " << type;
    file << ", currentBlock: " << currentBlock;
    file << ", nextBlock: " << nextBlock;
    file << ", modelTime: " << modelTime;
    file << ", typeWorkers: " << typeWorker;
}

void Transact::print() const{
    cout << '\t';
    cout << "ID: " << id;
    cout << ", TYPE: " << type;
    cout << ", currentBlock: " << currentBlock;
    cout << ", nextBlock: " << nextBlock;
    cout << ", modelTime: " << modelTime;
    cout << ", typeWorkers: " << typeWorker;
    
}
