#pragma once

#include <iostream>
#include <stdexcept>

//-----------------------------------------------------------
// class definitions
//-----------------------------------------------------------

/**
 * Class representing a simple, templated singly-linked list, with an
 * iterator.
 */
template <class T> class SLL {
private:
  //-------------------------------------------------------
  // inner class definition
  //-------------------------------------------------------

  /**
   * Private nested class representing nodes in the singly-linked
   * list.
   */
  class Node {
  public:
    /**
     * Default constructor. Make a new Node with default data
     * and next pointer set to zero.
     */
    Node() : data(), pNext(0) {}

    /**
     * Initializing constructor. Make a new Node with the specified
     * data and next pointer values.
     *
     * \param d Data value of type T for the node.
     *
     * \param pN Pointer to the next Node in the list, or 0 if
     * this is the last Node.
     */
    Node(T d, Node *pN) : data(d), pNext(pN) {}

    /** Type T payload of the Node. */
    T data;

    /** Pointer to the next Node in the list, or 0 if last node. */
    Node *pNext;
  };

public:
  //-------------------------------------------------------
  // inner class definition
  //-------------------------------------------------------

  /**
   * Iterator for the singly-linked list.
   */
  class Iterator {
  public:
    /** Dereferencing operator to allow access to node's data. */
    T &operator*();

    /** Equality operator to test if this iterator is at another's
     * position.
     */
    bool operator==(const Iterator &other) const {
      return pCurr == other.pCurr;
    }

    /** Inquality operator to test if this iterator is not at
     * another's position.
     */
    bool operator!=(const Iterator &other) const {
      return pCurr != other.pCurr;
    }

    /** Increment operator to advance to next element. */
    Iterator &operator++();

    // make us a friend of the outer class
    friend class SLL;

  private:
    /** Current iterater location. */
    Node *pCurr;

    /** Private constructor can't be accessed outside the list. */
    Iterator(Node *pC) : pCurr(pC) {}
  };

  /**
   * Default constructor. Make an empty list.
   */
  SLL() : pHead(0), n(0u) {}

  /**
   * Copy constructor. Make a new list just like the parameter.
   *
   * \param list SLL to copy
   */
  SLL(const SLL<T> &list);

  /**
   * Destructor. Delete the contents of this list.
   */
  ~SLL() { clear(); }

  /**
   * Add a value to the front of the list.
   *
   * \param d Value to add to the list.
   */
  void add(const T &d);

  /**
   * Get iterator to the first element of the list.
   */
  Iterator begin() const;

  /**
   * Remove all the elements of the list.
   */
  void clear();

  /**
   * Determine if a specific value is in the list.
   *
   * \param key Value of type T to search for.
   *
   * \return Index of first occurrence of key in the list, or
   * -1 if it is not in the list.
   */
  int contains(const T &key) const;

  /**
   * Get iterator that marks the end of the list (one element past
   * end).
   */
  Iterator end() const;

  /**
   * Get the value at a specified location.
   *
   * \param idx Index of the value to get.
   *
   * \return Value at location idx in the list
   */
  T &get(unsigned idx) const;

  /**
   * Determine if this list is empty.
   *
   * \return true if the list is empty, false otherwise.
   */
  bool isEmpty() const { return size() == 0u; }

  /**
   * Remove the value at a specified location.
   *
   * \param idx Index of the element to remove.
   *
   * \return Value that was in location idx.
   */
  T remove(unsigned idx);

  /**
   * Change the value at a specified index to another value.
   *
   * \param idx Index of the value to change.
   *
   * \param d New value to place in position idx.
   */
  void set(unsigned idx, const T &d);

  /**
   * Get the number of elements in the list.
   *
   * \return The number of elements in the list.
   */
  unsigned size() const { return n; }

  /**
   * Override of the assignment operator for SLL objects.
   *
   * \param list SLL to copy.
   */
  SLL<T> &operator=(const SLL<T> &list);

  /**
   * Override of the stream insertion operator for SLL objects.
   *
   * \param out ostream object to output to, e.g., cout
   *
   * \param list SLL to output
   *
   * \return the out ostream object
   */
  friend std::ostream &operator<<(std::ostream &out, const SLL<T> &list) {

    out << "[";

    // initialize cursor to the head of the list
    Node *pCurr = list.pHead;

    // iterate through the list
    while (pCurr != 0) {
      out << pCurr->data;

      // output no comma for last element
      if (pCurr->pNext != 0) {
        out << ", ";
      }
      pCurr = pCurr->pNext;
    }
    out << "]";

    return out;
  }

private:
  /**
   * Private helper method: make this list just like the parameter
   * list.
   *
   * \param list SLL to copy.
   */
  void copy(const SLL<T> &list);

  /** Pointer to the first Node in the list, or 0 if list is empty. */
  Node *pHead;

  /** Number of elements in the list. */
  unsigned n;
};

//-----------------------------------------------------------
// function implementations
//-----------------------------------------------------------

