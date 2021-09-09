#include "statHead.h"

double mean (int totnum, double data[])	{	//Calculate Mean
	double meanval, sum;
	int i;
	
	sum = 0.0;
	for(i=0; i<totnum; i++) {
		sum += data[i];
	}
	meanval = sum / totnum;
	return meanval;
}

double minimum(int totnum, double data[])	{	//Calculate Minimum
	double minval;
	int i;
	
	minval = data[0];
	for(i=1; i<totnum; i++) {
		if(data[i] < minval) {
			minval = data[i];
		}
	}
	return minval;
}

double maximum(int totnum, double data[])	{	//Calculate Maximum
	double maxval;
	int i;
	
	maxval = data[0];
	for(i=1; i<totnum; i++) {
		if(data[i] > maxval) {
			maxval = data[i];
		}
	}
	return maxval;
}

double median(int totnum, double data[])	{
	double medianval;
	
	if(totnum%2 == 1) {	//Odd Total Number
		medianval = data[totnum/2];
	}
	else {	//Even Total Number
		medianval = (data[totnum/2-1] + data[totnum/2])/2.0;
	}
	return medianval;
}

double variance(int totnum, double data[])	{ //Calculate Variance
	double meanval, sum, var;
	int i;
	
	meanval = mean(totnum, data);
	sum = 0.0;
	for(i=0; i<totnum; i++) {
		sum += (data[i] - meanval)*(data[i] - meanval);
	}
	var = sum/(totnum-1);
	return var;
}

double stdDeviation(int totnum, double data[])	{ //Calculate Standard Deviation
	double var, std;
	
	var = variance(totnum, data);
	std = sqrt(var);
	return std;
}

int aboveVal(int totnum, double data[], double val)	{ //Calculate the Number of Elements Greater than a Specified Value
	int i, num;
	for(i=0, num=0; i<totnum; i++)	{
		if(data[i] >= val){
			num++;
		}
	}
	return num;
}

void baseSort(int totnum, double data[])	{ //Sort Array
	int i, j, imin;
	double temp;
	
	for(i=0; i<totnum-1; i++) {
		imin=i;
		for(j=i+1; j<totnum; j++) {
			if(data[imin] > data[j]) {
				imin = j;
			}
		}
		if(imin != i) {
			temp = data[i];
			data[i] = data[imin];
			data[imin] = temp;
		}
	}
}

void bubbleSort(int totnum, double data[])	{ //Bubble-sort Array
	int i,j;
	double temp;
	
	for(i=0; i<totnum-1; i++) {
		for(j=0; j<totnum-i-1; j++) { //Previously handled i array elements already sorted
			if(data[j] > data[j+1]) {
				temp = data[j+1];
				data[j+1] = data[j];
				data[j] = temp;
			}
		}
	}
}