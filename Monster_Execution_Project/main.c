/* COP 3502C Assignment 3
This program is written by: Rylan Simpson*/

/*Phase 1:
 * For every group g_i (g<=10)
 *         Until the number of remaining nodes reaches threshold th_i
 *                 Delete every k element
 *         Return list of remaining nodes
 *Phase 2: Until one node remains
 *              Look at every group's leading node
 *                  If the given node is the highest that node will be removed
 *                      If a tie occurs remove the lower group's node
 *          Return the remaining node
 */

//Use enqueue, dequeue, peek, isEmpty function
//Use monster* createMonster(int sequence)
        //It takes an integer, dynamically allocate a monster structure and returns a monster node
//Use void createReverseCircle(queue *q)
        /*It takes the reference of a queue, and creates a
        circular singly linked list for that queue where the nodes contain sequence numbers in
        reverse order . For example, if n=5 it should produce a circular singly linked list starting
        from 5 and ending at 1 as sequence number. During this process, use the createMonster
        function as well as enqueue() function to add the monster to the queue.*/
//Use void rearrangeCircle(queue* q)
        /*This function takes the reference of a queue and
        reverse the given circular singly linked list where the first node of the linked list is
        pointed by the front of the queue*/

//void display(queue q)
        //This function displays a given queue

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "leak_detector_c.h"

FILE* fileIn;
FILE* fileOut;

typedef struct Monster {
    int sequenceNumber;
    struct Monster *next;
}Monster;

typedef struct Queue {
    struct Monster *front;
    struct Monster *back;
    int groundNumber;
    int nodeCount;
    int k;
    int threshold;
}Queue;

//Initialize queues
Queue* initialize(Queue *qPtr) {
    qPtr->front = NULL;
    qPtr->back = NULL;
    qPtr->groundNumber = 0;
    qPtr->k = 0;
    qPtr->nodeCount = 0;
    qPtr->threshold = 0;
    return qPtr;
}

//Reverse the order of the queue
Queue* rearrangeCircle(Queue* qPtr) {
    Monster *root = qPtr->front;
    if(root == NULL || root->next == NULL) {
        return qPtr;
    }
    //Setup tracking node pointers
    Monster *start = qPtr->front;
    Monster *main = root->next;
    Monster *reversed = root;

    //printf("Test: %d\n", start->sequenceNumber);
    //printf("Test: %d\n", main->sequenceNumber);

    reversed->next = NULL;
    //Reverse nodes
    while (main!=NULL){
        Monster *temp = main;
        main = main->next;
        temp -> next = reversed;
        reversed = temp;
    }
    root = reversed;
    qPtr->front = root;

    //Largest number is not swapped, swap here
    qPtr->back = start;
    qPtr->front = qPtr->front->next;
    qPtr->back->next = qPtr->front;

    return qPtr;
}

//Verify if the node is circular
    //Boolean can be changed back to int for returning 1 or 0, is boolean to ensure funcitonality
bool isCircular(Queue* qPtr) {
    //If empty return true
    if (qPtr->front == NULL)
        return true;

    Monster* temp = qPtr->front;

    //Traverse to end of linked list, end if not circular or if returned to beginning
    for(int i = 0; i<qPtr->nodeCount; i++){
        temp = temp->next;
    }

    //Return if ended node is the beginning
    return (temp == qPtr->front);
}

void display(Queue *qPtr){
    //Set temp pointer for ease of use
    Monster *temp = qPtr->front;
    printf("%d ", qPtr->groundNumber);
    fprintf(fileOut, "%d ", qPtr->groundNumber);
    //Print items while traversing
    while (temp->next != qPtr->front){
        printf("%d ", temp->sequenceNumber);
        fprintf(fileOut, "%d ", temp->sequenceNumber);
        temp = temp->next;
    }
    //Print final item
    printf("%d\n", temp->sequenceNumber);
    fprintf(fileOut, "%d\n", temp->sequenceNumber);
}

int isEmpty(Queue *qPtr){
    //Return boolean comparing if the front queue pointer is null
    return qPtr->front == NULL;
}

int peek(Queue *qPtr){
    //If the queue is not empty return the sequence number
    if(!isEmpty(qPtr)){
        return qPtr->front->sequenceNumber;
    }
    else {
        return -1;
    }
}

//Create a monster node with a malloc'd struct, return the pointer to said malloc
Monster* createMonster(int sequence){
    //Allocate memory
    Monster* temp = malloc(sizeof(Monster));
    //Assign the sequence number to the monster node
    temp -> sequenceNumber = sequence;
    //Assign null pointer to next list pointer, to be reassigned later
    temp -> next = NULL;
    //Return the pointer to the node
    return temp;
}

