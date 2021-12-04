/* COP 3502C Assignment 6
This program is written by: Rylan Simpson */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_SIZE 21

FILE *in;
FILE *out;

//Customer data structure
typedef struct customer {
    int timeArrival; // The time of arrival that the customer came in
    int loyaltyPoints; // Number of "loyalty points" the customer has
    int serviceTime; //Expected service time of the customer
    int leaveTime; //Time at which customer leaves
    int isFirst; //Marks if the customer is the first inserted into a given queue
    char *customerName; //Name of customer
    char *preferredStylist; //Name of preferred stylist
    char *actualStylist; //Name of serving stylist
} customer;

typedef struct stylistQueue { //Index in queue
    customer **queue;
    char *stylistName; // Name of stylist
    int numQueued; // Number of queued customers
    int totalTimeWorked; // The number of minutes the stylist has been working
} stylistQueue;

//Function prototypes
customer *createCustomerNode();
int getSeatOfStylist(char *stylistName, int numStylists, stylistQueue *seatArr);
int findIndexOfSmallestQueue(int numStylists, stylistQueue *seatArr);
int compareCustomers(customer *customer1, int indexOfCustomer1, customer *customer2, int indexOfCustomer2);
void percolateUp(stylistQueue *station, int index);
void percolateDown(stylistQueue *station, int index);
void insertCustomer(stylistQueue *station, customer *newCustomer);
void dequeueCustomer(stylistQueue *station);
void serveCustomers(customer **readyCustomers, int numReady);
void serveSingleCustomer(customer *readyCustomer);
void swap(stylistQueue *station, int indexA, int indexB);
void sortSwap(customer customerArr[], int index);
void sortCustomersByLeaveTime(int numReady, customer data[]);

