#include <stdio.h>
#include<stdlib.h>
#include<time.h>

#define MAXVAL 1000000

void randArray(int A[], int size, int maxval);
void bubbleSort(int A[], int n);
void insertionSort(int arr[], int n);
void swap(int *a, int *b);
void arrayCopy(int from[], int to[], int size);
long timediff(clock_t t1, clock_t t2);
void merge(int arr[], int l, int m, int r);
void mergeSort(int arr[], int l, int r);
void selectionSort(int arr[], int n);
int partition(int *vals, int low, int high);
void quickSort(int* numbers, int low, int high);

int main() {

    // Different sizes to test the sort.
    int sizes[] = {10000, 20000, 30000, 40000, 50000, 60000};

    int* originalArray; //to keep the original array unchanged so that we can use it for various algorithms.
    int* sortedArray; //We will mainly pass this one to all the function.
    int i, j;
    clock_t start, end;

    // Loop through trying each size.
    for (i=0; i<6; i++) {

        // Allocate the space for the array and fill it.
        originalArray = (int*)malloc(sizeof(int)*sizes[i]);
        sortedArray = (int*)malloc(sizeof(int)*sizes[i]);

        randArray(originalArray, sizes[i], MAXVAL);

        arrayCopy(originalArray, sortedArray, sizes[i]);

        start = clock();
        bubbleSort(sortedArray, sizes[i]);
        end = clock();
        printf("Sorting %d values took %ld ms for bubble sort.\n", sizes[i], timediff(start, end));

        arrayCopy(originalArray, sortedArray, sizes[i]);

        start = clock();
        selectionSort(sortedArray, sizes[i]);
        end = clock();
        printf("Sorting %d values took %ld ms for selection sort sort.\n", sizes[i], timediff(start, end));

        arrayCopy(originalArray, sortedArray, sizes[i]);

        start = clock();
        insertionSort(sortedArray, sizes[i]);
        end = clock();
        printf("Sorting %d values took %ld ms for insertion sort.\n", sizes[i], timediff(start, end));

        arrayCopy(originalArray, sortedArray, sizes[i]);


        start = clock();
        mergeSort(sortedArray, 0, sizes[i]-1);
        end = clock();
        printf("Sorting %d values took %ld ms for merge sort.\n", sizes[i], timediff(start, end));

        arrayCopy(originalArray, sortedArray, sizes[i]);

        start = clock();
        quickSort(sortedArray, 0, sizes[i]-1);
        end = clock();
        printf("Sorting %d values took %ld ms for quick sort.\n", sizes[i], timediff(start, end));


        printf("\n");

        free(sortedArray);
        free(originalArray);
    }

    //system("PAUSE");
    return 0;
}

void arrayCopy(int from[], int to[], int size)
{
    int j;
    for(j=0; j<size; j++)
        to[j] = from[j];

}

// Pre-condition: A is an array of size greater than or equal to size and
//                maxval is positive.
// Post-condition: A is filled with random integers in the range [1,maxval]
//                 from index 0 to index size-1.
void randArray(int A[], int size, int maxval) {

    int i;
    // Fill each array slot in question with a random value in the designated range.
    for (i=0; i<size; i++)
        A[i] = rand()%maxval + 1;
}

// Sorts the first length elements of vals in ascending order.
void bubbleSort(int A[], int n) {

    int i,j;

    // Loop through each element to insert.
    for (i=n-2; i>=0; i--) {

        for (j=0; j<=i; j++)
            if (A[j] > A[j+1])
                swap(&A[j], &A[j+1]);
    }
}

// Swaps the integers pointed to by a and b.
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void insertionSort(int arr[], int n)
{
    int i, item, j;
    for (i = 1; i < n; i++)
    {
        item = arr[i];

        /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
        for(j=i-1; j>=0; j--)
        {
            if(arr[j]>item)
                arr[j+1] = arr[j];
            else
                break;

        }
        arr[j+1] = item;
    }
}

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int lSize = m - l + 1;
    int rSize = r - m;

    //Allocate space for smaller arrays
    int *L = (int*)malloc(lSize*sizeof(int));
    int *R = (int*)malloc(rSize*sizeof(int));

    //Copy data
    for (i = 0; i < lSize; i++) {
        L[i] = arr[l + i];
    }
    for (j = 0; j < rSize; j++) {
        R[j] = arr[m + 1 + j];
    }

    i = 0;
    j = 0;
    k = l;
    //Refill main array, sorted
    while (i < lSize && j < rSize) {
        if (L[i] < R[j]) {
            arr[k] = L[i];
            i++;
            k++;
        }
        else {
            arr[k] = R[j];
            j++;
            k++;
        }
    }
    //Empty remaining array space
    while (i < lSize) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < rSize) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(R);
    free(L);
}

/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void mergeSort(int arr[], int l, int r)
{
    if (l < r) {
        int m = (r + l) / 2;

        //Recursive calls to split down the array, sorting occurs on merge
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        //Merge arrays
        merge(arr, l, m, r);
    }
}


void selectionSort(int arr[], int n)
{
    int i, j, min_idx, temp;
    // One by one move boundary of unsorted subarray
    for (i = 0; i < n-1; i++)
    {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i+1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;
        // Swap the found minimum element with the first element
        temp = arr[i];
        arr[i] = arr[min_idx];
        arr[min_idx] = temp;
    }
}

// Pre-condition: low and high are valid indexes into values
// Post-condition: Returns the partition index such that all the values
//                 stored in vals from index low to until that index are
//                 less or equal to the value stored there and all the values
//                 after that index until index high are greater than that
//
int partition(int *vals, int low, int high)
{
    int temp, i, p;
    i = low + rand()%(high-low+1);
    swap(&vals[low], &vals[i]);
    p = low;
    low++;
    while(low <= high){
        while(low <= high && vals[low] <= vals[p]){
            low++;
        }
        while(high >= low && vals[high] > vals[p]){
            high--;
        }
        if(low<high){
            swap(&vals[low], &vals[high]);
        }
        swap(&vals[p], &vals[high]);
        return high;
    }
}

// Pre-condition: s and f are value indexes into numbers.
// Post-condition: The values in numbers will be sorted in between indexes s
//                 and f.
void quickSort(int* numbers, int low, int high) {
    if(low<high){
        int k = partition(numbers, low, high);
        quickSort(numbers, low, k-1);
        quickSort(numbers, k+1, high);
    }
}

long timediff(clock_t t1, clock_t t2)
{
    long elapsed;
    elapsed = ((double)t2 - t1) / CLOCKS_PER_SEC * 1000;
    return elapsed;
}