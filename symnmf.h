#ifndef SYMNMF_H
#define SYMNMF_H
#include <stdio.h>
#include <math.h>

/*X is the datapoints*/
double** symg(double** X,int n,int d);
double** ddgg(double** X,int n,int d);
double** normg(double** X,int n,int d);
double** symnmfg(double** W,double** H,int n,int k);

#endif
