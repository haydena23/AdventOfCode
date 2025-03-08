#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 50
#define DELIMETER " "

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* createNode(int data) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    if(newNode == NULL) {
        perror("Error: Memory alocation for new node failed.");
        return NULL;
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

Node* insertAtEnd(Node* head, int data) {
    Node* newNode = createNode(data);
    if(newNode == NULL) {
        perror("Failed to create new node, returning head.");
        return head;
    }
    if(head == NULL) {
        perror("Head node is null, returning new node.");
        return newNode;
    }
    Node* current = head;
    while(current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
    return head;
}

bool isSortedAscending(Node* head) {
    Node* current = head;
    while(current != NULL && current->next != NULL) {
        if(current->data > current->next->data) {
            return false;
        }
        current = current->next;
    }
    return true;
}

bool isSortedDescending(Node* head) {
    Node* current = head;
    while(current != NULL && current->next != NULL) {
        if(current->data < current->next->data) {
            return false;
        }
        current = current->next;
    }
    return true;
}

bool difference(Node* head) {
    Node* current = head;
    while(current != NULL && current->next != NULL) {
        int diff = abs(current->data - current->next->data);
        if(diff < 1 || diff > 3) {
            return false;
        }
        current = current->next;
    }
    return true;
}

void printList(Node* head) {
    Node* current = head;
    while(current != NULL) {
        Node* next = current->next;
        printf("%d ",current->data);
        current = next;
    }
}

void freeList(Node* head) {
    Node* current = head;
    while(current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }
}

int main(void) {
    FILE *file = fopen("input.txt","r");
    if(!file) {
        perror("Failed to open input file.");
        exit(EXIT_FAILURE);
    }

    Node* lines[1000];
    int lineCount = 0;
    char buffer[MAX_LINE_LENGTH];

    while(fgets(buffer,MAX_LINE_LENGTH,file) != NULL && lineCount < 1000) {
        char* token = strtok(buffer,DELIMETER);
        if(token == NULL) {
            continue;
        }
        lines[lineCount] = createNode(atoi(token));
        while ((token = strtok(NULL, DELIMETER)) != NULL) {
            insertAtEnd(lines[lineCount],atoi(token));
        }
        lineCount++;
    }
    fclose(file);
    
    int safe = 0;

    int num_theads = 8;
    omp_set_num_threads(num_theads);

    #pragma omp parallel for schedule(dynamic) reduction(+:safe)
    for(int i = 0; i < 1000; i++)  {
        if((isSortedAscending(lines[i]) || isSortedDescending(lines[i])) && difference(lines[i])) {
            safe++;
        }
    }

    printf("Counted: %d\n",safe);

    for(int i = 0; i < 1000; i++) {
        freeList(lines[i]);
    }

    return EXIT_SUCCESS;
}