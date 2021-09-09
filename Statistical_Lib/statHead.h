#ifndef STATLIB_H
#define STATLIB_H

#include <math.h>

extern double mean(int totnum, double data[]);
extern double minimum(int totnum, double data[]);
extern double maximum(int totnum, double data[]);
extern double median(int totnum, double data[]);
extern double variance(int totnum, double data[]);
extern double stdDeviation(int totnum, double data[]);
extern int aboveVal(int totnum, double data[], double val);
extern void baseSort(int totnum, double data[]);
extern void bubbleSort(int totnum, double data[]);

#ifdef _CH_
#pragma importf "statLib.c"
#endif

#endif