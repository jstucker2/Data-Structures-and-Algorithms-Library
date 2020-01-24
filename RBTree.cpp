#include <iostream>
using namespace std;

template <typename keytype, typename valuetype>
class RBTree{
    struct Node{
        keytype key;
        valuetype value;
        Node* left;
        Node* right;
        Node* parent;
        bool color;
        int count;
    };
    public:
        RBTree<keytype, valuetype>(){
            currSize = 0;
        }
        RBTree<keytype, valuetype>(keytype keys[], valuetype values[], int s){
            currSize = 0;
            for(int i=0; i < s; i++)
                insert(keys[i], values[i]);
        }
        ~RBTree<keytype, valuetype>(){
            if(currSize > 0)
                destroyTree(parentNode);
        }
        RBTree(const RBTree &r2){
            currSize = r2.currSize;
            Node* node = new Node;
            if(currSize > 0){
                node->key = r2.parentNode->key;
                node->value = r2.parentNode->value;
                node->count = r2.parentNode->count;
                node->color = r2.parentNode->color;
                node->parent = nullptr;
                parentNode = node;
                if(r2.parentNode->left != nullptr)
                    rebuildTree(r2.parentNode->left, node);
                if(r2.parentNode->right != nullptr)
                    rebuildTree(r2.parentNode->right, node);
            }
        }
        RBTree& operator = (const RBTree &r2){ //check
            if(currSize > 0)
                destroyTree(parentNode);
            currSize = r2.currSize;
            Node* node = new Node;
            if(currSize > 0){
                node->key = r2.parentNode->key;
                node->value = r2.parentNode->value;
                node->count = r2.parentNode->count;
                node->color = r2.parentNode->color;
                node->parent = nullptr;
                parentNode = node;
                if(r2.parentNode->left != nullptr)
                    rebuildTree(r2.parentNode->left, node);
                if(r2.parentNode->right != nullptr)
                    rebuildTree(r2.parentNode->right, node);
            }
        }
        valuetype* search(keytype x){
            if(currSize > 0)
                return search(parentNode, x);
            return NULL;
        }
        void insert(keytype x, valuetype v){ 
            Node* node = new Node;
            node->key = x;
            node->value = v;
            node->count = 1;
            if(currSize == 0){ //sets node as the parent of the tree
                node->color = BLACK;
                parentNode = node;
            }
            else{
                node->color = RED;
                currentNode = binaryTreeSearch(parentNode, x);
                if(currentNode->key == node->key){
                    currentNode->value = v;
                    delete node;
                    return;
                }
                node->parent = currentNode;
                if(node->key > currentNode->key)
                    currentNode->right = node;
                else
                    currentNode->left = node;
                fixTree(currentNode);
            }
            currSize++;  
        }
        int remove(keytype x){
            if(currSize > 0){
                if(searchRemove(parentNode, x)){ //returns true if key exists in tree
                    remove(parentNode, x);
                    return 1;
                }
            }
            return 0;
        }
        int rank(keytype x) {
            if(currSize == 0)
                return 0;
            currentNode = parentNode;
            int counter = 0;
            if(currentNode->right != nullptr)
                counter = currentNode->count - currentNode->right->count;
            else
                counter = currentNode->count;
            while(1){
                if(x < currentNode->key){
                    if(currentNode->left != nullptr){
                        currentNode = currentNode->left;
                        if(currentNode->right != nullptr)
                            counter = counter - currentNode->right->count - 1;
                        else
                            counter--;
                    }
                    else
                        return 0;
                }
                else if(x > currentNode->key){
                    if(currentNode->right != nullptr){
                        currentNode = currentNode->right;
                        if(currentNode->left != nullptr)
                            counter = counter + currentNode->left->count + 1;
                        else
                            counter++;
                    }
                    else
                        return 0;
                }
                else{ //element we are looking for
                    return counter;
                }
            }
        }
        keytype select(int pos){ 
            if(pos > 0 && pos <= currSize){
                currentNode = parentNode;
                int counter = 0;
                if(currentNode->right != nullptr)
                    counter = currentNode->count - currentNode->right->count;
                else
                    counter = currentNode->count;
                while(1){
                    if(pos < counter){
                        currentNode = currentNode->left;
                        if(currentNode->right != nullptr)
                            counter = counter - currentNode->right->count - 1;
                        else
                            counter--;
                    }
                    else if(pos > counter){
                        currentNode = currentNode->right;
                        if(currentNode->left != nullptr)
                            counter = counter + currentNode->left->count + 1;
                        else
                            counter++;
                    }
                    else //element we are looking for
                        return currentNode->key;
                }
            }
            else
                return ERROR_CODE;
        }
        void split(keytype k, RBTree<keytype, valuetype>& T1, RBTree<keytype, valuetype>& T2){
            Node* node = new Node;
            node->key = k;
            node->count = 0;
            node->color = RED;
            currentNode = binaryTreeSearch(parentNode, k);
            node->parent = currentNode;
            if(currentNode != nullptr){
                if(node->key > currentNode->key)
                    currentNode->right = node;
                else
                    currentNode->left = node;
            }
            while(node->parent != nullptr){
                if(node->parent->key < node->key)
                    node = rotateLeft(node->parent);
                else
                    node = rotateRight(node->parent);
            }
            T1.parentNode = node->left;
            T2.parentNode = node->right;
            T1.currSize = T1.parentNode->count;
            T2.currSize = T2.parentNode->count;
            parentNode->left = nullptr;
            parentNode->right = nullptr;
            currSize = 1;
        }
        void preorder(){
            if(currSize > 0){
                preorder(parentNode);
                cout << endl;
            }
        }
        void inorder(){
            if(currSize > 0){
                inorder(parentNode);
                cout << endl;
            }
        }
        void postorder(){
            if(currSize > 0){
                postorder(parentNode);
                cout << endl;
            }
        }
        int size(){
            return currSize;
        }
    private:
        void remove(Node* current, keytype key){ 
            if(key < current->key){
                if(!isRed(current->left) && current->left != nullptr && !isRed(current->left->left)) 
                    current = pushRedLeft(current);
                if(current->left != nullptr)
                    remove(current->left, key);
            }
            else{
                if(isRed(current->left))
                    current = rotateRight(current);
                if(current->key == key && current->right == nullptr){ 
                    if(currSize == 1){
                        delete current;
                        currSize--;
                        return;
                    }
                    Node* temp = current->parent;
                    if(current->key <= temp->key)
                        temp->left = nullptr;
                    else
                        temp->right = nullptr;
                    delete current;
                    fixTreeDelete(temp); 
                    currSize--;
                    return;
                }
                if(!isRed(current->right) && !isRed(current->right->left))
                    current = pushRedRight(current);
                if(current->key == key){
                    Node* temp = removeMin(current->right);
                    Node* tempParent = temp->parent;
                    current->key = temp->key;
                    current->value = temp->value;
                    
                    if(temp->key < tempParent->key)
                        tempParent->left = nullptr;
                    else
                        tempParent->right = nullptr;
                    fixTreeDelete(tempParent);
                    currSize--;
                    return;
                }
                else
                    if(current->right != nullptr)
                        remove(current->right, key);
            }
        }
        Node* removeMin(Node* x){
            if(!isRed(x->left) && x->left != nullptr && !isRed(x->left->left))
                x = pushRedLeft(x);
            if(x->left != nullptr)
                removeMin(x->left);
            else
                return x;
        }
        void colorFlip(Node* x){
            x->color = !x->color;
            x->left->color = !x->left->color;
            x->right->color = !x->right->color;
            if(x == parentNode)
                x->color = BLACK;
        }
        Node* pushRedLeft(Node* x){
            colorFlip(x);
            Node* temp = x;
            if(x->right->left != nullptr && isRed(x->right->left)){
                temp = x->right;
                temp = rotateRight(temp);
                temp = rotateLeft(temp->parent);
                colorFlip(temp);
            }

            return temp;
        }
        Node* pushRedRight(Node* x){
            colorFlip(x);
            Node* temp = x;
            if(x->left->left != nullptr && isRed(x->left->left)){
                temp = rotateRight(x);
                colorFlip(temp);
            }
            return temp;
        }
        bool hasTwoChildren(Node* x){
            if(x->left != nullptr && x->right != nullptr)
                return true;
            return false;
        }
        bool isRed(Node* x){
            if(x != nullptr && x->color == RED)
                return true;
            return false;
        }
        void preorder(Node* x){
            if(x == nullptr)
                return;
            cout << x->key << " ";
            preorder(x->left);
            preorder(x->right);
        }
        void inorder(Node* x){
            if(x == nullptr)
                return;
            inorder(x->left);
            cout << x->key << " ";
            inorder(x->right);
        }
        void postorder(Node* x){
            if(x == nullptr)
                return;
            postorder(x->left);
            postorder(x->right);
            cout << x->key << " ";
        }
        Node* binaryTreeSearch(Node* x, keytype key){
            if(hasTwoChildren(x) && isRed(x->left) && isRed(x->right))
                colorFlip(x);
            if(x->key < key){
                if(x->right != nullptr)
                    return binaryTreeSearch(x->right, key);
                else
                    return x;
            }
            else if(x->key > key){
                if(x->left != nullptr)
                    return binaryTreeSearch(x->left, key);
                else
                    return x;
            }
            else
                return x;
        }
        valuetype* search(Node* x, keytype key){
            if(key < x->key){
                if(x->left != nullptr)
                    return search(x->left, key);
                else
                    return NULL;
            }
            else if(key > x->key){
                if(x->right != nullptr)
                    return search(x->right, key);
                else
                    return NULL;
            }
            else
                return &(x->value);
        }
        Node* rotateLeft(Node* x){ //check if done
            bool tempColor = x->color; //color switch
            x->color = x->right->color;
            x->right->color = tempColor;

            Node* tempRight = x->right; //right child of current
            if(x->right != nullptr)
                x->count -= x->right->count;
            if(x->right->left != nullptr)
                x->right->left->parent = x;
            x->right = x->right->left;
            
            if(x->right != nullptr)
                x->count += x->right->count;
            if(tempRight->left != nullptr)
                tempRight->count -= tempRight->left->count;
            tempRight->count += x->count;
            tempRight->left = x;
            
            tempRight->parent = x->parent;
            x->parent = tempRight;

            if(x == parentNode)
                parentNode = tempRight;
            else{
                if(x->key <= tempRight->parent->key)
                    tempRight->parent->left = tempRight;
                else
                    tempRight->parent->right = tempRight;
            }

            return tempRight;
        }
        Node* rotateRight(Node* x){ //copied and inverted from rotateLeft, so might be wrong
            bool tempColor = x->color;
            x->color = x->left->color;
            x->left->color = tempColor;

            Node* tempLeft = x->left;
            if(x->left != nullptr)
                x->count -= x->left->count;
            if(x->left->right != nullptr)
                x->left->right->parent = x;
            x->left = x->left->right;
            if(x->left != nullptr)
                x->count += x->left->count;
            if(tempLeft->right != nullptr)
                tempLeft->count -= tempLeft->right->count;
            tempLeft->count += x->count;
            tempLeft->right = x;
            
            tempLeft->parent = x->parent;
            x->parent = tempLeft;

            if(x == parentNode)
                parentNode = tempLeft;
            
            else{
                if(x->key <= tempLeft->parent->key)
                    tempLeft->parent->left = tempLeft;
                else
                    tempLeft->parent->right = tempLeft;
            }
            return tempLeft;
        }
        void fixTree(Node* x){
            x->count++;
            if((x->left == nullptr || x->left->color == BLACK) && (x->right != nullptr && x->right->color == RED))
                x = rotateLeft(x);
            else if(x->left != nullptr && x->left->left != nullptr)
                if(x->left->color == RED && x->left->left->color == RED)
                    x = rotateRight(x);
            if(x->parent != nullptr)
                fixTree(x->parent);
        }
        void fixTreeDelete(Node* x){
            x->count--; 
            if((x->left == nullptr || x->left->color == BLACK) && (x->right != nullptr && x->right->color == RED))
                x = rotateLeft(x);
            else if(x->left != nullptr && x->left->left != nullptr)
                if(x->left->color == RED && x->left->left->color == RED)
                    x = rotateRight(x);
            if(hasTwoChildren(x) && isRed(x->left) && isRed(x->right))
                colorFlip(x);
            if(x->parent != nullptr)
                fixTreeDelete(x->parent);
        }
        void destroyTree(Node* x){
            if(x->left != nullptr)
                destroyTree(x->left);
            if(x->right != nullptr)
                destroyTree(x->right);
            delete x;
        }
        void rebuildTree(Node* current, Node* past){
            Node* node = new Node;
            node->key = current->key;
            node->value = current->value;
            node->count = current->count;
            node->color = current->color;
            node->parent = past;
            if(node->key > past->key)
                past->right = node;
            else
                past->left = node;
            if(current->left != nullptr)
                rebuildTree(current->left, node);
            if(current->right != nullptr)
                rebuildTree(current->right, node);
        }
        bool searchRemove(Node* x, keytype key){
            if(x->key == key)
                return true;
            if(key < x->key){
                if(x->left != nullptr)
                    return searchRemove(x->left, key);
                return false;
            }
            else{
                if(x->right != nullptr)
                    return searchRemove(x->right, key);
                return false;
            }
        }
        Node* parentNode;
        Node* currentNode;
        bool RED = true;
        bool BLACK = false;
        int currSize;
        keytype ERROR_CODE;
};