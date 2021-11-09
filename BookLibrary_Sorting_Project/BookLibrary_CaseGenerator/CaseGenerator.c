/* COP 3502C Assignment 4
This program is written by: Rylan Simpson */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FILE *out;

int main(){
    out = fopen("out.txt", "w");
    srand(time(0));
    int numRands = 1000000;
    int numCases = 1;
    long long numThreshold = 10000000000;
    int maxRand = 100000+1;
    fprintf(out, "%d\n", numCases);
    fprintf(out,"%d %lld\n", numRands, numThreshold);
    for (int i = 0; i<numRands; i++){
        int r = rand() % maxRand;
        fprintf(out,"%d ", r);
    }
    fprintf(out, "\n");
    fclose(out);
}