int main() {

    /** BEGIN PROGRAM EXECUTION **/

    //Open files
    in = fopen("in.txt", "r");
    out = fopen("out.txt", "w");

    //Get number of stylists and customers
    int numCustomers;
    int numUnservedCustomers;
    int numStylists;
    fscanf(in, "%d ", &numCustomers);
    fscanf(in, "%d ", &numStylists);
    numUnservedCustomers = numCustomers;

    customer *readyCustomers[numStylists];
    stylistQueue *stylistPtrTracker[numStylists];
    stylistQueue stylistArray[numStylists];

    for (int i = 0; i < numStylists; i++) {
        //Make room for stylist heap struct
        stylistQueue *stylist = calloc(1, sizeof(stylistQueue));
        stylistPtrTracker[i] = stylist;
        //Make space for name
        char *stylistName = calloc(MAX_NAME_SIZE, sizeof(char));
        //Make space for actual heap
        customer **customerQueue = calloc((numCustomers) + 1, sizeof(customer *));
        //Mark all node indices as null for a starting queue
        for (int j = 0; j < numCustomers + 1; j++) {
            customerQueue[j] = NULL;
        }
        //Scan in stylist name
        fscanf(in, "%s", stylistName);
        //Initialize data
        stylist->stylistName = stylistName;
        stylist->numQueued = 0;
        stylist->totalTimeWorked = 0;
        stylist->queue = customerQueue;
        //Set stylist to array
        stylistArray[i] = *stylist;
    }

    /** STYLIST ARRAY INITIALIZATION COMPLETE **/

    //For each customer, create a node and attach the node to a given queue, given preferred stylist
    for (int i = 0; i < numCustomers; i++) {
        //Make customer node
        customer *customerNode = createCustomerNode();

        //Find index of stylist
        int indexOfStation = getSeatOfStylist(customerNode->preferredStylist, numStylists, stylistArray);

        if (indexOfStation == -1) {
            //Find index of station
            indexOfStation = findIndexOfSmallestQueue(numStylists, stylistArray);
            //Insert customer
            insertCustomer(&stylistArray[indexOfStation], customerNode);
            //Record stylist who took customer
            strcpy(stylistArray[indexOfStation].queue[0]->actualStylist, stylistArray[indexOfStation].stylistName); //Update name to serving stylist

            //Initialize number of completed customers
            int numReady = 0;

            //Check each stylist station
            for (int j = 0; j < numStylists; j++) {
                //If station's queue is not null and if the arrival time of inserted customer is greater than time after service of the active service
                if (stylistArray[j].queue[0] != NULL && (customerNode->timeArrival > (stylistArray[j].totalTimeWorked + stylistArray[j].queue[0]->serviceTime))) {

                    //If the inserted customer is the first served, offset time working by arrival time
                    if (stylistArray[j].totalTimeWorked == 0) {
                        stylistArray[j].totalTimeWorked = stylistArray[j].queue[0]->timeArrival;
                    }

                    //Calculate and save customer leave time
                    stylistArray[j].queue[0]->leaveTime =
                            stylistArray[j].totalTimeWorked + stylistArray[j].queue[0]->serviceTime;
                    //Update the time that the stylist worked
                    stylistArray[j].totalTimeWorked = stylistArray[j].queue[0]->leaveTime;

                    //Mark number of ready customers and unserved Customers
                    numReady++;
                    numUnservedCustomers--;

                    //Add customer to ready array
                    readyCustomers[j] = stylistArray[j].queue[0];
                    //Make active customer index null
                    stylistArray[j].queue[0] = NULL;

                    if (stylistArray[j].queue[1] != NULL) {
                        dequeueCustomer(&stylistArray[j]);
                    }
                }
            }
            //Sort the served customers by leaving time
            sortCustomersByLeaveTime(numReady, *readyCustomers);

            //Print and clean up the customers
            serveCustomers(readyCustomers, numReady);
            for (int j = 0; j < numReady; j++) {
                readyCustomers[j] = NULL;
            }
        }

            //Else if the preferred stylist is found
        else {
            //Insert customer into preferred stylist index
            insertCustomer(&stylistArray[indexOfStation], customerNode);
            //Update actual stylist name, should be same as preferred
            strcpy(stylistArray[indexOfStation].queue[0]->actualStylist, stylistArray[indexOfStation].stylistName); //Name will be updated even if preferred stylist exists


            //Initialize number of completed customers
            int numReady = 0;

            for (int j = 0; j < numStylists; j++) {
                //If station's queue is not null and if the arrival time of inserted customer is greater than time after service of the active service
                if (stylistArray[j].queue[0] != NULL && (customerNode->timeArrival > (stylistArray[j].totalTimeWorked + stylistArray[j].queue[0]->serviceTime))) {

                    //If the inserted customer is the first served, offset time working by arrival time
                    if (stylistArray[j].totalTimeWorked == 0) {
                        stylistArray[j].totalTimeWorked = stylistArray[j].queue[0]->timeArrival;
                    }

                    //Calculate and save customer leave time
                    stylistArray[j].queue[0]->leaveTime =
                            stylistArray[j].totalTimeWorked + stylistArray[j].queue[0]->serviceTime;
                    //Update the time that the stylist worked
                    stylistArray[j].totalTimeWorked = stylistArray[j].queue[0]->leaveTime;

                    //Mark number of ready customers and unserved Customers
                    numReady++;
                    numUnservedCustomers--;

                    //Add customer to ready array
                    readyCustomers[j] = stylistArray[j].queue[0];
                    //Make active customer index null
                    stylistArray[j].queue[0] = NULL;

                    if (stylistArray[j].queue[1] != NULL) {
                        dequeueCustomer(&stylistArray[j]);
                    }
                }
            }
            //Sort the served customers by leaving time
            sortCustomersByLeaveTime(numReady, *readyCustomers);

            //Print and clean up the customers
            serveCustomers(readyCustomers, numReady);
            for (int j = 0; j < numReady; j++) {
                readyCustomers[j] = NULL;
            }
        }
    }

    /** COMPLETED PLACING PEOPLE IN HEAP QUEUES **/

//    printf("Insertion portion completed\n\nCommencing testing...\n");
//    int testNum = 0;
//    while (testNum <= stylistArray[0].numQueued) {
//        printf("%s, ", stylistArray[0].queue[testNum]->customerName);
//        testNum++;
//    }
//    printf("\n");
//    testNum = 0;
//    while (testNum <= stylistArray[1].numQueued) {
//        printf("%s, ", stylistArray[1].queue[testNum]->customerName);
//        testNum++;
//    }
//    printf("\n");
//    testNum = 0;
//    while (testNum <= stylistArray[2].numQueued) {
//        printf("%s, ", stylistArray[2].queue[testNum]->customerName);
//        testNum++;
//    }
//
//    printf("\nTesting completed\n");

    // Loop through each heap until each remaining customer is served
    while (numUnservedCustomers > 0) {
        int readyCustomerIndex = 0;

        //Probe for first non-null index
        while (stylistArray[readyCustomerIndex].queue[0] == NULL){
            readyCustomerIndex++;
        }

        //For each station, not including first
        for (int i = 0; i<numStylists; i++){
            //Find the smallest leave time for the "Ready" customer
            if (stylistArray[i].queue[0] != NULL && (stylistArray[i].totalTimeWorked+stylistArray[i].queue[0]->serviceTime < stylistArray[readyCustomerIndex].totalTimeWorked+stylistArray[readyCustomerIndex].queue[0]->serviceTime)) {
                readyCustomerIndex = i;
            }
        }

        //Calculate leave time
        stylistArray[readyCustomerIndex].queue[0]->leaveTime = stylistArray[readyCustomerIndex].totalTimeWorked + stylistArray[readyCustomerIndex].queue[0]->serviceTime;
        //Mark time that the stylist worked
        stylistArray[readyCustomerIndex].totalTimeWorked = stylistArray[readyCustomerIndex].queue[0]->leaveTime;

        //Mark stylist who worked
        strcpy(stylistArray[readyCustomerIndex].queue[0]->actualStylist, stylistArray[readyCustomerIndex].stylistName); //Name will be updated even if preferred stylist exists

        //Serve customer and set index to NULL
        serveSingleCustomer(stylistArray[readyCustomerIndex].queue[0]);
        stylistArray[readyCustomerIndex].queue[0] = NULL;

        if (stylistArray[readyCustomerIndex].queue[1] != NULL && stylistArray[readyCustomerIndex].numQueued > 0) {
            dequeueCustomer(&stylistArray[readyCustomerIndex]);
        }

        //Decrement number of unserved customer counter
        numUnservedCustomers--;
    }

    /** COMPLETED PRINTING REMAINING CUSTOMERS **/

    //Free the memory-allocated info of the stations
    for (int i = 0; i < numStylists; i++) {
        //Free heap array
        free(stylistArray[i].queue);
        //Free stylist name malloc
        free(stylistArray[i].stylistName);
        //Free stylist struct at array
        free(stylistPtrTracker[i]);
    }

    //Close files
    fclose(in);
    fclose(out);
}
customer *createCustomerNode() {
    //Create space for the node
    customer *customerNode = calloc(1 , sizeof(customer));
    char *customerName = calloc(MAX_NAME_SIZE , sizeof(char));
    char *stylistName = calloc(MAX_NAME_SIZE ,  sizeof(char));
    char *actualStylistName = calloc(MAX_NAME_SIZE ,  sizeof(char));
    int timeArrived;
    int timeService;
    int currentPoints;

    //Scan in data
    fscanf(in, "%d", &timeArrived);
    fscanf(in, "%s", customerName);
    fscanf(in, "%s", stylistName);
    fscanf(in, "%d", &currentPoints);
    fscanf(in, "%d", &timeService);

    //Assign/initialize data
    customerNode->timeArrival = timeArrived;
    customerNode->customerName = customerName;
    customerNode->preferredStylist = stylistName;
    customerNode->actualStylist = actualStylistName;
    customerNode->loyaltyPoints = currentPoints;
    customerNode->serviceTime = timeService;
    customerNode->leaveTime = 0; //Leave time initialized as 0
    customerNode->isFirst = 0; //Initialize marker to false

    return customerNode;
}

