/* COP 3502C Assignment 1
This program is written by: Rylan Simpson */

/*Given the (x, y) positions of 2n treehouse,
 of all possible ways to create n pairs of distinct trees,
 find the minimum possible sum of distances between each tree in the pairs*/
//-10000 ≤ xi, yi ≤ 10000 is the maximum case
//Travelling salesman problem, use inspirations of similar known algorithms methodologies, such as Held–Karp algorithm and nearest neighbor algorithm

#include <stdio.h>
#include <math.h>
#define LIMIT 999999   //SYMBOL IS STATIC

//Commented out print lines are used for debugging purposes - do not remove

FILE* fileIn;
FILE* fileOut;

//Calculate distace between two points using distance formula
double distanceCalc(int x1, int y1, int x2, int y2){
	double dist = sqrt(((x1-x2)*(x1-x2))+((y1-y2)*(y1-y2)));
	return dist;
}

//Calculate distance between coordinates indices and compare if the length is less than the current set length. If so, then save it to the length variable
double bestDistCalc( int xcoord[], int ycoord[], int tracker[], int numConnections) {
        //printf("Calculation entered\n");
		double currDist = 0; 
		int i, j;
        int marker[numConnections];
        //printf("(4)Operation completed\n");
        for(i=0; i<numConnections; i++) {
            marker[i] = tracker[i];
        }

        for(j=0; j<numConnections; j++) {
                if(marker[j] == 1) { /////
					continue;
				}
                int index1 = marker[j];
                int index2 = marker[index1];
                //printf("Current x1 coord: %d\n", xcoord[index1]);
                //printf("Current y1 coord: %d\n", ycoord[index1]);
                //printf("Current x1 coord: %d\n", xcoord[index2]);
                //printf("Current y2 coord: %d\n", ycoord[index2]);
                //double distanceCalc(int x1, int y1, int x2, int y2)
                currDist += distanceCalc(xcoord[index1], ycoord[index1], xcoord[index2], ycoord[index2]);
                marker[j] = 1; /////
                marker[index1] = 1; /////
        }
        return currDist;
}

//Recursively iterate through different combinations of coordinates, and run the distance calculation functions if the distance for that combo has not already been calculated.
//This function keeps track of and skips over different variations of the same pair of coordinates
double distCalcRecursive( int xcoord[], int ycoord[], int marked[], int tracker[], int numConnections, int iterations, double length) {
    //printf("Recurion entered\n");
	if (iterations == numConnections){
		//printf("Final conditions met\n");
		//Calculate the best distance of a given set of combinations
        return bestDistCalc(xcoord, ycoord, tracker, numConnections);
    }
    if(marked[iterations]!=1) {  /////
    	//printf("Iteration if statement entered\n");
        tracker[iterations] = marked[iterations];
        //printf("(2)Operation completed\n");
        double bestDist = distCalcRecursive(xcoord, ycoord, marked, tracker, numConnections, iterations+1, length);
        if (bestDist < length){
        	//printf("Remark loop entered\n");
			length = bestDist;
		}
    }
    else {
    	int i=0;
        for (i=0; i<numConnections+1; i++) {
        	//printf("Else statement entered\n");
            if(marked[i] != 1) {  /////
            	//printf("(1)Continue statement entered\n");
				continue;
        	}
        	if(i==iterations) {
				//printf("(2)Continue statement entered\n");
				continue;
       	 	}
            marked[i] = iterations;
            marked[iterations] = i;
            tracker[iterations] = i;
            //printf("(3)Operation completed\n");
            //Enter recursion for next iterative sequence
            double bestDist = distCalcRecursive(xcoord, ycoord, marked, tracker, numConnections, iterations+1, length);
            if (bestDist < length){
            	//printf("Remark loop entered\n");
				length = bestDist;
			}
			//printf("Remark loop passed\n");
            marked[i] = 1;  /////
            marked[iterations] = 1;  /////
        }
    }
    //printf("Returning value\n");
    return length;
}

//This function calls the recursive function into operation, while initializing its needed values for a given test case
double shortestDistanceSetup(int xcoord[], int ycoord[], int numConnections, double length){
	//This funciton serves as a constructor for the recursive functions 
	int i;
	int iterations = 0;
	int marked[numConnections*2], tracker[numConnections*2]; // results[numConnections*2];
	for (i=0; i<numConnections*2; i++){
		marked[i]=1;  /////
	}
	double optimalDist = 0;
	//printf("(1)Operation completed\n");
	optimalDist = distCalcRecursive(xcoord, ycoord, marked, tracker, (numConnections*2), iterations, length);
	return optimalDist;
}

int main(void){
	fileIn = fopen("in.txt", "r");
	fileOut = fopen("out.txt", "w");
	int numCases, tests, i;
	//Number of test cases
	fscanf(fileIn, "%d", &numCases);
	
	for(tests = 0; tests<numCases; tests++) {
		int numConnections;
		//Number of tree connections (*2 = number of trees)
		fscanf(fileIn, "%d",&numConnections);
		int xcoord[numConnections*2], ycoord[numConnections*2];
		//Tree coordinates
		for(i=0; i < numConnections*2; i++) {
            fscanf(fileIn, "%d%d", &xcoord[i], &ycoord[i]);
        }
        //Run the recursionoary program. Length is set the the maximum value so that any following value will overtake it
		double length = LIMIT;    
		length = shortestDistanceSetup(xcoord, ycoord, numConnections, length);
		//Length of summed connections
        printf("%.3f\n", length);
        fprintf(fileOut, "%.3f\n", length);
	}
	//Close files upon completion
	fclose(fileIn);	
	fclose(fileOut);
}