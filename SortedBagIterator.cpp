#include "SortedBagIterator.h"
#include "SortedBag.h"
#include <exception>

using namespace std;

SortedBagIterator::SortedBagIterator(const SortedBag& b) : bag(b) {
    this->currentNode = nullptr;
    this->currentPosition = 0;
    this->stack.size = 0;
    this->stack.array = new BSTNode*[this->bag.length];

    InOrderTraversal(this->bag.root);
    if (this->stack.size != 0)
        this->currentNode = this->stack.array[this->currentPosition];
    else
        this->currentNode = nullptr;
} // O(n)

TComp SortedBagIterator::getCurrent() {
    if (!valid())
        throw exception();
    return this->currentNode->info.first;
} //theta(1)

bool SortedBagIterator::valid() {
    if (this->currentPosition >= this->stack.size)
        return false;
    return true;
} // theta(1)

void SortedBagIterator::next() {
    if (!valid())
        throw exception();
    this->currentPosition++;
    this->currentNode = this->stack.array[this->currentPosition];
} // theta(1)

void SortedBagIterator::first() {
    this->currentPosition = 0;
    this->currentNode= this->stack.array[0];
} // theta(1)

void SortedBagIterator::InOrderTraversal(BSTNode *node) {
    auto** auxiliaryStack = new BSTNode * [this->bag.length];
    int index = 0;
    BSTNode* currentNodeStack = node;

    // push to stack all left nodes
    while (currentNodeStack != nullptr)
    {
        auxiliaryStack[index] = currentNodeStack;
        index++;
        currentNodeStack = currentNodeStack->left;
    }

    // while stack is not empty
    while (index != 0)
    {
        // pop from stack
        currentNodeStack = auxiliaryStack[index - 1];
        index--;
        // put in the stack the most left node at that moment n times
        for(int i=0; i<currentNodeStack->info.second; i++)
            this->stack.array[this->stack.size++]=currentNodeStack;
        // visit current node (add the left child nodes from this parent)
        currentNodeStack = currentNodeStack->right;
        while (currentNodeStack != nullptr)
        {
            // push
            auxiliaryStack[index] = currentNodeStack;
            index++;
            currentNodeStack = currentNodeStack->left;
        }
    }
}
//Time complexity: O(n)
//extra space complexity: O(n);


void SortedBagIterator::previous() {
    if (!valid())
        throw exception();

    if(this->currentPosition == 0)
        this->currentPosition = this->stack.size; //it's invalid
    else {
        this->currentPosition--;
        this->currentNode = this->stack.array[this->currentPosition];
    }
} // theta(1)
