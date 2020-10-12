#include "Tree.h"
#include <cstdlib>
#include <iostream>

int main() {

  using namespace std;

  Tree<int> tree1(1);
  
  Tree<int>::Handle loc1 = tree1.rootHandle();
  loc1.addChild(2);
  loc1.addChild(3);
  
  loc1 = loc1.getChild(0);
  loc1.addChild(4);
  loc1.addChild(5);
  loc1.addChild(6);
  
  loc1 = loc1.getChild(0);
  loc1.addChild(7);
  
  loc1 = loc1.getChild(0);
  loc1.addChild(8);
  
  loc1 = loc1.getChild(0);
  loc1.addChild(9);
  
  cout << "Preorder traversal of first tree: " << tree1 << endl;
  cout << "\tHeight = " << tree1.height() << endl;

  cout << "\tThe tree " << (tree1.contains(7) ? "does" : "does not") <<
    " contain 7." << endl;

  cout << "\tThe tree " << (tree1.contains(17) ? "does" : "does not") <<
    " contain 17." << endl;
    
  Tree<char> tree2('a');
  Tree<char>::Handle loc2 = tree2.rootHandle();
  loc2.addChild('b');
  loc2.addChild('c');
  loc2.addChild('d');
  
  loc2 = loc2.getChild(0);
  loc2.addChild('e');
  loc2.addChild('f');
  
  loc2 = loc2.getParent();
  loc2 = loc2.getChild(1);
  loc2.addChild('g');
  
  loc2 = loc2.getChild(0);
  loc2.addChild('j');
  loc2.addChild('k');
  
  loc2 = loc2.getChild(1);
  loc2.addChild('l');
  loc2.addChild('m');
  
  loc2 = loc2.getParent();
  loc2 = loc2.getParent();
  loc2 = loc2.getParent();
  loc2 = loc2.getChild(2);
  loc2.addChild('h');
  loc2.addChild('i');
  
  cout << "Preorder traversal of second tree: " << tree2 << endl;
  cout << "\tHeight = " << tree2.height() << endl;

  cout << "\tThe tree " << (tree2.contains('j') ? "does" : "does not") <<
    " contain 'j'." << endl;

  cout << "\tThe tree " << (tree2.contains('q') ? "does" : "does not") <<
    " contain 'q'." << endl;

  Tree<double> tree3(1.2);

  cout << "Preorder traversal of third tree: " << tree3 << endl;
  cout << "\tHeight = " << tree3.height() << endl;

  cout << "\tThe tree " << (tree3.contains(1.2) ? "does" : "does not") <<
    " contain 1.2." << endl;

  cout << "\tThe tree " << (tree3.contains(2.3) ? "does" : "does not") <<
    " contain 2.3." << endl;

  return EXIT_SUCCESS;
}
