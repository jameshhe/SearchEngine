//"Owner": James Huang
#ifndef AVLTREE_H
#define AVLTREE_H
#include <iostream>
#include <fstream>
using namespace std;


template <class T>
class AVLTree{
private:
    template <class U>
    class AVLNode{
    public:
        U element;
        AVLNode<U> *left;
        AVLNode<U> *right;
        int height;
        AVLNode(const U &theElement, AVLNode<U> *lt = nullptr, AVLNode<U> *rt = nullptr, int h = 0): element(theElement), left(lt), right(rt), height(h){}
    };

    AVLNode<T> *root;
    int totalNodes;
public:
    AVLTree():root(nullptr),totalNodes(0){}
    AVLTree(const AVLTree<T> &rhs):root(nullptr){
        *this = rhs;
    }
    AVLTree<T>& operator=(const AVLTree<T> &rhs){
        if(this != &rhs){
            makeEmpty();
            root = clone(rhs.root);
        }
        return *this;
    }

    ~AVLTree(){
        makeEmpty();
    }
    //find smallest item in the tree, throw underflowException if empty
    const T& findMin() const{
        if(isEmpty()){
            throw std::underflow_error("The tree is empty! Cannot find the minimum value");
        }
        return findMin(root)->element;
    }
    //find largest item in the tree, throw underflow exception if empty
    const T& findMax() const{
        if(isEmpty()){
            throw std::underflow_error("The tree is empty! Cannot find the maximum value");
        }
        return findMax(root)->element;
    }
    //returns true if x is found in the tree
    bool contains(const T &x) const{
        return contains(x,root);
    }
    //test if the tree is logically empty
    bool isEmpty() const{
        return root == nullptr;
    }
    //make the tree logically empty
    void makeEmpty(){
        makeEmpty(root);
        totalNodes = 0;
    }
    //insert x into the tree; duplicates are ignored
    void insert(const T &x){
        insert(x,root);
        totalNodes++;
    }
    //print the entire tree
    void printTree(){
        printTree(root);
    }

    T& getNode(T &x){
        return getNode(x,root);
    }

    //get the totalNumber of nodes
    int getTotalNodes(){
        return totalNodes;
    }

    void saveToFile(ofstream& output){
        saveToFile(output, root);
    }
private:
    //return the height of node t or -1 if null
    int height(AVLNode<T> *t) const{
        return t == nullptr ? -1 : t->height;
    }

    AVLNode<T>* clone(AVLNode<T> *t) const{
        if(t != nullptr){
            return new AVLNode<T>(t->element,clone(t->left),clone(t->right),t->height);
        }
        else return nullptr;
    }

    int max(int lhs, int rhs) const{
        return lhs > rhs ? lhs: rhs;
    }

    //internal method to find the minimum value of a node
    AVLNode<T>* findMin(AVLNode<T>* t) const{
        if(t == nullptr){
            return nullptr;
        }
        if(t->left == nullptr){
            return t;
        }
        return findMin(t->left);
    }

    //internal method to find the maximum value of a node
    AVLNode<T>* findMax(AVLNode<T>* t) const{
        if(t == nullptr){
            return nullptr;
        }
        if(t->right == nullptr){
            return t;
        }
        return findMax(t->right);
    }

    //internal method to find if the tree contains a certain value
    bool contains(const T &x, AVLNode<T> *t) const{
        //if our traversal results in an empty node, meaning that there is no match
        if(t == nullptr){
            return false;
        }
        //if x is less than the current node's value
        //traver down the left subtree
        else if(x < t->element){
            return contains(x,t->left);
        }
        else if(x > t->element){
            return contains(x,t->right);
        }
        //if x is neither greater than nor less than the current node, return true
        else return true;
    }

    //internal method to make the tree empty
    void makeEmpty(AVLNode<T> *&t) const{
        //if t is not a nullptr
        //traverse down the left and right subtree to delete all the nodes
        //also delete t
        if(t != nullptr){
            makeEmpty(t->left);
            makeEmpty(t->right);
            delete t;
        }
        //make t a nullptr
        t = nullptr;
    }