/*
 * Iterator dereference operator overload.
 */
template <class T> T &SLL<T>::Iterator::operator*() {
  // if the access is past list end, throw an exception
  if (pCurr == 0) {
    throw std::out_of_range("Dereferencing beyond list end in "
                            "Iterator::*()");
  }

  return pCurr->data;
}

/*
 * Iterator increment operator overload.
 */
template <class T> typename SLL<T>::Iterator &SLL<T>::Iterator::operator++() {
  // if the access is past list end, throw an exception
  if (pCurr == 0) {
    throw std::out_of_range("Increment beyond list end in "
                            "Iterator::++()");
  }

  pCurr = pCurr->pNext;
  return *this;
}

/*
 * Copy constructor
 */
template <class T> SLL<T>::SLL(const SLL<T> &list) : pHead(0), n(0u) {
  copy(list);
}

/*
 * Assignment operator.
 */
template <class T> SLL<T> &SLL<T>::operator=(const SLL<T> &list) {
  copy(list);
  return *this;
}

/*
 * Add d to front of list.
 */
template <class T> void SLL<T>::add(const T &d) {
  // create new node
  Node *pN = new Node(d, pHead);
  // change head to point to new node
  pHead = pN;
  // increment size
  n++;
}

/*
 * Get iterator to the first node.
 */
template <class T> typename SLL<T>::Iterator SLL<T>::begin() const {
  return Iterator(pHead);
}

/*
 * Delete all list nodes.
 */
template <class T> void SLL<T>::clear() {
  // create cursors
  Node *pCurr = pHead, *pPrev = 0;

  // iterate through the list
  while (pCurr != 0) {
    // "inchworm" up to next node
    pPrev = pCurr;
    pCurr = pCurr->pNext;

    // delete previous node
    delete pPrev;
  }

  // reset head pointer and size
  pHead = 0;
  n = 0u;
}

/*
 * Search the list for element key.
 */
template <class T> int SLL<T>::contains(const T &key) const {
  // create cursors
  int idx = -1;
  Node *pCurr = pHead;

  // iterate until we find key or end of list
  while (pCurr != 0) {
    idx++;
    // hit? return index
    if (pCurr->data == key) {
      return idx;
    }

    pCurr = pCurr->pNext;
  }

  // not found
  return -1;
}

template <class T> void SLL<T>::copy(const SLL<T> &list) {
  // remove any existing data
  clear();

  // initialize cursors, one for parameter and
  // one for this list
  Node *pL = list.pHead, *pCurr = pHead;

  // iterate through nodes in parameter list
  while (pL != 0) {
    // special case: first node changes head pointer
    if (pHead == 0) {
      pHead = new Node(pL->data, 0);
      pCurr = pHead;
    } else {
      // general case: add new node to end of this list
      pCurr->pNext = new Node(pL->data, 0);
      pCurr = pCurr->pNext;
    }

    n++;
    pL = pL->pNext;
  }
}

/*
 * Get iterator to one-past-the-last element of the list.
 */
template <class T> typename SLL<T>::Iterator SLL<T>::end() const {
  return Iterator(0);
}

/*
 * Get value at location idx.
 */
template <class T> T &SLL<T>::get(unsigned idx) const {
  // initialize cursor
  Node *pCurr = pHead;

  // if the access is past list end, throw an exception
  if (idx >= n) {
    throw std::out_of_range("Index out of range in SLL::get()");
  }

  // iterate cursor to postion
  for (unsigned i = 0u; i < idx && pCurr != 0; i++) {
    pCurr = pCurr->pNext;
  }

  // return requested value
  return pCurr->data;
}

/*
 * Remove node at location idx.
 */
template <class T> T SLL<T>::remove(unsigned idx) {
  // initialize cursors
  Node *pCurr = pHead, *pPrev = 0;

  // if the access is past list end, throw an exception
  if (idx >= n) {
    throw std::out_of_range("Index out of range in SLL::remove()");
  }

  // iterate cursors to position
  for (unsigned i = 0u; i < idx && pCurr != 0; i++) {
    pPrev = pCurr;
    pCurr = pCurr->pNext;
  }

  // save value of node we're deleting
  T d = pCurr->data;

  // first element? change head pointer
  if (pCurr == pHead) {
    pHead = pCurr->pNext;
  } else {
    // general case: "wire around" node
    pPrev->pNext = pCurr->pNext;
  }

  // remove node and decrement size
  delete pCurr;
  n--;

  // send return value
  return d;
}

/*
 * Change the value at location idx to d.
 */
template <class T> void SLL<T>::set(unsigned idx, const T &d) {
  // initialize cursor
  Node *pCurr = pHead;

  // if the access is past list end, throw an exception
  if (idx >= n) {
    throw std::out_of_range("Index out of range in SLL::set()");
  }

  // iterate to location
  for (unsigned j = 0u; j < idx; j++) {
    pCurr = pCurr->pNext;
  }

  // change data in location idx to d
  pCurr->data = d;
}
