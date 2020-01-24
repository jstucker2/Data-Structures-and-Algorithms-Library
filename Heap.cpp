#include <iostream>
using namespace std;

template <typename keytype, typename valuetype>
class Heap{
    template <typename T>
    // CDA class is used for storing the heap
    class CircularDynamicArray{
        private:

        //copies current array over to a new one that is twice the size
        void grow(){
            maxSize *= 2; 
            T *temp = new T[maxSize];
            for(int i = 0; i < maxSize / 2; i++)
                temp[i] = arr[i];
            delete [] arr;
            arr = NULL;
            arr = temp;
            currSize++;
            head = 0;
            tail = currSize - 1;
        }

        //copies current array over to a new one that is half the size
        void shrink(){
            maxSize /= 2;
            T *temp = new T[maxSize];
            for(int i = 0; i <= tail; i++)
                temp[i] = arr[i];
            delete [] arr;
            arr = NULL;
            arr = temp;
        }

        T *arr;
        T ERROR_CODE;
        int currSize;
        int maxSize;
        int head;
        int tail;

        public:

        //This is the default constructor for the CDA class
        CircularDynamicArray <T>(){
            arr = new T[2];
            maxSize = 2;
            currSize = 0;
            head = 0;
            tail = 0;
        }
        //This is the destructor for the CDA class
        ~CircularDynamicArray <T>(){
            delete [] arr;
        }
        //deletes current array and copies over the CDA parameter to the current class
        CircularDynamicArray& operator = (const CircularDynamicArray &c2){ //check
            delete [] arr;
            arr = NULL;
            maxSize = c2.maxSize;
            currSize = c2.currSize;
            head = c2.head;
            tail = c2.tail;
            arr = new T[maxSize];
            for(int i = 0; i < maxSize; i++)
                arr[i] = c2.arr[i];
            return *this;
        }
        //adds input to the end of the current array, adjusting size and tail
        void addEnd(T input){
            //copys the array and doubles it if it is full
            if(currSize == maxSize){
                grow();
                arr[tail] = input;
            }
            else{
                if(currSize > 0)
                    tail++;
                arr[tail] = input;
                currSize++;
            }
        }
        //replaces the first element in the array with the last, decrementing currSize
        void delFront(){
            //can only delete if the array has elements in it
            if(currSize > 0){
                currSize--;
                arr[0] = arr[tail];
                tail--;
                //if 25% or less of the array is being used, shrink it
                if(currSize * 4 <= maxSize && currSize != 0)
                    shrink();
            }
        }
        //returns the number of elements in the CDA
        int length(){
            return currSize;
        }
        //returns the total number of elements that the CDA can currently hold
        int capacity(){
            return maxSize;
        }
        //clears all values out of the CDA and resets maxSize to 2
        void clear(){
            delete [] arr;
            arr = NULL;
            arr = new T[2];
            maxSize = 2;
            currSize = 0;
            head = 0;
            tail = 0;
        }
        //[] overloader
        T& operator[](int index){ //returns
            if (index >= currSize || index < 0) 
            { 
                cout << "Out of bounds reference: " << index << endl; 
                return ERROR_CODE;
            } 
            return arr[(head + index) % maxSize];
        }
    };

    private:
        //swaps the elements in fpos and spos
        void swap(int fpos, int spos){
            keytype temp = keys[fpos];
            keys[fpos] = keys[spos];
            keys[spos] = temp;

            valuetype tempVal = values[fpos];
            values[fpos] = values[spos];
            values[spos] = tempVal;
        }
        //checks if either if a nodes two children are less than it.
        //if one of them are, the smallest node becomes the parent and fixHeapDown recurses.
        void fixHeapDown(int pos){
            int minIndex = pos;
            int leftPos = pos*2 + 1;
            int rightPos = pos*2 + 2;
            if(leftPos < currHeapSize && keys[leftPos] < keys[minIndex])
                minIndex = leftPos;
            if(rightPos < currHeapSize && keys[rightPos] < keys[minIndex])
                minIndex = rightPos;
            if(minIndex != pos){
                swap(pos, minIndex);
                fixHeapDown(minIndex);
            }
        }
        //checks if the current node is greater than its parent.
        //if so, it swaps with the parent and fixHeapUp recurses.
        void fixHeapUp(int pos){
            int tempParent = (pos-1)/2;
            if(keys[pos] < keys[tempParent]){
                swap(pos, tempParent);
                pos = tempParent;
                if(pos > 0)
                    fixHeapUp(pos);
            }
        }
        CircularDynamicArray <keytype> keys;
        CircularDynamicArray <valuetype> values;
        keytype KEY_ERROR_CODE;
        valuetype VALUE_ERROR_CODE;
        int currHeapSize;
    public:
        //Default constructor for the Heap class. Sets currHeapSize to 0.
        Heap<keytype, valuetype>(){
            currHeapSize = 0;
        }
        //Overloaded constructor for the Heap class that uses bottom up heap building to achieve
        //O(n) time
        Heap<keytype, valuetype>(keytype k[], valuetype v[], int s){
            for(int i = 0; i < s; i++){
                keys.addEnd(k[i]);
                values.addEnd(v[i]);
            }
            currHeapSize = s;
            for(int i = s-1; i >= 0; i--)
                fixHeapDown(i);
        }
        ~Heap<keytype, valuetype>(){
        }
        //sets the current heap to be equal to b2
        Heap(const Heap &b2){
            keys = b2.keys;
            values = b2.values;
            currHeapSize = b2.currHeapSize;
        }
        //destroys the current keys and values lists and sets the current heap to be equal to b2
        Heap& operator = (const Heap &b2){
            keys = b2.keys;
            values = b2.values;
            currHeapSize = b2.currHeapSize;
            return *this;
        }
        //returns the smallest key in the heap
        keytype peakKey(){
            if(currHeapSize > 0)
                return keys[0];
            return KEY_ERROR_CODE;
        }
        //returns the value associated with the smallest key in the heap
        valuetype peakValue(){
            if(currHeapSize > 0)
                return values[0];
            return VALUE_ERROR_CODE;
        }
        //inserts a new node in the next available spot, then fixes up to maintain heap order
        void insert(keytype k, valuetype v){
            keys.addEnd(k);
            values.addEnd(v);
            if(currHeapSize > 0)
                fixHeapUp(currHeapSize);
            currHeapSize++;
        }
        //replaces the min node with the last node in heap and then fixes down to preserve heap order
        //returns the minimum key in the heap.
        keytype extractMin(){
            if(currHeapSize > 0){
                keytype min = keys[0];
                keys.delFront();
                values.delFront();
                currHeapSize --;
                fixHeapDown(0);
                return min;
            }
            return KEY_ERROR_CODE;
        }
        //prints the contents of the CDA containing the heap
        void printKey(){
            for(int i = 0; i < currHeapSize; i++)
                cout << keys[i] << " ";
            cout << endl;
        }
};