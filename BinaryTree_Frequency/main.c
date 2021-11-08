/* COP 3502C Assignment 5
This program is written by: Rylan Simpson */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//File input and out
FILE* in;
FILE* out;

typedef struct info {
    char* word;
    int freq;
} info;

//Node structure
typedef struct node {
    info *data;
    struct node *left;
    struct node *right;
} node;

//Set starting index for copying to zero
int arrayIndex = 0;

//Function prototypes
node* createNode(char newWord[]);
node* insertNode(node* root, node* element);
int getTreeSize(node* root);
int getDepthOfNode(node* root, char qWord[], int depth);
int getFreqOfNode(node* root, char qWord[]);
void printInorder(node* treeNode);
void copyToArrayInorder(node* root, info* nodeArray[]);
int compareTo(info *data1, info *data2);
void mergeReverse(info* arr[], int l, int m, int r);
void mergeSortReverse(info* arr[], int l, int r);

int main() {
    in = fopen("in.txt", "r");
    out = fopen("out.txt", "w");

    //Get the total number of cases to run
    int totalCases;
    fscanf(in, "%d", &totalCases);
    //Print number of cases, for debugging
    printf("%d\n", totalCases);

    //Create the pointer to the root node
    node* root = NULL;

    //For the total number of cases, build tree (1) and print results (2)
    for (int i = 0; i < totalCases; i++){
        int option = 0;
        //Max string size is 20 characters
        char* word = (char*)malloc(sizeof(char)*20);

        //Scan option for handing word
        fscanf(in, "%d", &option);
        //Scan word
        fscanf(in, "%s", word);

        //If option is 1, either increment an existing a node or add the node to the tree
        if (option == 1) {
            node* newElement = createNode(word);
            root = insertNode(root, newElement);
        }
        //If the option is 2, print the frequency of the word and the node depth
        else if (option == 2) {
            //Print word frequency
            int wordFreq = getFreqOfNode(root, word);
            fprintf(out, "%d ", wordFreq);
            printf( "%d ", wordFreq);
            //Print word depth
            int wordDepth = getDepthOfNode(root, word, 0);
            fprintf(out, "%d\n", wordDepth);
            printf( "%d\n", wordDepth);
        }

    }

    printf( "\n");

    //Print tree with inorder, for debugging
       printInorder(root);
       printf("\n");

    // PART 1 COMPLETED

    //Get size of tree
    int totalNumNodes = getTreeSize(root);
    printf("%d\n\n", totalNumNodes);

    //Allocate memory for array of nodes
    info* nodeArray[totalNumNodes];

    //Set all indices to null
    for (int i = 0; i < totalNumNodes; i++){
        nodeArray[i] = NULL;
    }

    copyToArrayInorder(root, nodeArray);
    //Reset global index back to 0
    arrayIndex = 0;

    //Print array, not sorted by frequency
    for (int i = 0; i < totalNumNodes; i++){
        printf("%s, ", nodeArray[i]->word);
    }
    printf("\n\n");

    // Part 2

    mergeSortReverse(nodeArray, 0, totalNumNodes-1);

    //Print array words, sorted by frequency
    for (int i = 0; i < totalNumNodes; i++){
        printf("%s, ", nodeArray[i]->word);
    }
    //Print array frequencies, sorted
    printf("\n");
    for (int i = 0; i < totalNumNodes; i++){
        printf("%d, ", nodeArray[i]->freq);
    }

    /*
     * Print results of sorted array
     * */

    /*
     * Make tree free node function
     * */

    fclose(in);
    fclose(out);
}

node* createNode(char newWord[]){
    //Allocate memory
    node* treeNode = (node*) malloc(sizeof(node));
    info* nodeInfo = (info*) malloc(sizeof(info));
    //Initialize data
    nodeInfo->word = newWord;
    nodeInfo->freq = 1;
    treeNode->data = nodeInfo;
    treeNode->right = NULL;
    treeNode->left = NULL;

    //Return treeNode
    return treeNode;
}

node* insertNode(node* root, node* element) {
    //Begin tree if tree is empty
    if (root == NULL) {
        return element;
    }
    //Else follow tree until place for element is found or until existing element is found
    else {

        //If the element string is less than root string
        if (strcmp(element->data->word, root->data->word) < 0) {
            //If left pointer is not null, continue going down tree
            if (root->left != NULL) {
                insertNode(root->left, element);
            }
            //Else if pointer is null, place element as leaf
            else {
                root->left = element;
            }
        }
        //If the element string is greater than root string
        else if (strcmp(element->data->word, root->data->word) > 0) {
            //If right pointer is not null, continue going down tree
            if (root->right != NULL) {
                insertNode(root->right, element);
            }
            //Else if pointer is null, place element as leaf
            else {
                root->right = element;
            }
        }
        //If string exists in tree
        else if (strcmp(element->data->word, root->data->word) == 0) {
            //Increment frequency counter
            root->data->freq = root->data->freq + 1;
            //Free element node that was to be placed in tree
            free(element->data);
            free(element);
        }

    }

    //Return the root node of the expanded/node-incremented tree
    return root;
}