void dequeue(Queue* qPtr){

    //Return null if queue is somehow empty
    if (qPtr->front == NULL)
        return;
    // Tracked node to remove and move front to next nodes
    Monster* temp = qPtr->front;
    Monster* nextTemp = qPtr->front->next;

    qPtr->front = qPtr->front->next;

    //If list had a single item, make queue null and return
    if (qPtr->nodeCount == 1) {
        free(temp);
        qPtr->front = NULL;
        qPtr->back = NULL;
        return;
    }

    //If queue has two items
    if (qPtr->nodeCount == 2){
        nextTemp->next = qPtr->back;
    }

    //Prevent re-circulation
    temp->next = NULL;

    //Ensure queue is circular
    if (qPtr->front->next == NULL){
        qPtr->front->next = qPtr->front;
    }

    //Free the node item
    free(temp);

}

/*This function should create a monster node and */
void enqueue(Queue *qPtr, int sequence){
    //Create a monster node
    Monster *temp = createMonster(sequence);
    //If the queue is empty
    if (qPtr->front == NULL){
        //Assign the node to the front
        qPtr->front = temp;
    }
    else {
        //Else place node at the back of the queue
        qPtr->back->next = temp;
    }
    //Setup back link within the queue
    qPtr->back = temp;
    qPtr->back->next = qPtr->front;
}

//Create the initial queue
Queue* createReverseCircle(Queue *qPtr){
    int i;
    //Get the ground number
    fscanf(fileIn, "%d", &qPtr->groundNumber);
    //Get the number of monster nodes
    fscanf(fileIn, "%d", &qPtr->nodeCount);
    //Get the k value for the queue
    fscanf(fileIn, "%d", &qPtr->k);
    //Get the threshold number
    fscanf(fileIn, "%d", &qPtr->threshold);
    //For the number of nodes
    for (i = qPtr->nodeCount; i>0; i--){
        enqueue(qPtr, i);
    }
    return qPtr;
}

Monster* removeMonster(Monster* position, int k) {
    //printf("k test: %d\n", k);
    //Traverse the queue k times
    for(int i = 1; i < k-1; i++) {
        position = position->next;
    }
    //create a temporary node to store the node to be freed
    Monster* temp = position->next;
    //Connect the two nodes on either side of the removed node
    position->next = temp->next;
    //Print and remove the node
    printf("Monster# %d executed\n",temp->sequenceNumber);
    free(temp);
    //Return the currently traced node
    return position->next;
}


