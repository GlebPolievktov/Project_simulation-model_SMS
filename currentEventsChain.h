#pragma once

#include <iostream>
#include "List.h"
#include "transact.h"
#include "storage.h"
#include <fstream>

class CurrentEventsChain: public List<Transact> {
private:
    ListNode* executingTransact;
public:
    CurrentEventsChain() {}
    void nextExecutingTransact();
    void resetExecutingTransact();
    bool canRun(Workers &worker);
    ListNode* getExecutingTransact() const {return executingTransact;}
    void print(std::ofstream& file);
    void print();
    void deleteExecutingTransact();
};

void CurrentEventsChain::nextExecutingTransact() {
    if(executingTransact == nullptr)
        return;
    executingTransact = executingTransact->next;
}

void CurrentEventsChain::resetExecutingTransact() {
    executingTransact = _head;
}

bool CurrentEventsChain::canRun(Workers &worker) {
    if(executingTransact && executingTransact->data.getNextBlock() == 2) { // 2 = ENTER
        if(worker.isFree())
            return true;
        return false;
    }
    return true;
}

void CurrentEventsChain::print(std::ofstream& file) {
    ListNode* currentNode;
    for(currentNode = this->getHead(); currentNode != nullptr; currentNode = currentNode->next) {
        currentNode->data.print(file);
        file << std::endl;
    }
}

void CurrentEventsChain::print() {
    ListNode* currentNode;
    for(currentNode = this->getHead(); currentNode != nullptr; currentNode = currentNode->next) {
        currentNode->data.print();
        std::cout << std::endl;
    }
}

void CurrentEventsChain::deleteExecutingTransact() {
    if (executingTransact == nullptr) {
        return;
    }
      
      ListNode* nodeToDelete = executingTransact;
    if(executingTransact == _head) {
        _removeNodeFromHead();
          executingTransact = _head;
        return;
    }

    
    if(executingTransact == _tail) {
        
         _removeNode(executingTransact);
          executingTransact = nullptr;
          return;
    }

       executingTransact = executingTransact->next;
     _removeNode(nodeToDelete);
}