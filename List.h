#pragma once

#include <stdexcept>
#include "IList.h"

template <typename T>
class List : public IList<T>
{
  protected:
    struct ListNode
    {
      T data;
      ListNode *next;
      ListNode *prev;
    };

    ListNode *_head, *_tail;

    ListNode* _createNode(const T &value);
    void _addNodeInTail(ListNode *node);
    void _removeNodeFromHead();
    void _removeNode(ListNode* node);

  public:
    List(): IList<T>(), _head(nullptr), _tail(nullptr) {}
    ~List() { clear(); }

    void clear();

    ListNode* getHead() { return _head; }
    ListNode* getTail() { return _tail; }

    IList<T>& pushBack(const T &value);
    T popFront();
    const T& operator[](int index) const;
    T& operator[](int index);
    
};

// private list-node helpers ------------------------------

template <typename T>
typename List<T>::ListNode* List<T>::_createNode(const T &value)
{
  List<T>::ListNode *newNode = new List<T>::ListNode;
  newNode->next = newNode->prev = nullptr;
  newNode->data = value;
  return newNode;
}

template <typename T>
void List<T>::_removeNodeFromHead()
{
  if (!this->_head)
  {
    return;
  }

  List<T>::ListNode *oldHead = this->_head;
  this->_head = this->_head->next;
  delete oldHead;

  if (!this->_head)
  {
    this->_tail = nullptr;
  }
}

template <typename T>
void List<T>::_addNodeInTail(List<T>::ListNode *node)
{
  if (!this->_tail)
  {
    this->_head = this->_tail = node;
    return;
  }

  node->prev = this->_tail;
  this->_tail->next = node;
  this->_tail = node;
}

// public list interface ---------------------------------

template <typename T>
void List<T>::clear()
{
  if (!this->_head && !this->_tail && !this->_size)
  {
    return;
  }

  while (this->_size)
  {    
    this->_removeNodeFromHead();
    this->_size--;
  }
}

template <typename T>
IList<T>& List<T>::pushBack(const T &value)
{
  this->_addNodeInTail(
    this->_createNode(value)
  );

  this->_size++;

  return *this;
}

template <typename T>
T List<T>::popFront()
{
  if (!this->_size)
  {
    throw std::logic_error("trying to remove element from empty list");
  }

  T value = this->_head->data;

  this->_removeNodeFromHead();
  this->_size--;

  return value;
}

template <typename T>
const T& List<T>::operator[](int index) const
{
  if (!this->_size)
  {
    throw std::logic_error("trying to get element from empty list");
  }

  if (index >= this->_size || index < 0)
  {
    throw std::invalid_argument("trying to get element by wrong index");
  }

  List<T>::ListNode *tempNode = nullptr;

  if (index < this->_size / 2)
  {
    tempNode = this->_head;
    for (int i = 0; i != index; i++, tempNode = tempNode->next);
  }
  else
  {
    tempNode = this->_tail;
    for (int i = this->_size - 1; i != index; i--, tempNode = tempNode->prev);
  }

  return tempNode->data;
}

template <typename T>
T& List<T>::operator[](int index)
{
  if (!this->_size)
  {
    throw std::logic_error("trying to get element from empty list");
  }

  if (index >= this->_size || index < 0)
  {
    throw std::invalid_argument("trying to get element by wrong index");
  }

  List<T>::ListNode *tempNode = nullptr;

  if (index < this->_size / 2)
  {
    tempNode = this->_head;
    for (int i = 0; i != index; i++, tempNode = tempNode->next);
  }
  else
  {
    tempNode = this->_tail;
    for (int i = this->_size - 1; i != index; i--, tempNode = tempNode->prev);
  }

  return tempNode->data;
}

template<typename T>
void List<T>::_removeNode(ListNode* node) {
  if(!node) {
    return;
  }
  if(node == this->_head) {
    this->_removeNodeFromHead();
    return;
  }
  if(node == this->_tail) {
    this->_tail = node->prev;
    if(this->_tail) {
      this->_tail->next = nullptr;
      }
    delete node;
    node = nullptr;
    this->_size--;
    return;
  }

  node->prev->next = node->next;
  node->next->prev = node->prev;

  delete node;
  node = nullptr;
  this->_size--;
}
