#include <iostream>
using namespace std;

template <typename keytype, typename valuetype>
class BHeap{
    struct Node{
        keytype key;
        valuetype value;
        Node* left;
        Node* sibling;
        Node* parent;
        int degree;
    };
    class DLL{
        //contains a parent node and pointers to next and previous Square structs containg other parents
        //parents stored in order of increasing degree
        struct Square{
            Node* current = nullptr;
            Square* next = nullptr;
            Square* prev = nullptr;
        };
        private:
            //merges nodes with the same degree, making the one with the smaller key the parent
            Square* merge(Square* x, Square* y){
                Node* current = x->current;
                Node* next = y->current;
                Square* temp;
                //sets x->current as y->current's parent and adjusts the other pointers accordingly
                //also removes y from the parent list
                if(current->key < next->key){
                    next->sibling = current->left;
                    current->left = next;
                    next->parent = current;
                    current->degree++;
                    if(y->next != nullptr)
                        y->next->prev = x;
                    x->next = y->next;
                    if(y == tail)
                        tail = x;
                    delete y;
                    currSize--;
                    //this executes if parent nodes degrees are equal
                    if(x->next != nullptr && x->current->degree == x->next->current->degree){
                        temp = x;
                        if(x->next->next != nullptr && x->next->next->current->degree == x->current->degree)
                            temp = x->next;
                        return merge(temp, temp->next);
                    }
                    return x->next;
                }
                //sets x->yurrent as x->current's parent and adjusts the other pointers accordingly
                //also removes x from the parent list
                else{
                    current->sibling = next->left;
                    next->left = current;
                    current->parent = next;
                    next->degree++;
                    if(x->prev != nullptr)
                        x->prev->next = y;
                    y->prev = x->prev;
                    if(x == head)
                        head = y;
                    delete x;
                    currSize--;
                    //this executes if parent nodes degrees are equal
                    if(y->next != nullptr && y->current->degree == y->next->current->degree){
                        temp = y;
                        if(y->next->next != nullptr && y->next->next->current->degree == y->current->degree)
                            temp = y->next;
                        return merge(temp, temp->next);
                    }
                    return y->next;
                }
            }
            //modified inorder traversal for the BHeap
            void printKey(Node* x){
                cout << x->key << " ";
                if(x->left != nullptr)
                    printKey(x->left);
                if(x->sibling != nullptr)
                    printKey(x->sibling);
            }
            //uses a modified preorder traversal to clone the current BTree
            Node* cloneTree(Node* x, Node* parentNode, Node* leftSibling){
                Node* node = new Node();
                node->key = x->key;
                node->value = x->value;
                node->degree = x->degree;
                node->parent = parentNode;
                if(leftSibling != nullptr)
                    leftSibling->sibling = node;
                if(parentNode != nullptr && parentNode->left == nullptr)
                    parentNode->left = node;
                if(x->left != nullptr)
                    cloneTree(x->left, node, nullptr);
                if(x->sibling != nullptr)
                    cloneTree(x->sibling, parentNode, node);
                return node;
            }
            //uses a modified postorder traversal to delete the current BTree
            void destroyHeap(Node* x){
                if(x->left != nullptr)
                    destroyHeap(x->left);
                if(x->sibling != nullptr)
                    destroyHeap(x->sibling);
  
                delete x;
            }
            Square* head;
            Square* tail;
            Square* minNode;
            int currSize;
        public:
            //construstor for the DLL class that sets current size to 0 and all private pointer
            //variables to nullptr
            DLL(){
                head = nullptr;
                tail = nullptr;
                minNode = nullptr;
                currSize = 0;
            }
            //sets the current DLL object to equal l2
            DLL& operator = (const DLL &l2){
                Square* temp = l2.head;
                Square* prev = nullptr;
                currSize = l2.currSize;
                for(int i = 0; i < currSize; i++){
                    Square* square = new Square();
                    square->current = cloneTree(temp->current, nullptr, nullptr);
                    if(i == 0){
                        head = square;
                        tail = square;
                        prev = square;
                    }
                    else{
                        tail = square;
                        prev->next = square;
                        square->prev = prev;
                        prev = square;
                    }
                    temp = temp->next;
                }
                return *this;
            }
            //destroys the DLL
            void destroyList(){
                Square* temp = head;
                while(temp != nullptr){
                    Square* tempSquare = temp;
                    temp = temp->next;
                    delete tempSquare;
                }
                head = nullptr;
                tail = nullptr;
                currSize = 0;
            }
            //inserts parent node x into the DLL
            void insert(Node* x){
                Square* sq = new Square();
                sq->current = x;
                //if the list is empty, set the current square as the head and tail
                if(head == nullptr){
                    head = sq;
                    tail = sq;
                }
                //otherwise sets it as the new head and points to the previous head
                else{
                    head->prev = sq;
                    sq->next = head;
                    head = sq;
                }
                currSize++;
                //if there exists another B0 in the heap, start merging
                if(sq->next != nullptr && sq->current->degree == sq->next->current->degree)
                    merge(sq, sq->next);
            }
            //insert used for the merge public function
            //starts trying to insert parent node x at position y
            Square* insert(Node* x, Square* y){
                //if the list is empty, make x the head and tail of the list
                if(currSize == 0){
                    Square* sq = new Square();
                    sq->current = x;
                    head = sq;
                    tail = sq;
                    currSize++;
                    return sq;
                }
                //if at the end of the list, set x as the new tail
                if(y == nullptr){
                    Square* sq = new Square();
                    sq->current = x;
                    tail->next = sq;
                    sq->prev = tail;
                    tail = sq;
                    currSize++;
                    return sq;
                }
                //if the degree of the x is greater than the degree of the node in y, recurse
                if(x->degree > y->current->degree)
                    return insert(x, y->next);
                //if it is less than or equal, insert a new square struct before y
                else{
                    Square* sq = new Square();
                    sq->current = x;
                    sq->next = y;
                    sq->prev = y->prev;
                    y->prev = sq;
                    if(sq->prev != nullptr)
                        sq->prev->next = sq;
                    if(y == head)
                        head = sq;
                    currSize++;
                    return y;
                }
            }
            //traverses through all of the BTree parent nodes, returning the one with the smallest
            //key
            Node* min(){
                minNode = head;
                Square* temp = head->next;
                while(temp != nullptr){
                    if(temp->current->key < minNode->current->key)
                        minNode = temp;
                    temp = temp->next;
                }
                return minNode->current;
            }
            //extracts the min node and fixes the heap accordingly
            keytype extractMin(){
                min(); //updates minNode value
                int size = minNode->current->degree;
                Node* temp = minNode->current->left;
                keytype minKey = minNode->current->key;
                if(minNode == head){
                    //clears the list if there is only one parent in it
                    if(currSize == 1){
                        delete minNode;
                        head = nullptr;
                        tail = nullptr;
                    }
                    //else, sets the new head as the 2nd node and deletes the first node
                    else{
                        minNode->next->prev = nullptr;
                        head = minNode->next;
                        delete minNode;
                    }
                    currSize--;
                }
                //if the min node is the last parent, set the second to last node as the new tail
                //and delete the last node
                else if(minNode == tail){ 
                    minNode->prev->next = nullptr;
                    tail = minNode->prev;
                    delete minNode;
                    currSize--;
                }
                else{ //element in the middle
                    minNode->prev->next = minNode->next;
                    minNode->next->prev = minNode->prev;
                    delete minNode;
                    currSize--;
                }
                Node* arr [size];
                int count = size - 1;
                Node* tempSibling;
                //builds arr to contain the children parent nodes of the minNode in ascending degree
                while(count >= 0){
                    arr[count] = temp;
                    if(count > 0){
                        tempSibling = temp->sibling;
                        temp->sibling = nullptr;
                        temp = tempSibling;
                    }
                    count--;
                }
                Square* tempSquare;
                //inserts the elements of arr into the DLL
                for(int i = 0; i < size; i++){
                    if(i == 0)
                        tempSquare = insert(arr[0], head);
                    else
                        tempSquare = insert(arr[i], tempSquare);
                }
                //fixes the BHeap to not contain any trees of the same degree
                consolidate();
                return minKey;
            }
            //fixes heap to contain no trees of equal degree
            void consolidate(){
                Square* temp = head;
                while(temp != nullptr && temp->next != nullptr){
                    //merges if two nodes next to each other have an equal degree
                    if(temp->current->degree == temp->next->current->degree){
                        if(temp->next->next != nullptr && temp->next->next->current->degree == temp->current->degree)
                            temp = temp->next;
                        temp = merge(temp, temp->next);
                    }
                    else
                        temp = temp->next;
                }
            }
            //merges l2 with the current BHeap, destroying l2
            void merge(DLL &l2){
                Square* temp = l2.head;
                l2.head = nullptr;
                l2.tail = nullptr;
                l2.currSize = 0;
                Square* tempSquare = head;
                while(temp != nullptr){
                    tempSquare = insert(temp->current, tempSquare);
                    temp = temp->next;
                }
                consolidate();
            }
            //returns the size of the DLL
            int size(){
                return currSize;
            }
            //prints out the contents of the BHeap
            void printKey(){
                Square* temp = head;
                while(temp != nullptr){
                    cout << "B" << temp->current->degree << endl;
                    printKey(temp->current);
                    cout << endl << endl;
                    temp = temp->next;
                }
            }
            //destroys the heap
            void destroyHeap(){
                Square* temp = head;
                while(temp != nullptr){
                    destroyHeap(temp->current);
                    temp = temp->next;
                }
            }
    };
    private:
        DLL list;
        keytype KEY_ERROR_CODE;
        valuetype VALUE_ERROR_CODE;
    public:
        //There is nothing to set inside the default constructor
        BHeap <keytype, valuetype>(){
        }
        //Builds the BHeap via repeated insertion
        BHeap <keytype, valuetype> (keytype k[], valuetype v[], int s){  
            for(int i = 0; i < s; i++)
                insert(k[i], v[i]);
        }
        //if the size of the Bheap is greater than 0, this destroys it
        ~BHeap <keytype, valuetype> (){
            if(list.size() > 0){
                list.destroyHeap();
                list.destroyList();
            }
        }
        //sets the current BHeap to be equal to b2
        BHeap(const BHeap &b2){
            list = b2.list;
        }
        //destroys the current BHeap and sets it to be equal to b2
        BHeap& operator = (const BHeap &b2){
            if(list.size() > 0){
                list.destroyHeap();
                list.destroyList();
            }
            list = b2.list;
            return *this;
        }
        //returns the smallest key in the BHeap
        keytype peakKey(){
            if(list.size() == 0)
                return KEY_ERROR_CODE;
            return list.min()->key;
        }
        //returns the value associated with the smallest key in the Bheap
        valuetype peakValue(){
            if(list.size() == 0)
                return VALUE_ERROR_CODE;
            return list.min()->value;
        }
        //extracts the min from the BHeap
        keytype extractMin(){
            if(list.size() > 0)
                return list.extractMin();
            return KEY_ERROR_CODE;
        }
        //merges the current BHeap with H2, destroying H2 in the process
        void merge(BHeap<keytype, valuetype> &H2){
            if(H2.list.size() > 0)
                list.merge(H2.list);
        }
        //inserts a node with keytype k and valuetype v into the current BHeap
        void insert(keytype k, valuetype v){
            Node* node = new Node();
            node->key = k;
            node->value = v;
            node->degree = 0;
            list.insert(node);
        }
        //prints the current contents of the BHeap
        void printKey(){
            list.printKey();
        }
};