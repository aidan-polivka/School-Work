#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>

//-----------------------------------------------------------
// class definitions
//-----------------------------------------------------------

/**
 * Class representing a general tree, with no restrictions on the
 * number of children per node.
 */
template <class T> class Tree {

private:
  //-------------------------------------------------------
  // inner class definition
  //-------------------------------------------------------

  /**
   * Private nested class representing a node in the tree.
   */
  class Node {

  public:
    /**
     * Default constructor. Create an empty node with no children.
     */
    Node() : data(), children(), pParent(0) {}

    /**
     * Initializing constructor. Make a new node containing the
     * specified value, and no children.
     *
     * \param v Value to place into the node.
     * \param pPar Pointer to this node's parent, or 0 if the
     * node is the root.
     */
    Node(const T &v, Node *pPar) : data(v), children(), pParent(pPar) {}

    /** Type T payload of the node. */
    T data;

    /** Pointers to the children of this node. */
    std::vector<Node *> children;

    /** Pointer to this node's parent, 0 if the node is the root. */
    Node *pParent;
  };

public:
  //-------------------------------------------------------
  // inner class definition
  //-------------------------------------------------------

  /**
   * Public inner class used to navigate and construct trees.
   */
  class Handle {
  public:
    /**
     * Add a new child to the node at this handle.
     *
     * \param v Data value to store in the new child.
     */
    void addChild(const T &v);

    /**
     * Get a Handle object for the ith child of this handle.
     *
     * \param i Which child to fetch.
     * \return Handle of ith child of this node.
     */
    Handle getChild(unsigned i);

    /**
     * Get a Handle object for the parent of this handle.
     *
     * \return Handle of this node's parent in the tree.
     */
    Handle getParent();

    /**
     * Get the number of children the node at this handle has.
     *
     * \return Number of children.
     */
    unsigned numChildren() { return pCurr->children.size(); }

    /**
     * Prune all the nodes (and their decendants) that are
     * children of this node.
     */
    void prune();

    /**
     * Dereferencing operator to get access to the data in
     * this handle's node.
     *
     * \return Reference to the data element at this handle
     * in the tree.
     */
    T &operator*();

    /**
     * Assignment operator overload.
     *
     * \param loc Handle to copy from.
     * \return Reference to this handle, for chaining.
     */
    Handle &operator=(const Handle &loc);

    // make us a friend of the enclosing class
    friend class Tree;

  private:
    /** Current handle in the tree. */
    Node *pCurr;

    /** Pointer to the handle's parent tree. */
    Tree<T> *pTree;

    /**
     * Private constructor can't be accessed outside of Tree
     * class.
     */
    Handle(Node *pC, Tree<T> *pT) : pCurr(pC), pTree(pT) {}
  };

  /**
   * Initializing constructor. Create a new tree, with a single
   * root node containing the specified value.
   *
   * \param v Value to place into the root node.
   */
  Tree(const T &v) : pRoot(new Node(v, 0)), n(1u) {}

  /**
   * Copy constructor. Make a new tree just like an existing one.
   *
   * \param tree Tree to make a deep copy of.
   */
  Tree(const Tree<T> &tree);

  /**
   * Destructor. Remove all the nodes from this tree, and free
   * the associated memory.
   */
  ~Tree();

  /**
   * Determine if this tree is empty.
   *
   * \return True if the tree is empty, false otherwise.
   */
  bool isEmpty() const { return n == 0u; }

  /**
   * Overridden assignment operator.
   *
   * \param tree Tree to copy from.
   * \return Reference to this tree, for chaining.
   */
  Tree<T> &operator=(const Tree<T> &tree);

  /**
   * Override of the stream insertion operator for Tree objects.
   * Prints via a preorder traversal.
   *
   * \param out ostream object to output to, e.g., cout
   *
   * \param tree Tree to output
   *
   * \return the out ostream object
   */
  friend std::ostream &operator<<(std::ostream &out, const Tree<T> &tree) {
    out << "[";
    preOrderPrint(out, tree.pRoot);
    out << "]";
    return out;
  }

  /**
   * Get a Handle object positioned on the root of the tree.
   */
  Handle rootHandle() { return Handle(pRoot, this); }

  /**
   * Get the number of nodes in the tree.
   *
   * \return Number of nodes in the tree.
   */
  unsigned size() const { return n; }

private:
  /** Pointer to the root node. */
  Node *pRoot;

  /** Number of nodes in the tree. */
  unsigned n;

  /**
   * Recursive helper function to copy trees.
   *
   * \param pCurr Pointer to the node in the existing tree we are
   * copying from.
   * \param pPar Pointer to the new node's parent in the tree.
   * \return Pointer to the newly created node.
   */
  Node *copy(Node *pCurr, Node *pPar);

  /**
   * Delete all nodes in the tree at or below this node.
   *
   * \param pCurr Pointer to the node to delete from.
   */
  void remove(Node *pCurr);

  /**
   * Recursive helper function for operator>>.
   *
   * \param out ostream object to output to, e.g., cout
   * \param pCurr Pointer to current node in the traversal.
   */
  friend void preOrderPrint(std::ostream &out, Node *pCurr) {
    using namespace std;

    if (pCurr != 0) {
      out << pCurr->data << " ";

      typename vector<Tree<T>::Node *>::iterator i = pCurr->children.begin();

      while (i != pCurr->children.end()) {
        preOrderPrint(out, *i);
        i++;
      }
    }
  }
};