//Returns the station index of the stylist if they are working, else returns -1
int getSeatOfStylist(char *stylistName, int numStylists, stylistQueue *seatArr) {
    if (strcmp(stylistName, "NONE") == 0) return -1;
    else {
        for (int i = 0; i < numStylists; i++) {
            if (strcmp(stylistName, seatArr[i].stylistName) == 0) return i;
        }
        return -1;
    }
}

//Returns the index of the station with the lowest number of people queued
int findIndexOfSmallestQueue(int numStylists, stylistQueue *seatArr) {
    //First min is assumed to be at first index
    int minIndex = 0;

    //Test for empty active seats. Return first empty station
    for (int i = 0; i < numStylists; i++) {
        if (seatArr[i].queue[0] == NULL && seatArr[i].queue[1] == NULL) return i;
    }
    for (int i = 1; i < numStylists; i++) { //First index can be skipped in loop.
        //If queue size of current loop is less than queue size of minimum marked index, mark new minimum index
        if (seatArr[i].numQueued < seatArr[minIndex].numQueued) minIndex = i;
        //Else current station receives customer. Station number will always be less than compared seat
    }
    return minIndex;
}

void insertCustomer(stylistQueue *station, customer *newCustomer) {
    //If the customer is the first to be implemented into the queue
    if (station->queue[0] == NULL && station->queue[1] == NULL) {
        //Mark customer as first
        newCustomer->isFirst = 1;
        //Add customer
        station->queue[0] = newCustomer;
        //Update total time worked to "waiting" time for first customer
        station->totalTimeWorked = newCustomer->timeArrival;
        return;
    }
    else {
        //Increase number of queued people
        station->numQueued++;
        //Add customer to the queue
        station->queue[station->numQueued] = newCustomer;
        //Percolate the customer node
        percolateUp(station, station->numQueued);
    }
}

