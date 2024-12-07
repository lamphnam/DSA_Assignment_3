/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   DLinkedListSE.h
 * Author: LTSACH
 *
 * Created on 31 August 2020, 14:13
 */

#ifndef DLINKEDLISTSE_H
#define DLINKEDLISTSE_H
#include "list/DLinkedList.h"
#include "sorting/ISort.h"

template <class T>
class DLinkedListSE : public DLinkedList<T> {
public:
    DLinkedListSE(void (*removeData)(DLinkedList<T> *) = 0, bool (*itemEQ)(T &, T &) = 0)
        : DLinkedList<T>(removeData, itemEQ) {

          };

    DLinkedListSE(const DLinkedList<T> &list) {
        this->copyFrom(list);
    }

    void sort(int (*comparator)(T &, T &) = 0) {
        // If list is empty or has only one element, no need to sort
        if(this->size() <= 1) return;

        // Convert the list to an array for easier sorting
        int listSize = this->size(); // Store size before clearing
        T *arr = new T[listSize];
        for(int i = 0; i < listSize; i++) {
            arr[i] = this->get(i);
        }

        // Perform merge sort on the array
        mergeSort(arr, 0, listSize - 1, comparator);

        // Clear the original list and refill with sorted elements
        this->clear();
        for(int i = 0; i < listSize; i++) {
            this->add(arr[i]);
        }

        // Free the temporary array
        delete[] arr;
    }

    // Recursive merge sort method
    void mergeSort(T *arr, int left, int right, int (*comparator)(T &, T &)) {
        if(left >= right) return;

        int mid = left + (right - left) / 2;

        // Recursively sort both halves
        mergeSort(arr, left, mid, comparator);
        mergeSort(arr, mid + 1, right, comparator);

        // Merge the sorted halves
        merge(arr, left, mid, right, comparator);
    }

    // Merge two sorted subarrays
    void merge(T *arr, int left, int mid, int right, int (*comparator)(T &, T &)) {
        int leftSize = mid - left + 1;
        int rightSize = right - mid;

        // Create temporary arrays
        T *leftArr = new T[leftSize];
        T *rightArr = new T[rightSize];

        // Copy data to temporary arrays
        for(int i = 0; i < leftSize; i++) {
            leftArr[i] = arr[left + i];
        }
        for(int j = 0; j < rightSize; j++) {
            rightArr[j] = arr[mid + 1 + j];
        }

        // Merge the temporary arrays
        int i = 0, j = 0, k = left;
        while(i < leftSize && j < rightSize) {
            // Use the compare method to determine sorting order
            if(compare(leftArr[i], rightArr[j], comparator) <= 0) {
                arr[k] = leftArr[i];
                i++;
            } else {
                arr[k] = rightArr[j];
                j++;
            }
            k++;
        }

        // Copy remaining elements of leftArr if any
        while(i < leftSize) {
            arr[k] = leftArr[i];
            i++;
            k++;
        }

        // Copy remaining elements of rightArr if any
        while(j < rightSize) {
            arr[k] = rightArr[j];
            j++;
            k++;
        }

        // Free temporary arrays
        delete[] leftArr;
        delete[] rightArr;
    }

protected:
    static int compare(T &lhs, T &rhs, int (*comparator)(T &, T &) = 0) {
        if(comparator != 0) return comparator(lhs, rhs);
        else {
            if(lhs < rhs) return -1;
            else if(lhs > rhs) return +1;
            else return 0;
        }
    }
};

#endif /* DLINKEDLISTSE_H */
