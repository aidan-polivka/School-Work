#pragma once

#include <climits>
#include <iostream>
#include <stdexcept>
#include <vector>

//-----------------------------------------------------------
// class definitions
//-----------------------------------------------------------

/**
 * Integer min-heap class.
 */
class IntMinHeap {

public:
  /**
   * Create a new min-heap.
   */
  IntMinHeap();

  /**
   * Create a new min-heap just like an existing one.
   *
   * \param Existing min-heap to copy from.
   */
  IntMinHeap(const IntMinHeap &heap);

  /**
   * Destructor. Free the memory used by the min-heap.
   */
  ~IntMinHeap();

  /**
   * Remove all of the elements from the min-heap.
   */
  void clear();

  /**
   * Insert an integer into the heap.
   *
   * \param val Element to insert.
   */
  void insert(int val);

  /**
   * Determine if this min-heap is empty.
   *
   * \return True if the heap is empty, false otherwise.
   */
  bool isEmpty() const { return vec.size() == 1u; }

  /**
   * Override of the stream insertion operator for MinHeap objects.
   *
   * \param out ostream object to output to, e.g., cout
   *
   * \param heap Heap to output
   *
   * \return the out ostream object
   */
  friend std::ostream &operator<<(std::ostream &out, const IntMinHeap &heap) {
    out << "[";
    unsigned n = heap.size();
    if (n == 0u) {
      out << "]";
    } else {
      for (unsigned i = 1u; i < n; i++) {
        out << heap.vec.at(i) << ", ";
      }
      out << heap.vec.at(n);
      out << "]";
    }
    return out;
  }

  /**
   * Override of the assignment operator for MinHeap objects.
   *
   * \param heap MinHeap to copy from
   * \return Reference to this MinHeap, for chaining purposes.
   */
  IntMinHeap &operator=(const IntMinHeap &heap);

  /**
   * Remove the smallest element from the min-heap.
   *
   * \return Smallest element from the heap.
   */
  int removeMin();

  /**
   * Get the number of elements in the min-heap.
   *
   * \return Number of elements in the heap.
   */
  unsigned size() const {
    return vec.size() - 1u;
    ;
  }

private:
  /** STL vector serving as the array for this heap. */
  std::vector<int> vec;

  /**
   * Copy the contents of one min-heap into this one.
   *
   * \param heap Min-heap to copy from.
   */
  void copy(const IntMinHeap &heap);

  /**
   * Decrease the key for heap element i to a new value.
   *
   * \param i Index of element to modify.
   * \param key New key value for element i.
   */
  void heapDecreaseKey(unsigned i, int key);

  /**
   * Determine the index of an element's left child.
   *
   * \param i Index of parent element.
   * \return Index of i's left child.
   */
  inline unsigned left(unsigned i) { return i << 1; }

  /**
   * Reinforce the min-heap property.
   *
   * \param i Index to begin the process.
   */
  void minHeapify(unsigned i);

  /**
   * Determine the index of the parent of an element.
   *
   * \param i Index of child element.
   * \return Index of i's parent.
   */
  inline unsigned parent(unsigned i) { return i >> 1; }

  /**
   * Determine the index of an element's right child.
   *
   * \param i Index of parent element.
   * \return Index of i's right child.
   */
  inline unsigned right(unsigned i) { return (i << 1) + 1u; }

  /**
   * Swap two elements in the heap's backing array.
   *
   * \param i Index of one element to swap.
   * \param j Index of the other element to swap.
   */
  void swap(unsigned i, unsigned j);
};

//-----------------------------------------------------------
// function implementations
//-----------------------------------------------------------

/*
 * Implementation of the Heap constructor.
 */
IntMinHeap::IntMinHeap() {
  // add a default node to the vector, consuming location 0,
  // so our math can use index 1 as the start
  vec.push_back(INT_MAX);
}

/*
 * Copy constructor implementation.
 */
IntMinHeap::IntMinHeap(const IntMinHeap &heap) { copy(heap); }

/*
 * Destructor implementation.
 */
IntMinHeap::~IntMinHeap() { clear(); }

/*
 * Implementation of the clear() function.
 */
void IntMinHeap::clear() { vec.clear(); }

/*
 * Implementation of the copy() function.
 */
void IntMinHeap::copy(const IntMinHeap &heap) {
  auto i = heap.vec.cbegin();
  while (i != heap.vec.cend()) {
    vec.push_back(*i);
    i++;
  }
}

/*
 * Implementation of heapDecreaseKey() function.
 */
void IntMinHeap::heapDecreaseKey(unsigned i, int key) {
  if (key > vec[i]) {
    throw std::out_of_range("New key greater than original in"
                            "MinHeap::heapDecreaseKey()");
  }

  vec[i] = key;
  unsigned p = parent(i);
  while (i > 1 && vec[p] > vec[i]) {
    swap(i, p);
    i = p;
    p = parent(i);
  }
}

/*
 * Implementation of insert() function.
 */
void IntMinHeap::insert(int val) {
  vec.push_back(INT_MAX);
  heapDecreaseKey(size(), val);
}

/*
 * Implementation of the minHeapify() function.
 */
void IntMinHeap::minHeapify(unsigned i) {
  unsigned le = left(i);
  unsigned ri = right(i);
  unsigned smallest = i;

  if (le < size() && vec[le] < vec[i]) {
    smallest = le;
  }
  if (ri < size() && vec[ri] < vec[smallest]) {
    smallest = ri;
  }

  if (smallest != i) {
    swap(i, smallest);
    minHeapify(smallest);
  }
}

/*
 * Implementation of assignment operator.
 */
IntMinHeap &IntMinHeap::operator=(const IntMinHeap &heap) {
  // delete existing min-heap contents
  clear();

  // copy existing min-heap
  copy(heap);
  
  return *this;
}

/*
 * Implementation of the removeMin() function.
 */
int IntMinHeap::removeMin() {
  // prevent removes from empty heaps
  if (size() == 0u) {
    throw std::out_of_range("Empty heap in Heap::removeMin()");
  }

  // save smallest value
  int min = vec[1];

  // overwrite smallest with last element, then re-heapify
  vec[1] = vec[size()];
  minHeapify(1);
  vec.pop_back();

  // send back smallest value
  return min;
}

/**
 * Implementation of the swap() function.
 */
void IntMinHeap::swap(unsigned i, unsigned j) {
  int t = vec[i];
  vec[i] = vec[j];
  vec[j] = t;
}