void percolateUp(stylistQueue *station, int index) {
    // Stop percolation at root
    if (index > 1) {
        //If parent's loyalty points is less, percolate up
        if (station->queue[index/2]->loyaltyPoints < station->queue[index]->loyaltyPoints) {
            //Swap nodes
            swap(station, index, index / 2);
            //Recursively call percolation for continuation
            percolateUp(station, index / 2);
        }
    }
}

//Function should not be called if there are no customers in the queue (Queue[1] == null)
void dequeueCustomer(stylistQueue * station) {
    //If there are no queued customers, do not dequeue. "Ready" index remains NULL
    if (station->numQueued < 1) {
        return;
    }

    //If there are more than one queue members
    if (station->numQueued > 1) {
        //Move the top-heap node to be in the "active" index
        station->queue[0] = station->queue[1];
        //Take bottom value to top
        station->queue[1] = station->queue[station->numQueued];
        //Make the previously final index NULL
        station->queue[station->numQueued] = NULL;
    }
    //If there was only one queue member
    else if (station->numQueued == 1) {
        //Move the top-heap node to be in the "active" index
        station->queue[0] = station->queue[1];
        station->queue[1] = NULL;
    }

    //Mark size of queue as having one less customer, only if the customer was not the first of the day for the queue
    if (station->queue[0]->isFirst == 0) {
        station->numQueued--;
        if (station->numQueued < 0) station->numQueued = 0; //prevent negative counting
    }

    //Percolate item to proper place
    percolateDown(station, 1);
}

// Runs percolate down on the heap pointed to by h on the node stored in index.
void percolateDown(stylistQueue *station, int index) {
    int maxIndex;

    //Only percolate within active indices
    if ((2*index+1) <= station->numQueued) {
        //Find child node with greater loyalty point count
        maxIndex = compareCustomers(station->queue[2*index], 2*index,station->queue[2*index+1], 2*index+1);
        //If this loyalty count is greater than current, swap and continue percolation
        if (station->queue[index]->loyaltyPoints < station->queue[maxIndex]->loyaltyPoints) {
            swap(station, index, maxIndex);
            percolateDown(station, maxIndex);
        }
    }
    //One left child case
    else if (station->numQueued == 2 * index) {
        //Only compare the child and parent
        if (station->queue[index]->loyaltyPoints < station->queue[2 * index]->loyaltyPoints)
            swap(station, index, 2*index);
    }
}

