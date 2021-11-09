/* COP 3502C Assignment 4
This program is written by: Rylan Simpson */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include "leak_detector_c.h"

FILE *in;
FILE *out;

void insertionSort(int arr[], int n);
void merge(int arr[], int left, int mid, int right);
void mergeSort(int arr[], int left, int right);

int main() {
    //atexit(report_mem_leak);

    //To calculate actual runtime
    clock_t begin = clock();

    in = fopen("message.txt", "r");
    out = fopen("out.txt", "w");

    int numCases;
    fscanf(in, "%d", &numCases);

    for (int i = 0; i<numCases; i++){
        int arrLen;
        long long numPages;

        fscanf(in, "%d ", &arrLen);
        fscanf(in, "%lld", &numPages);

        //Allocate space for book array
        int *bookArr = (int*)malloc(arrLen*sizeof(int));

        //Scan in array of books, unsorted
        for (int j = 0; j<arrLen; j++){
            fscanf(in, "%d", &bookArr[j]);
        }

        mergeSort(bookArr, 0, arrLen-1);

        //For debugging use
//        for (int j = 0; j<arrLen; j++){
//            printf("%d ", bookArr[j]);
//        }

        //Count how many books can be read
        long long pageCount = 0;
        int a = 0;
        //While the number of read pages is lower than limit
        while (pageCount <= numPages && a < arrLen) {
            //Catch if current book breaches limit
            if (bookArr[a] > numPages) {
                break;
            }
            //Count book
            pageCount = pageCount + bookArr[a];
            a++;
            //Check if next book will breach limit
            if (pageCount + bookArr[a] >= numPages + 1){
                break;
            }
        }
        printf("%d\n", a);
        fprintf(out, "%d\n", a);
        free(bookArr);
    }

    fclose(in);
    fclose(out);

    //Calculate how long the total execution time took
    clock_t end = clock();
    double runtime = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Actual runtime: %f", runtime);
}

void merge(int arr[], int left, int mid, int right) {
    int i, j, k;
    int lSize = mid - left + 1;
    int rSize = right - mid;

    //Allocate space for smaller arrays
    int *L = (int*)malloc(lSize*sizeof(int));
    int *R = (int*)malloc(rSize*sizeof(int));

    //Copy data
    for (i = 0; i < lSize; i++) {
        L[i] = arr[left + i];
    }
    for (j = 0; j < rSize; j++) {
        R[j] = arr[mid + 1 + j];
    }

    i = 0;
    j = 0;
    k = left;
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

//insertion sort for optimization
void insertionSort(int arr[], int n){
    int i, k, j;
    for (i = 1; i < n; i++){
        k = arr[i];
        for(j=i-1; j>=0; j--) {
            if (arr[j] > k)
                arr[j + 1] = arr[j];
            else {
            break;
            }
        }
        arr[j+1] = k;
    }
}

//Recursive mergeSort call
void mergeSort(int arr[], int left, int right){
    int n = right-left;
//    if(n <= 30){
//        insertionSort(arr, n);
//            return;
//    }
    //else {
        if (left < right) {
            int mid = (right + left) / 2;

            //Recursive calls to split down the array, sorting occurs on merge
            mergeSort(arr, left, mid);
            mergeSort(arr, mid + 1, right);

            //Merge arrays
            merge(arr, left, mid, right);
        }
    //}
}