    T& getNode(T &x, AVLNode<T> *t){
        //if we cannot find this element
        if(t == nullptr){
            throw std::runtime_error("Requested node does not exist!");
        }
        else if(x == t->element){
            return t->element;
        }
        else if(x < t->element){
            getNode(x,t->left);
        }
        else if(x > t->element){
            getNode(x,t->right);
        }

    }
    //internal method to print the tree
    void printTree(AVLNode<T> *t) const{
        if(t!=nullptr){
            //traverse down the left subtree to print out all the elements
            printTree(t->left);
            //print out t
            cout << t->element << endl;
            //then traverse down the right subtree to print out all the elements
            printTree(t->right);
        }
    }

    //add all the data to an ostream object in addToFile() first and then save to file
    //save the entire tree to an output file
    void saveToFile(ofstream& output, AVLNode<T> *t) const{
            addToFile(t,output);
    }

    void addToFile(AVLNode<T> *t, ostream& o) const{
        //recursively add all the elements to file
        if(t != nullptr){
            addToFile(t->left,o);
            //write to output
            o << t->element << endl;
            addToFile(t->right,o);
        }
    }

    //internal method to insert into a subtree
    //sets the new root of the subtree
    void insert(const T&, AVLNode<T>*&);
    //rotate binary tree node with left child (case 1)
    void rotateWithLeftChild(AVLNode<T>*&);
    //rotate binary tree node with right child (case 4)
    void rotateWithRightChild(AVLNode<T>*&);
    //double rotate binary tree node (case 2)
    void doubleWithLeftChild(AVLNode<T>*&);
    //double rotate binary tree node (case 3)
    void doubleWithRightChild(AVLNode<T>*&);

};


//internal method to insert into a subtree
//x is the item to insert
//t is the node that roots the subtree
//sets the new root of the subtree
template <class T>
void AVLTree<T>::insert(const T &x, AVLNode<T> *&t){
    // t is null, meaning that it's time to insert
    if(t == nullptr){
        t = new AVLNode<T>(x);
    }
    //if x is less than t's value, traverse down the left subtree until there's an opening(nullptr)
    else if(x < t->element){
        insert(x,t->left);
        //if the tree is heavier on the left, meaning we have either a case 1 or a case 2
        if(height(t->left) - height(t->right) == 2){
            //if it's a case 1
            if(x < t->left->element){
                rotateWithLeftChild(t);
            }
            //else (if it's a case 3)
            else doubleWithLeftChild(t);
        }
    }
    //if x is greater than t's value, traverse down the right subtree until there's an opening(nullptr)
    else if(x > t->element){
        insert(x,t->right);
        //if the tree is heavier on the right, meaning we have either a case 3 or a case 4
        if(height(t->right) - height(t->left) == 2){
            //if it's a case 4
            if(x > t->right->element){
                rotateWithRightChild(t);
            }
            //if it's a case 3
            else doubleWithRightChild(t);
        }
    }
    //Ignore the duplicate
    else{

    }
    //update the height of tree at the end
    t->height = max(height(t->left),height(t->right));
}

template <class T>
void AVLTree<T>::rotateWithLeftChild(AVLNode<T> *&k2){
    //set k2's left child to k1
    AVLNode<T> *k1 = k2->left;
    //rotate k2 and k1 by setting k2's left to k1's right and k1's right to k2
    k2->left = k1->right;
    k1->right = k2;
    //update heights of k1 and k2
    k2->height = max(height(k2->left),height(k2->right));
    k1->height = max(height(k1->left),k2->height);
    //reset k2 by setting it to k1
    k2 = k1;
}

template <class T>
void AVLTree<T>::rotateWithRightChild(AVLNode<T> *&k2){
    //set k2's right child to k1
    AVLNode<T> *k1 = k2->right;
    //rotate k2 and k1 by setting k2's right to k1's left and k1's left to k2
    k2->right = k1->left;
    k1->left = k2;
    //update heights of k1 and k2
    k2->height = max(height(k2->left),height(k2->right));
    k1->height = max(k2->height,height(k1->right));
    //reset k2 by setting it to k1
    k2 = k1;
}

template <class T>
void AVLTree<T>::doubleWithLeftChild(AVLNode<T> *&k3){
    //left rotate k3's left child to simplify the problem to a case 1
    rotateWithRightChild(k3->left);
    //right rotate once the tree is in a case 1 situation
    rotateWithLeftChild(k3);
}

template <class T>
void AVLTree<T>::doubleWithRightChild(AVLNode<T> *&k3){
    //right rotate k3's right child to simplify the problem to a case 4
    rotateWithLeftChild(k3->right);
    //once the problem is in case 4, left rotate
    rotateWithRightChild(k3);
}



#endif // AVLTREE_H
