#include <iostream>
#include "SortedBag.h"
#include "SortedBagIterator.h"

SortedBag::SortedBag(Relation r) {
    this->root = nullptr;
    this->r = r;
    this->length = 0;
} // theta(1)

void SortedBag::add(TComp e) {
	// case 1: add root
    if (this->length == 0) {
        this->root = initNode(e);  this->length++; return;
    }

    BSTNode* currentNode = this->root;
    while (true) {
        // case 2: element already exists
        if (currentNode->info.first == e){
            currentNode->info.second++; this->length++; return;
        }
        // case 3: you add to the left where is empty
        if (!this->r(currentNode->info.first, e) && currentNode->left == nullptr) { //ex: 3<2 false
            BSTNode* new_node = initNode(e); currentNode->left = new_node;
            this->length++;
            return;
        }

        // case 4: you add to the right where is empty
        if (this->r(currentNode->info.first, e) && currentNode->right == nullptr)
        {
            BSTNode* new_node = initNode(e); currentNode->right = new_node;
            this->length++;
            return;
        }

        // go to next node in the left tree
        if (!this->r(currentNode->info.first, e) && currentNode->left != nullptr)
            currentNode = currentNode->left;
        // go to next node in the right tree
        else if (this->r(currentNode->info.first, e) && currentNode->right != nullptr)
            currentNode = currentNode->right;
    }
}
//BC: we want to add the first node, which will became the root
//WC: we want to add a node on the last level in the tree
//=>O(n)


bool SortedBag::delete_rec(BSTNode* node, BSTNode* parent, TComp elem)
{
    if (node == nullptr)
        return false;

    // if element has frequency > 1
    if (node->info.first == elem && node->info.second!=1)
        node->info.second--;

    // if element has frequency = 1
    if (node->info.first == elem && node->info.second==1) {
        // case1: The node to be removed has no descendant
        if (node->left == nullptr && node->right == nullptr) {
            // if it's only the root in the tree
            if (parent == nullptr) {
                delete node;
                this->root = nullptr;
                return true;
            }

            // else remove from parent
            if(node==parent->left) {
                parent->left = nullptr;
                delete node;
                return true;
            }
            if (node == parent->right) {
                parent->right = nullptr;
                delete node;
                return true;
            }
        }

        // case2: The node to be removed has one descendant (left)
        if (node->left != nullptr && node->right == nullptr) {
            // if it's root
            if (parent == nullptr) {
                this->root = node->left;
                delete node;
                return true;
            }
            if (parent->left == node) {
                parent->left = node->left;
                delete node;
                return true;
            }
            if (parent->right == node) {
                parent->right = node->left;
                delete node;
                return true;
            }
        }

        // case3: The node to be removed has one descendant (right)
        if (node->left == nullptr && node->right != nullptr) {
            // if it's root
            if (parent == nullptr) {
                this->root = node->right;
                delete node;
                return true;
            }
            if (parent->left == node) {
                parent->left = node->right;
                delete node;
                return true;
            }
            if (parent->right == node) {
                parent->right = node->right;
                delete node;
                return true;
            }
        }

        // case4: The node to be removed has two descendants
        if (node->left != nullptr && node->right != nullptr) {
            // get the maximum value from the left tree of the node
            BSTNode *maximum_node = maximum(node);
            TElem value = maximum_node->info.first;
            int freq = maximum_node->info.second;

            // delete the maximum node
            maximum_node->info.second=1;
            delete_rec(this->root, nullptr, value);

            // put the maximum node in the current node
            node->info.first = value;
            node->info.second = freq;
            return true;
        }
    }

    // it wasn't found yet
    if (!this->r(node->info.first, elem))
        delete_rec(node->left, node,elem);
    else delete_rec(node->right,node, elem);

    return false; // just so the } is not yellow
} //best case: O(n); worst case O(n^2) -> case4

BSTNode* SortedBag::maximum(BSTNode* node)
{
    if (node == nullptr)
        return nullptr;

    node = node->left;

    // start going to the right
    while (node->right != nullptr)
        node = node->right;
    return node;
} // O(n)

bool SortedBag::remove(TComp e) {
    if(!search(e))
        return false;

    if (delete_rec(this->root, nullptr, e))
    {
        this->length--;
        return true;
    }
    else return false;
} // same as delete_rec


bool SortedBag::search(TComp elem) const {
    BSTNode* node = this->root;
    // case 1: tree doesn't exist
    if (node == nullptr)
        return false;

    while (true) {
        // case 2: element was found
        if (node->info.first == elem)
            return true;
        // case 3: it should be in the left but it isn't => next left node
        if (!this->r(node->info.first, elem) && node->left != nullptr) {
            node = node->left; continue; }
        // case 4: it should be in the right but it isn't => next right node
        if (this->r(node->info.first, elem) && node->right != nullptr) {
            node = node->right; continue; }
        // case 5: it reached the end on the left and it wasn't found
        if (!this->r(node->info.first, elem) && node->left == nullptr)
            return false;
        // case 6: it reached the end on the right and it wasn't found
        if (this->r(node->info.first, elem) && node->right == nullptr)
            return false;
        }
} // O(n)


int SortedBag::nrOccurrences(TComp elem) const {
    BSTNode* currentNode = this->root;
    // if it doesn't exist just return 0
    if (!this->search(elem))
        return 0;

    while (true) {
        if (currentNode->info.first == elem) {
            return currentNode->info.second;
        }
        // go to next node in the left tree
        if (!this->r(currentNode->info.first, elem))
            currentNode = currentNode->left;
        // go to next node in the right tree
        else if (this->r(currentNode->info.first, elem))
            currentNode = currentNode->right;
    }
} // O(n) for search * O(n) for while => O(n^2)



int SortedBag::size() const {
    return this->length;
} // theta(1)


bool SortedBag::isEmpty() const {
    if (this->root == nullptr)
        return true;
    return false;
} // theta(1)


SortedBagIterator SortedBag::iterator() const {
	return SortedBagIterator(*this);
}


SortedBag::~SortedBag() {
    //InOrder traversal , removing every node
    auto** arr = new BSTNode * [this->length];
    int currentPos = 0;
    BSTNode* currentNodee = this->root;
    BSTNode* nodeToDelete;
    while (currentNodee != nullptr)
    {
        arr[currentPos] = currentNodee;
        currentPos++;
        currentNodee = currentNodee->left;
    }

    while (currentPos != 0)
    {
        currentNodee = arr[currentPos - 1];
        currentPos--;
        nodeToDelete  = currentNodee;

        currentNodee = currentNodee->right;
        delete nodeToDelete;
        while (currentNodee != nullptr)
        {
            arr[currentPos] = currentNodee;
            currentPos++;
            currentNodee = currentNodee->left;
        }
    }

} // O(n)

BSTNode* SortedBag::initNode(TElem e) const
{
    auto* node=new BSTNode;
    node->info.first = e;
    node->info.second = 1;
    node->left = nullptr;
    node->right = nullptr;
    return node;

} //theta(1)