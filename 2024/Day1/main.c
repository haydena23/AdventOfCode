#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int comp(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int findLeftmost(int arr[], int size, int target) {
    int left = 0;
    int right = size - 1;
    int leftmostPos = -1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (arr[mid] == target) {
            leftmostPos = mid; 
            right = mid - 1;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return leftmostPos;
}

int findRightmost(int arr[], int size, int target) {
    int left = 0;
    int right = size - 1;
    int rightmostPos = -1;
    while (left <= right) {
        int mid = (left + right) / 2;

        if (arr[mid] == target) {
            rightmostPos = mid;
            left = mid + 1;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return rightmostPos;
}

int findOccurrences(int arr[], int size, int target) {
    int leftBound = findLeftmost(arr, size, target);
    if (leftBound == -1) {
        return 0;
    }
    int rightBound = findRightmost(arr, size, target);
    return (rightBound - leftBound + 1);
}

int main(void) {
    FILE *file = fopen("input.txt","r");
    if(!file) {
        perror("Failed to open input file.");
        exit(EXIT_FAILURE);
    }

    int left[1000];
    int right[1000];
    int counter = 0;

    while(counter < 1000 && fscanf(file, "%d %d", &left[counter], &right[counter]) == 2) {
        counter++;
    }
    fclose(file);

    qsort(left,1000,sizeof(int),comp);
    qsort(right,1000,sizeof(int),comp);

    // Part 1
    int totalDistance = 0;
    for(int i = 0; i < 1000; i++) {
        totalDistance += abs((left[i]-right[i]));
    }
    printf("Total Distance: %d\n",totalDistance);

    // Part 2
    int similarity = 0;
    for(int i = 0; i < 1000; i++) {
        similarity += (left[i] * findOccurrences(right,1000,left[i]));
    }
    printf("Similarity Score: %d\n",similarity);
    exit(EXIT_SUCCESS);
}