int main(void){
    atexit(report_mem_leak);

    fileIn = fopen("in.txt", "r");
    fileOut = fopen("out.txt", "w");

    //Create and scan the number of groups to run
    int numGroups;
    fscanf(fileIn, "%d", &numGroups);
    //printf("%d\n", numGroups);

    //Create an array for the number of queues/groups
    Queue* groups[numGroups];

    //Create an array to check if the queue array is empty or not
    int groupIsEmpty[numGroups];
    for(int g = 0; g<numGroups; g++){
        groupIsEmpty[g] = -1;
    }

    //Create the queue groups with their respective data
    for (int i = 0; i<numGroups; i++){
        groups[i] = malloc(sizeof(Queue));
        groups[i] = initialize(groups[i]);
        createReverseCircle(groups[i]);
    }

    //Sink sort groups based on grounds number
            //Use braces to make the created node pointer local
    {
        Queue *sortTemp;
        for (int a = 0; a < numGroups; a++) {
            for (int b = 0; b < (numGroups - 1 - a); b++) {
                if (groups[b]->groundNumber > groups[b + 1]->groundNumber) {
                    sortTemp = groups[b];
                    //printf("%d\n", sortTemp->groundNumber);
                    groups[b] = groups[b + 1];
                    //printf("%d\n", groups[x]->groundNumber);
                    groups[b + 1] = sortTemp;
                    //printf("%d\n", groups[x+1]->groundNumber);
                }
            }
        }
    }

    //Print initial queues
    printf("Initial nonempty lists status\n");
    fprintf(fileOut,"Initial nonempty lists status\n");
    for (int j = 0; j<numGroups; j++){
        display(groups[j]);
    }

    printf("\n");
    fprintf(fileOut, "\n");

    //Rearrange queues
    for (int k = 0; k<numGroups; k++){
        groups[k] = rearrangeCircle(groups[k]);
    }

    //Print ordered queues
    printf("After ordering nonempty lists status\n");
    fprintf(fileOut ,"After ordering nonempty lists status\n");
    for (int w = 0; w<numGroups; w++){
        display(groups[w]);
    }

    printf("\n");
    fprintf(fileOut, "\n");

    printf("Phase1 execution\n");
    printf("\n");
    fprintf(fileOut, "Phase1 execution");
    fprintf(fileOut, "\n");

    //Remove every kth monster from the queue
    for (int l = 0; l<numGroups; l++) {
        printf("Line# %d\n", groups[l]->groundNumber);
        fprintf(fileOut, "Line# %d\n", groups[l]->groundNumber);
        //int nodeTrace = groups[l]->nodeCount;
        while(groups[l]->nodeCount > groups[l]->threshold) {
            groups[l]->front = removeMonster(groups[l]->front, groups[l]->k);
            groups[l]->nodeCount = groups[l]->nodeCount - 1;
        }
        printf("\n");
        fprintf(fileOut, "\n");
    }

    //Get the number of nodes left after phase 1
    int currNodes = 0;
    for (int n = 0; n<numGroups; n++) {
        currNodes = currNodes + groups[n]->nodeCount;
    }

    //Re-sort each queue from low to high
        //Use braces to make the created node pointer local
    {
        Monster *sortCurrTemp;
        for (int x = 0; x < numGroups; x++) {
            //For the number of nodes in the queue
            for (int y = 0; y < groups[x]->nodeCount; y++) {
                //For the remaining number of nodes
                for (int z = 0; z < (groups[x]->nodeCount - 1); z++) {
                    //Bubble sort sequence num values
                    if (groups[z]->front->sequenceNumber > groups[z + 1]->front->next->sequenceNumber) {
                        sortCurrTemp = groups[z]->front;
                        groups[z]->front = groups[z]->front->next;
                        groups[z]->front->next = sortCurrTemp;
                    }
                }
            }
        }
    }

    //Reconnect queue if found to be not circular
    for(int v = 0; v<numGroups; v++){
        if (!isCircular(groups[v])){
            Monster* temp = groups[v]->front;
            //Traverse to end of linked list, end if not circular or if returned to beginning
            for(int i = 0; i<groups[v]->nodeCount; i++){
                temp = temp->next;
            }
            temp->next = groups[v]->front;
        }
    }

    //Print queues for test if they maintain their status
    /*printf("Remaining number of monsters: %d\n", currNodes);
    printf("After k-removal lists status\n");
    for (int w = 0; w<numGroups; w++){
        display(groups[w]);
    }
    printf("\n");*/

    printf("Phase2 execution\n");
    fprintf(fileOut, "Phase2 execution\n");
    //While there is more than one queue
    for (int p = 0; p<currNodes-1; p++){

        //Get the fist set of non-empty information
        int nextNonemptyIndex = 0;
        while (1){
            if(groupIsEmpty[nextNonemptyIndex] == -1){
                break;
            }
            nextNonemptyIndex++;
        }
        //Set initial comparator index to first queue

        int highest = peek(groups[nextNonemptyIndex]);
        int highestIndex = nextNonemptyIndex;

        //For the remaining number of present queues
        for (int t = 1; t<numGroups; t++) {

            //If the queue is empty skip its comparison
            if(groupIsEmpty[t] == 1){
                continue;
            }

            //Create integer for comparing
            int groupSeqMatcher = peek(groups[t]);

            //If the sequence numbers are equal
            if (highest == groupSeqMatcher){
                //Return the index/value of the lower queue number
                if (groups[highestIndex]->groundNumber > groups[t]->groundNumber){
                    highest = peek(groups[t]);
                    highestIndex = t;
                }
                //Return same
                    //Maintain redundancy for insurance and debugging
                else {
                    highest = groups[highestIndex]->front->sequenceNumber;
                    highestIndex = highestIndex;
                }
            }
            //If the highest marked group is lower than the currently inspected group
            else if (highest < groupSeqMatcher){
               highest = peek(groups[t]);
               highestIndex = t;
            }
            else if (highest > groupSeqMatcher){
                //Proceed, current index is the lowest

            }
        }
        printf("Executed Monster %d from line %d\n", groups[highestIndex]->front->sequenceNumber, groups[highestIndex]->groundNumber);
        fprintf(fileOut, "Executed Monster %d from line %d\n", groups[highestIndex]->front->sequenceNumber, groups[highestIndex]->groundNumber);
        //Remove the selected monster from the front of their queue
        dequeue(groups[highestIndex]);
        //Trace the changes in node count
        groups[highestIndex]->nodeCount = groups[highestIndex]->nodeCount - 1;

        //If the queue is empty set it up for skipping
        if(isEmpty(groups[highestIndex])) {
            free(groups[highestIndex]);
            groupIsEmpty[highestIndex] = 1;
        }

    }

    //Trace which monster survived
    int FinalNonemptyIndex = 0;
    while (1){
        if(groupIsEmpty[FinalNonemptyIndex] == -1){
            break;
        }
        FinalNonemptyIndex++;
    }
    //Print result
    printf("\nMonster %d from line %d will survive", groups[FinalNonemptyIndex]->front->sequenceNumber, groups[FinalNonemptyIndex]->groundNumber);
    fprintf(fileOut, "\nMonster %d from line %d will survive", groups[FinalNonemptyIndex]->front->sequenceNumber, groups[FinalNonemptyIndex]->groundNumber);

    //free final monster node and queue
    free(groups[FinalNonemptyIndex]->front);
    free(groups[FinalNonemptyIndex]);

    //Close files
    fclose(fileIn);
    fclose(fileOut);
}