//-----------------------------------------------------------
// function implementations
//-----------------------------------------------------------

/*
 * Handle::addChild() implementaiton.
 */
template <class T> void Tree<T>::Handle::addChild(const T &v) {
  pCurr->children.push_back(new Node(v, pCurr));
  pTree->n++;
}

/*
 * Handle::getChild() implementation.
 */
template <class T>
typename Tree<T>::Handle Tree<T>::Handle::getChild(unsigned i) {
  if (i >= pCurr->children.size()) {
    throw std::out_of_range("Indexing invalid child in "
                            "Tree::Handle:: getChild()");
  }

  return Handle(pCurr->children[i], pTree);
}

/*
 * Handle::getParent() implementation
 */
template <class T> typename Tree<T>::Handle Tree<T>::Handle::getParent() {
  if (pCurr->pParent == 0) {
    throw std::out_of_range("Requesting parent of root in "
                            "Tree::Handle::getParent()");
  }

  return Handle(pCurr->pParent, pTree);
}

/*
 * Handle::prune() implementation.
 */
template <class T> void Tree<T>::Handle::prune() {
  using namespace std;

  typename vector<Tree<T>::Node *>::iterator i = pCurr->children.begin();

  while (i != pCurr->children.end()) {
    pTree->remove(*i);
    i++;
  }

  pCurr->children.clear();
}

/*
 * Handle::operator*() implementation.
 */
template <class T> T &Tree<T>::Handle::operator*() {
  if (pCurr == 0) {
    throw std::out_of_range("Dereferencing empty handle in "
                            "Tree::Handle::operator*()");
  }

  return pCurr->data;
}

/*
 * Handle::operator=() implementation.
 */
template <class T>
typename Tree<T>::Handle &Tree<T>::Handle::
operator=(const Tree<T>::Handle &loc) {

  pCurr = loc.pCurr;
  pTree = loc.pTree;

  return *this;
}

/*
 * Tree copy constructor implementation.
 */
template <class T> Tree<T>::Tree(const Tree<T> &tree) {
  using namespace std;

  pRoot = new Tree<T>::Node(tree.pRoot->data, 0);

  typename vector<Tree<T>::Node *>::iterator i = tree.pRoot->children.begin();

  while (i != tree.pRoot->children.end()) {
    pRoot->children.push_back(copy(*i, pRoot));
    i++;
  }

  n = tree.n;
}

/*
 * Tree destructor implementation.
 */
template <class T> Tree<T>::~Tree() { remove(pRoot); }

/*
 * Recursive tree copy function implementation.
 */
template <class T>
typename Tree<T>::Node *Tree<T>::copy(Node *pCurr, Node *pPar) {

  using namespace std;

  Node *pN = new Node(pCurr->data, pPar);

  typename vector<Tree<T>::Node *>::iterator i = pCurr->children.begin();
  while (i != pCurr->children.end()) {
    pN->children.push_back(copy(*i, pN));
    i++;
  }

  return pN;
}

/*
 * Recursive remove function implementation.
 */
template <class T> void Tree<T>::remove(Node *pCurr) {
  using namespace std;

  // base case? non-null node
  if (pCurr != 0) {
    // first, delete all children
    typename vector<Tree<T>::Node *>::iterator i = pCurr->children.begin();

    while (i != pCurr->children.end()) {
      remove(*i);
      i++;
    }

    // then delete this node
    delete pCurr;

    // update size
    n--;
  }
}

/*
 * Overriden assignment operator for Tree class.
 */
template <class T> Tree<T> &Tree<T>::operator=(const Tree<T> &tree) {

  using namespace std;

  // erase current contents
  remove(pRoot);
  n = 0u;

  // copy existing tree's contents
  pRoot = new Node(tree.pRoot->data, 0);

  typename vector<Tree<T>::Node *>::iterator i = tree.pRoot->children.begin();

  while (i != tree.pRoot->children.end()) {
    pRoot->children.push_back(copy(*i, pRoot));
    i++;
  }

  n = tree.n;

  return *this;
}