int getTreeSize(node* root){
    if (root == NULL){
        return 0;
    }
    else {
        return (1 + getTreeSize(root->left) + getTreeSize(root->right));
    }
}

int getFreqOfNode(node* root, char qWord[]){
    //Return -1 if tree is null
    if (root == NULL){
        return -1;
    }

    else {

        //If word is less than root word, continue down tree
        if (strcmp(qWord, root->data->word) < 0) {
            if (root->left != NULL) {
                return getFreqOfNode(root->left, qWord);
            }
            //If pointer is null, return -1
            else {
                return -1;
            }
        }
        //If word is greater than root word, continue down tree
        else if (strcmp(qWord, root->data->word) > 0) {
            if (root->right != NULL) {
                return getFreqOfNode(root->right, qWord);
            }
            //If pointer is null, return -1
            else {
                return -1;
            }
        }
        //Else if comparator is 0, word is found
        else {
            //Return the frequency of the word
            return root->data->freq;
        }

    }
}

//Depth will start at 0 on call
int getDepthOfNode(node* root, char qWord[], int depth){
    //Return -1 if tree is null
    if (root == NULL){
        return -1;
    }

    else {

        //If word is less than root word, continue down tree, increment depth counter
        if (strcmp(qWord, root->data->word) < 0) {
            if (root->left != NULL) {
                return getDepthOfNode(root->left, qWord, depth + 1);
            }
                //If pointer is null, return -1
            else {
                return -1;
            }
        }
            //If word is greater than root word, continue down tree, increment depth counter
        else if (strcmp(qWord, root->data->word) > 0) {
            if (root->right != NULL) {
                return getDepthOfNode(root->right, qWord, depth + 1);
            }
                //If pointer is null, return -1
            else {
                return -1;
            }
        }
            //Else if comparator is 0, word is found
        else {
            //Return the frequency of the word
            return depth;
        }

    }
}

//Print words in Inorder format
void printInorder(node* treeNode) {
    if (treeNode != NULL) {
        printInorder(treeNode->left);
        printf("%s ", treeNode->data->word);
        printInorder(treeNode->right);
    }
}

//arrayIndex is first passed as global 0
void copyToArrayInorder(node* root, info* nodeArray[]){
    if (root != NULL) {
        copyToArrayInorder(root->left, nodeArray);
        nodeArray[arrayIndex++] = root->data;
        copyToArrayInorder(root->right, nodeArray);
    }
}

//From requirement pdf: Takes pointers of two struct (the
//struct that contains the string and frequency) and returns a positive integer if the first struct should
//go up in the list if sorted (based on the criteria mentioned earlier).
int compareTo(info *data1, info *data2) {
    //If the first string is smaller
    if (strcmp(data1->word, data2->word) < 0){
        return 1;
    }
    //If the first string is larger, x == 0 is impossible
    else {
        return -1;
    }
}

void mergeReverse(info* arr[], int l, int m, int r) {
    int i, j, k;
    int lSize = m - l + 1;
    int rSize = r - m;

    //Allocate space for smaller arrays
    info **L = (info**)malloc(lSize*sizeof(info));
    info **R = (info**)malloc(rSize*sizeof(info));

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
    //Refill main array, sorted (Large frequencies first)
    while (i < lSize && j < rSize) {
        if (L[i]->freq > R[j]->freq) {
            arr[k] = L[i];
            i++;
            k++;
        }
        else if (L[i]->freq < R[j]->freq){
            arr[k] = R[j];
            j++;
            k++;
        }
        //If a tie happens, compare strings
        else {
            //If first string is smaller, merge it first
            if (compareTo(L[i], R[j]) == 1) {
                arr[k] = L[i];
                i++;
                k++;
            }
            //If first string is larger, merge second
            else if(compareTo(L[i], R[j]) == -1) {
                arr[k] = R[j];
                j++;
                k++;
            }
            //Catch erroneous bad results
            else {
                printf("\n\n\nERROR OCCURRED IN MERGE\n\n\n");
                exit(-420);
            }
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

void mergeSortReverse(info* arr[], int l, int r) {
    if (l < r) {
        int m = (r + l) / 2;

        //Recursive calls to split down the array, sorting occurs on merge
        mergeSortReverse(arr, l, m);
        mergeSortReverse(arr, m + 1, r);

        //Merge arrays
        mergeReverse(arr, l, m, r);
    }
}
