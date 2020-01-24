#include <iostream>
#include <string>
#include <cmath>
using namespace std;

template <typename T>
class CircularDynamicArray{
    private:
    
    //works the same as the [] overload but in this class
    int index(int i){
        return (head + i) % maxSize;
    }
    //copies the current CDA over to one of 2 times the size
    void grow(bool flag){
        maxSize *= 2; 
        T *temp = new T[maxSize];
        if(flag == false){ //for add end
            for(int i = 0; i < maxSize / 2; i++){
                if(i == head)
                    temp[0] = arr[i];
                else if(i < head)
                    temp[(maxSize / 2) - head + i] = arr[i];
                else if(i > head)
                    temp[i - head] = arr[i];
            }
        }
        else{ //for add front
            for(int i = 0; i < maxSize / 2; i++){
                if(i == head)
                    temp[1] = arr[i];
                if(i < head)
                    temp[(maxSize / 2) - head + i + 1] = arr[i];
                if(i > head)
                    temp[i - head + 1] = arr[i];
            }
        }
        delete [] arr;
        arr = NULL;
        arr = temp;
        currSize++;
        head = 0;
        tail = currSize - 1;
    }
    //copies the current CDA over to one of half the size
    void shrink(){
        maxSize /= 2;
        T *temp = new T[maxSize];
        if(head < tail)
            for(int i = head; i <= tail; i++)
                temp[i-head] = arr[i];
        else{
            int counter = 0;
            for(int i = head; i < maxSize * 2; i++)
                temp[i - head] = arr[i];
            for(int i = 0; i <= tail; i++)
                temp[(maxSize * 2) - head + i] = arr[i];
        }
        delete [] arr;
        arr = NULL;
        arr = temp;
        head = 0;
        tail = currSize - 1;
    }
    //merge function used for mergesort
    void merge(int low, int high, int mid) {
        int i, j, k;
        T temp[high-low+1];
        i = low;
        k = 0;
        j = mid + 1;
        while (i <= mid && j <= high) {
            if (arr[index(i)] < arr[index(j)]) {
                temp[k] = arr[index(i)];
                k++;
                i++;
            }
            else {
                temp[k] = arr[index(j)];
                k++;
                j++;
            }
        }
        while (i <= mid) {
            temp[k] = arr[index(i)];
            k++;
            i++;
        }
        while (j <= high) {
            temp[k] = arr[index(j)];
            k++;
            j++;
        }
        for (i = low; i <= high; i++){
            arr[index(i)] = temp[i-low];
        }
    }
    //mergesort function sorts CDA using divide and conquer strategy
    void mergeSort(int low, int high) {
        int mid = 0;
        if (low < high) {
            mid = (low+high)/2;
            mergeSort(low, mid);
            mergeSort(mid+1, high);
            merge(low, high, mid);
        }
    }
    void merge(int low, int high, int mid, T* input) { //merge for sorting arrays besides arr
        int i, j, k;
        T temp[high-low+1];
        i = low;
        k = 0;
        j = mid + 1;
        while (i <= mid && j <= high) {
            if (input[i] < input[j]) {
                temp[k] = input[i];
                k++;
                i++;
            }
            else {
                temp[k] = input[j];
                k++;
                j++;
            }
        }
        while (i <= mid) {
            temp[k] = input[i];
            k++;
            i++;
        }
        while (j <= high) {
            temp[k] = input[j];
            k++;
            j++;
        }
        for (i = low; i <= high; i++){
            input[i] = temp[i-low];
        }
    }
    void mergeSort(int low, int high, T* input) { //for sorting arrays other than arr
        int mid = 0;
        if (low < high) {
            mid = (low+high)/2;
            mergeSort(low, mid, input);
            mergeSort(mid+1, high, input);
            merge(low, high, mid, input);
        }
    }
    //bitNum is how many bits we are sorting by, shift represents the current byte
    void countingSort(int bitNum, int shift){ 
        int size = pow(2, bitNum);
        int count[size] = {0};
        int sum[size] = {0};
        int tempDigit[length()];
        T tempArr[length()];

        //makes count array
        for(int i = 0; i < length(); i++){ 
            int casted = (int)arr[index(i)];
            int bit = (casted >> (shift * 4)) & (size - 1);
            tempDigit[i] = bit;
            count[bit]++;
        }
        // makes sum array
        for(int i = 1; i < size; i++)
            sum[i] = sum[i - 1] + count [i - 1];
        //sorts array and puts it in temp
        for(int i = 0; i < length(); i++){
            tempArr[sum[tempDigit[i]]] = arr[index(i)];
            sum[tempDigit[i]]++;
        }
        //copies over temp to arr
        for(int i = 0; i < length(); i++)
            arr[index(i)] = tempArr[i];
    }
    int partition(int left, int right){ // for Quick Select
        //i is the farthest left value that is greater than x
        T x = arr[index(right)], temp;
        int i = left; 
        for (int j = left; j <= right - 1; j++) { 
            if (arr[index(j)] <= x) { 
                temp = arr[index(i)];
                arr[index(i)] = arr[index(j)];
                arr[index(j)] = temp;
                i++; 
            } 
        }
        temp = arr[index(i)];
        arr[index(i)] = arr[index(right)];
        arr[index(right)] = temp;
        return i;
    }
    int partition(int left, int right, T median){ //for WC Select
        T temp;
        for(int i = left; i <= right; i++){
            if(arr[index(i)] == median){ // moves the median to the right most element
                temp = arr[index(right)];
                arr[index(right)] = arr[index(i)];
                arr[index(i)] = temp;
                break;
            }
        }
        //i is equal to the farthest left value greater than x
        int i = left; 
        for (int j = left; j <= right - 1; j++) { 
            if (arr[index(j)] <= median) { 
                temp = arr[index(i)];
                arr[index(i)] = arr[index(j)];
                arr[index(j)] = temp;
                i++; 
            } 
        }
        temp = arr[index(i)];
        arr[index(i)] = arr[index(right)];
        arr[index(right)] = temp;
        return i;
    }
    //returns the median of the passed indexes
    T findMedian(int left, int right){
        if(left == right){
            return arr[index(left)];
        }
        mergeSort(left, right);
        return arr[index(left + ((right - left) / 2))];
    }
    T KthElement(int left, int right, int k){
        int size = right - left + 1;
        int medianSize = (size + 4) / 5; //number of groups of 5 or less
        T median [medianSize];
        //populates the median array
        for(int i = 0; i < size / 5; i++)
            median[i] = findMedian(left + (5 * i), left + (5 * i) + 4);
        if(size % 5 != 0){ // handles the smallest group
            int a = size / 5; // a is how many groups of 5 exist
            int leftIndex = left + (5 * a);
            median[a] = findMedian(leftIndex, leftIndex + (size % 5) - 1); //changed to a from a + 1
        }
        T medianOfMedians;
        //finds the median medians
        if(medianSize == 1)
            medianOfMedians = median[0];
        else{
            mergeSort(0, medianSize - 1, median); 
            medianOfMedians = median[(medianSize - 1) / 2];
        }
        //partitions around the median of medians
        int medianPos = partition(left, right, medianOfMedians);
        if(medianPos - left == k - 1)
            return arr[index(medianPos)];
        if(medianPos - left > k - 1)
            return KthElement(left, medianPos - 1, k);
        return KthElement(medianPos + 1, right, k - medianPos + left - 1);
    }
    T QuickSelect(int left, int right, int k){
        if (k > 0 && k <= right - left + 1) { 
            if(left == right)
                return arr[index(left)];
            //partitions around right
            int i = partition(left, right); 
            if (i - left == k - 1) 
                return arr[index(i)]; 
            if (i - left > k - 1)  
                return QuickSelect(left, i - 1, k); 
            return QuickSelect(i + 1, right,  k - i + left - 1); 
        } 
    }
    //binary search to find element e
    int binSearch(int left, int right, T e){
        if(right >= left){
            int mid = left + (right - left) / 2;
            if(e == arr[index(mid)])
                return mid;
            else if(e < arr[index(mid)])
                right = mid - 1;
            else 
                left = mid + 1;
            return binSearch(left, right, e);
        }
        return -1;
    }
    T *arr;
    T ERROR_CODE;
    int currSize;
    int maxSize;
    int head;
    int tail;

