#ifndef SYMNMF_H
#define SYMNMF_H
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>

/*X is the datapoints*/
double** sym(double** X);
double** ddg(double** X);
double** norm(double** X);
double** symnmf(double** W,double** H,int k);

#endif