int compareCustomers(customer *customer1, int indexOfCustomer1, customer *customer2, int indexOfCustomer2) {
    //If first customer has more points
    if (customer1->loyaltyPoints > customer2->loyaltyPoints) return indexOfCustomer1;
    //Else if both point counts are equal
    else if (customer2->loyaltyPoints == customer1->loyaltyPoints) {
        //If first customer preferred the stylist
        if (strcmp(customer1->preferredStylist, customer1->actualStylist) == 0 && strcmp(customer2->preferredStylist, customer2->actualStylist) != 0) {
            return indexOfCustomer1;
        //If second customer preferred the stylist
        } else if (strcmp(customer2->preferredStylist, customer2->actualStylist) == 0 && strcmp(customer1->preferredStylist, customer1->actualStylist) != 0) {
            return indexOfCustomer2;
        //Else if either both or neither preferred the stylist
        } else if ((strcmp(customer1->preferredStylist, customer1->actualStylist) == 0 && strcmp(customer2->preferredStylist, customer2->actualStylist) == 0) || (strcmp(customer1->preferredStylist, customer1->actualStylist) != 0 && strcmp(customer2->preferredStylist, customer2->actualStylist) != 0)) {
            if (strcmp(customer1->customerName, customer2->customerName) < 0) return indexOfCustomer1;
            else return indexOfCustomer2;
        }
    }
    //Else if the second customer has more points
    else if (customer1->loyaltyPoints < customer2->loyaltyPoints) {
        return indexOfCustomer2;
    }
    //Default to first customer
    return indexOfCustomer1;
}

void swap(stylistQueue *station, int indexA, int indexB) {
    customer *temp = station->queue[indexA];
    station->queue[indexA] = station->queue[indexB];
    station->queue[indexB] = temp;
}

//Regular sort for an array that should always be less than 10 units. Binary algorithms are unneeded
void sortCustomersByLeaveTime(int numReady, customer data[]) {
    for (int i = 0; i < numReady - 1; i++) {
        for (int j = 0; j < numReady - i - 1; j++) { //Previously handled i array elements already sorted
            if (data[j].leaveTime > data[j + 1].leaveTime) {
                sortSwap(data, j);
            }
        }
    }
}

void sortSwap(customer customerArr[], int index) {
    customer temp = customerArr[index + 1];
    customerArr[index + 1] = customerArr[index];
    customerArr[index] = temp;
}

void serveCustomers(customer **readyCustomers, int numReady) {
    if (numReady > 0) {
        for (int i = 0; i < numReady; i++) {
            //Print details
            printf("%s ", readyCustomers[i]->customerName);
            printf("%d ", readyCustomers[i]->leaveTime);
            printf("%d ", readyCustomers[i]->loyaltyPoints + (readyCustomers[i]->serviceTime / 10));
            printf("%s\n", readyCustomers[i]->actualStylist);
            fprintf(out, "%s ", readyCustomers[i]->customerName);
            fprintf(out, "%d ", readyCustomers[i]->leaveTime);
            fprintf(out, "%d ", readyCustomers[i]->loyaltyPoints + (readyCustomers[i]->serviceTime / 10));
            fprintf(out, "%s\n", readyCustomers[i]->actualStylist);
            //Free stylist name mallocs
            free(readyCustomers[i]->preferredStylist);
            readyCustomers[i]->preferredStylist = NULL;
            free(readyCustomers[i]->actualStylist);
            readyCustomers[i]->actualStylist = NULL;
            //Free customer name malloc
            free(readyCustomers[i]->customerName);
            readyCustomers[i]->customerName = NULL;
            //Free customer struct
            free(readyCustomers[i]);
            readyCustomers[i] = NULL;
        }
    }
}

void serveSingleCustomer(customer *readyCustomer) {
    //Print details
    printf("%s ", readyCustomer->customerName);
    printf("%d ", readyCustomer->leaveTime);
    printf("%d ", readyCustomer->loyaltyPoints + (readyCustomer->serviceTime / 10));
    printf("%s\n", readyCustomer->actualStylist);
    fprintf(out, "%s ", readyCustomer->customerName);
    fprintf(out,"%d ", readyCustomer->leaveTime);
    fprintf(out, "%d ", readyCustomer->loyaltyPoints + (readyCustomer->serviceTime / 10));
    fprintf(out, "%s\n", readyCustomer->actualStylist);
    //Free stylist name mallocs
    free(readyCustomer->preferredStylist);
    readyCustomer->preferredStylist = NULL;
    free(readyCustomer->actualStylist);
    readyCustomer->actualStylist = NULL;
    //Free customer name malloc
    free(readyCustomer->customerName);
    readyCustomer->customerName = NULL;
    //Free customer struct
    free(readyCustomer);
    readyCustomer = NULL;
}