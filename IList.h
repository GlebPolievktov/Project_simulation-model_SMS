#pragma once

template <typename T>
class IList {
protected:
  unsigned _size;
public:
  IList(): _size(0) {}
  virtual ~IList() {}
  unsigned size() const { return _size; }
  virtual IList<T>& pushBack(const T &value) = 0;
  virtual T popFront() = 0;
  virtual const T& operator[](int index) const = 0;
  virtual T& operator[](int index) = 0;
};