/* COP 3502C Assignment 1
This program is written by: Rylan Simpson */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leak_detector_c.h"

FILE *fileInput; 
//fileInput = fopen("in.txt","r");

FILE *fileOutput;
//fileOutput = fopen("out.txt", "w");

	
typedef struct item { //This stores one component of a smoothie recipe
 	int itemID; //Ingredient number
 	int numParts; //Number of parts / ratio
} item;
	
typedef struct recipe {
 	int numItems; //Number of different recipes
 	item* itemList; //DMA array of item (above), each element stores one ingredient
 	int totalParts; //Sum of the number of parts of each ingredient
} recipe;

//List of functions
//char** readIngredients(int *numIngredients);
//recipe* readRecipe();
//recipe** readAllRecipes(int *numRecipes);
//double* calculateOrder(int ingredientCount, int numSmoothies, recipe** recipeList);
//void printOrder(char** ingredientNames, double* orderList, int numIngredients);
//void freeIngredients(char** ingredientList, int numIngredients);
//void freeRecipes(recipe** allRecipes, int numRecipes);
	
	/* Pre-condition: reference to a variable to store number of ingredients.
	 Post-condition: Reads in numIngredients and that number of strings from 
	 the inputs, allocates an array of
	 strings to store the input, and sizes each
	 individual string dynamically to be the
	 proper size (string length plus 1), and 
	 returns a pointer to the array. */
char** readIngredients(int *numIngredients) {
	int i;
	char **ingredList = calloc(*numIngredients, sizeof(char*));
		for (i=0; i<*numIngredients; i++){
			char *temp = calloc(26, sizeof(char)); //Strings do not exceed 25 (+1) characters, create temporary array of characters
			fscanf(fileInput, "%s", temp);
		//	printf("%s\n", temp);
			ingredList[i] = malloc((strlen(temp)+1)*sizeof(char)); //allocate to size of temp[i] string 
			strcpy(ingredList[i], temp);
			free(temp);
		}
	return ingredList;
}
	
	/*Pre-condition: does not take any parameter
	Post-condition: Reads in details of a recipe such as numItems,
	Dynamically allocates space for a single
	recipe, dynamically allocates an array of
	item of the proper size, updates the
	numItems field of the struct, fills the
	array of items appropriately based on the 
	input and returns a pointer to the struct
	dynamically allocated. */
recipe* readRecipe() { //SEG FAULT HERE? [FIXED] -m "Removed temporary holders and scanned directly"
	int b;
	int ingredTemp = 0;
	int *numIngred = &ingredTemp;
	recipe* fullRecipe = (recipe*)malloc(sizeof(recipe));
	fscanf(fileInput, "%d", numIngred);
//	printf("%d ", *numIngred);
	fullRecipe->itemList = calloc((*numIngred + 1), sizeof(item)); //Account for \0
	fullRecipe->numItems = *numIngred;
	fullRecipe->totalParts = 0; //Initialize to 0
		for (b=0; b<ingredTemp; b++){
			fscanf(fileInput, "%d", &fullRecipe->itemList[b].itemID);
		//	printf("%d ", fullRecipe->itemList[b].itemID);
			fscanf(fileInput, "%d", &fullRecipe->itemList[b].numParts);
		//	printf("%d ", fullRecipe->itemList[b].numParts);
			fullRecipe->totalParts = fullRecipe->totalParts + fullRecipe->itemList[b].numParts;
		} 
	//	printf("\n");
		return fullRecipe;
	}
	
	/*Pre-condition: reference to a variable to store number of recipes.
	Post-condition: Read number of recipes. Dynamically allocates an array of 
	pointers to recipes of size numRecipes, reads numRecipes
	number of recipes from standard input, creates
	structs to store each recipe and has the 
	pointers point to each struct, in the order 
	the information was read in. (Should call
	readRecipe in a loop.) */
recipe** readAllRecipes(int *numRecipes) {   //Reintroduce numItems if needed
	int i;
	recipe** allRecipes = (recipe**)calloc((*numRecipes), sizeof(recipe*));
		for (i=0; i<*numRecipes; i++){
			allRecipes[i] = readRecipe();
		}	
	return allRecipes;
	}
	
	/*Pre-condition: 0 < numSmoothies <= 100000, recipeList is 
	pointing to the list of all smoothie recipes and 
	numIngredients equals the number of total ingredients (you have 
	already read it in the first line of the input).
	Post-condition: Reads in information from standard input
	about numSmoothies number of smoothie orders and dynamically 
	allocates an array of doubles of size numIngredients such 
	that index i stores the # of pounds of ingredient i
	needed to fulfill all smoothie orders and returns a pointer 
	to the array.*/
