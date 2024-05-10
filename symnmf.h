#ifndef SYMNMF_H
#define SYMNMF_H
#include <stdio.h>
#include <math.h>

/*X is the datapoints*/
double** sym(double** X,int n,int d);
double** ddg(double** X,int n,int d);
double** norm(double** X,int n,int d);
double** symnmf(double** W,double** H,int n,int k);

#endif