    public:

    //sets arr to be size 2
    CircularDynamicArray <T>(){
        arr = new T[2];
        maxSize = 2;
        currSize = 0;
        head = 0;
        tail = 0;
    }
    //creates CDA of size size
    CircularDynamicArray <T>(int size){
        if(size > 0){
            arr = new T[size];
            maxSize = size;
            currSize = size;
            head = 0;
            tail = size - 1;
        }
        else
            cout << "Error: Circular Dynamic Array must be of size greater than 0." << endl;
    }
    //sets current CDA to be equal to c2
    CircularDynamicArray(const CircularDynamicArray &c2){ 
        maxSize = c2.maxSize;
        currSize = c2.currSize;
        head = c2.head;
        tail = c2.tail;
        arr = new T[maxSize];
        for(int i = 0; i < maxSize; i++)
            arr[i] = c2.arr[i];
    }
    //destructor for CDA
    ~CircularDynamicArray <T>(){
        delete [] arr;
    }
    //destroys current CDA and sets it to be equal to c2
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
    }
    void addEnd(T input){
        //copys the array and doubles it
        if(currSize == maxSize){
            grow(false);
            arr[tail] = input;
        }
        else{
            if(currSize == 0)
                arr[tail] = input;
            else if (tail == maxSize - 1){
                tail = 0;
                arr[tail] = input;
            }
            else{
                tail++;
                arr[tail] = input;
            }
            currSize++;
        }
    }
    void addFront(T input){
        //case in which the array is full
        //flag is necessary because the grow function in the first if statement resets the head to zero
        //which would trigger the else if statement if there was no flag
        bool flag = false;
        if(currSize == maxSize){
            grow(true);
            arr[head] = input;
            flag = true;
        }
        //case in which head is at the start of the array and array is not full
        else if(head == 0 && flag == false){
            //puts it at the end of the array and makes it the new head pointer, make sure to check
            //for null other places
            if(currSize == 0)
                arr[head] = input;
            else{
                head = maxSize - 1;
                arr[head] = input;
            }
            currSize++;
        }
        //case in which head is not in spot 0 of the array
        else{
            //sets it as the spot before the head and then points the head to it
            head--;
            arr[head] = input;
            currSize++;
        }
    }
    void delEnd(){
        if(currSize > 0){
            currSize--;
            if(tail < head){
                if(tail == 0)
                    tail = maxSize - 1;
                else
                    tail--;
            }
            else if(tail > head)
                tail--;
            else if(tail == head){
                head = 0;
                tail = 0;
            }
            //shrinks the arr if it is only taking up 25% of the capacity
            if(currSize * 4 <= maxSize && currSize != 0)
                shrink();
        }
    }
    void delFront(){
        if(currSize > 0){
            currSize--;
            if(head < tail)
                head++;
            else if(head > tail){
                if(head == maxSize - 1)
                    head = 0;
                else
                    head++;
            }
            else if(head == tail){
                head = 0;
                tail = 0;
            }
            //shrinks the arr if it is only taking up 25% of the capacity
            if(currSize * 4 <= maxSize && currSize != 0)
                shrink();
        }
    }
    //returns the number of items in the CDA
    int length(){
        return currSize;
    }
    //returns the capacity of the CDA
    int capacity(){
        return maxSize;
    }
    //clears the CDA and sets it to a size of 2
    void clear(){
        delete [] arr;
        arr = NULL;
        arr = new T[2];
        maxSize = 2;
        currSize = 0;
        head = 0;
        tail = 0;
    }
    //bracket overloader for CDA
    T& operator[](int index){
        if (index >= currSize || index < 0) 
        { 
            cout << "Out of bounds reference: " << index << endl; 
            return ERROR_CODE;
        } 
        return arr[(head + index) % maxSize];
    }
    //mergesorts CDA
    void stableSort(){
        mergeSort(0, currSize - 1);
    }
    //uses a linear search to find the pos of element e
    int linearSearch(T e){ 
        for(int i = 0; i < length(); i++)
            if(arr[index(i)] == e)
                return i;
        return -1;
    }
    //uses a binary search to find the pos of element e
    int binSearch(T e){ 
        return binSearch(0, length() - 1, e);
    }
    //sorts CDA using radix sort
    void radixSort(int i){
        if(i > sizeof(int) * 8)
            i = sizeof(int) * 8;
        int numOfFourBits = i / 4; //number of times we will loop through using 4 bit counting sort
        for(int j = 0; j < numOfFourBits; j++)
            countingSort(4, j);
        if(i % 4 != 0){
            int remainderBits = i % 4; //deals with remainder bits if i is not a mult of 4
            countingSort(remainderBits, numOfFourBits);
        }
    }
    //finds the element of rank k in the CDA
    T QuickSelect(int k){
        return QuickSelect(0, currSize - 1, k);
    }
    //finds the element of rank k in the CDA
    T WCSelect(int k){
        if(k <= 0)
            return ERROR_CODE;
        return KthElement(0, length() - 1, k);
    }
};