double* calculateOrder(int numIngredients, int numSmoothies, recipe** recipeList) { //ingrediuenCount == numIngredients?
	double* order = (double*)calloc(numIngredients, sizeof(double));
	int i, j;
		for (i=0; i<numSmoothies; i++) {
			int ingredID;
			int ingredWeight;
			fscanf(fileInput, "%d", &ingredID);
		//	printf("%d ", ingredID);
			fscanf(fileInput, "%d", &ingredWeight);
		//	printf("%d ", ingredWeight);			
				for (j=0; j<recipeList[ingredID]->numItems; j++){
					order[recipeList[ingredID]->itemList[j].itemID] += ((double)(ingredWeight)*(recipeList[ingredID]->itemList[j].numParts)/(recipeList[ingredID]->totalParts));
			}
		}
	//	printf("CALCULATION SUCCESS\n");
	return order;
	}
	
	/*Pre-conditions: ingredientNames store the names of each
	ingredient and orderList stores the amount
	to order for each ingredient, and both arrays 
	are of size numIngredients.
	Post-condition: Prints out a list, in ingredient order, of each
	ingredient, a space and the amount of that
	ingredient to order rounded to 6 decimal
	places. One ingredient per line. */
void printOrder(char** ingredientNames, double* orderList, int numIngredients)	{
	int i;
		for (i=0; i<numIngredients; i++){
			if(orderList[i] == 0){
				continue;
			}
		fprintf(fileOutput, "%s %.6f\n",ingredientNames[i],orderList[i]);
		printf("%s %.6f\n", ingredientNames[i], orderList[i]);
		}
		fprintf(fileOutput, "\n");
		printf("\n");
	}
	
	/*Pre-conditions: ingredientList is an array of char* of size 
	numIngredients with each char* dynamically allocated.
	Post-condition: all the memory pointed to by ingredientList is 
	freed. */
void freeIngredients(char** ingredientList, int numIngredients) {
	int i;
		for(i=0; i<numIngredients; i++){
			free(ingredientList[i]);
		}
	free(ingredientList);
	}
	
	/*Pre-conditions: allRecipes is an array of recipe* of size 
	numRecipes with each recipe* dynamically allocated 
	to point to a single recipe.
	Post-condition: all the memory pointed to by allRecipes is 
	freed. */
void freeRecipes(recipe** allRecipes, int numRecipes) {
	int i;
		for(i=0; i<numRecipes; i++) {
		free(allRecipes[i]->itemList);
       	free(allRecipes[i]);
    }
	free(allRecipes);
	}
	
int main(void){
	atexit(report_mem_leak);
	
	//List of funtion prototypes
/*
char** readIngredients(int *numIngredients)
recipe* readRecipe();
recipe** readAllRecipes(int *numRecipes);
double* calculateOrder(int ingredientCount, int numSmoothies, recipe** recipeLis; //Num smoothies is smoothies per store. Ingredient count is ???
void printOrder(char** ingredientNames, double* orderList, int numIngredients) ;
void freeIngredients(char** ingredientList, int numIngredients);
void freeRecipes(recipe** allRecipes, int numRecipes);
*/
	
	fileInput = fopen("in.txt", "r");
	fileOutput = fopen("out.txt", "w");
	
	recipe** smoothieList;  //Array of pointers to recipes
	double* amtOfEachItem; //Array of the amount, in weight, of each ingredient
	char** listOfIngred;
	
	//Initialization for function parameters and pointers
	int ingredHolder = 0;
	int recipeHolder = 0;
	int smoothieHolder = 0;
	int storeHolder = 0;
	int SPSHolder = 0;
	
	int *numIngredients = &ingredHolder;
	int *numRecipes = &recipeHolder;
	int *numSmoothies = &smoothieHolder;
	int *numStores = &storeHolder;
	int *smoothiesPstore = &SPSHolder;
	
	fscanf(fileInput, "%d", numIngredients); //Number of ingredients saved here: ingredHolder is updated
//	printf("%d\n", *numIngredients);
	listOfIngred = readIngredients(numIngredients);
	
	fscanf(fileInput, "%d", &smoothieHolder);
//	printf("%d\n", smoothieHolder);
	smoothieList = readAllRecipes(numSmoothies);
	
	fscanf(fileInput, "%d", numStores); //Number of stores saved here: storeHolder is udated
//	printf("%d\n", *numStores);
	
	int a;
	for (a=0; a<*numStores; a++){
		
		//Multiple variable instances created for possible neccessary changes
		int SPStemp = 0;
		int *SPSPtr = &SPStemp;
	//	printf("\nStore #%d\n", a + 1);
		fprintf(fileOutput, "Store #%d:\n", a + 1);
		printf("Store #%d:\n", a + 1);
        fscanf(fileInput, "%d", SPSPtr);
        smoothiesPstore = SPSPtr;
        
        amtOfEachItem = calculateOrder(ingredHolder, SPStemp, smoothieList);
        printOrder(listOfIngred, amtOfEachItem, ingredHolder);
        free(amtOfEachItem);
	}

	freeIngredients(listOfIngred, ingredHolder);
	freeRecipes(smoothieList, smoothieHolder);

	fclose(fileInput);
	fclose(fileOutput);
	
	return 0;
}