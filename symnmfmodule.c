#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdlib.h>
#include "symnmf.h"

static PyObject *sym(PyObject *self, PyObject *args) {
    PyObject *py_X,*py_list,*py_A,*py_tmp;
    int i,j,n,d;
    double **X,**A;
    /* check if we managed to pass them well*/
    if (!PyArg_ParseTuple(args, "O", &py_X)){
        printf("An Error Has Occurred\n");
        exit(1);
    }
    if (!PyList_Check(py_X)){ /*check if its an instance of a subtype of list*/
        printf("An Error Has Occurred\n");
        exit(1);
    }
    
    n = PyObject_Length(py_X);
    d = PyObject_Length(PyList_GetItem(py_X, 0));
    /*allocating spcae*/
    X = (double**)malloc(sizeof(double*)*n);
    if(X == NULL){
        printf("An Error Has Occurred\n");
        exit(1);
    }
    for (i = 0; i < n; i++)
    {
        X[i] = (double*)malloc(sizeof(double)*d);
        if(X[i]==NULL){
            printf("An Error Has Occurred\n");
            exit(1);
        }
    }

    for (i = 0; i < n; i++) {
        py_list = PyList_GetItem(py_X, i); /* get the row */
        /* save element in matrix */
        for (j = 0; j < d; j++) {
            py_tmp = PyList_GetItem(py_list, j);
            X[i][j] = PyFloat_AsDouble(py_tmp);
        }
    }
    /* get A from c code, and then n */
    A= symg(X,n,d);
    
    /*freeing X*/
    for(i=0;i<n;i++){
        free(X[i]);
    }
    free(X);
    
    py_A = PyList_New(n); /* create a py_list */
    if (py_A == NULL){
        printf("An Error Has Occurred\n");
        exit(1);
    }
    for (i = 0; i < n; i++)
    {
        py_list = PyList_New(n); /* create a row */
        if (py_list == NULL){
       	    printf("An Error Has Occurred\n");
            exit(1);
        }
        for (j = 0; j < n; j++)/*copying the row*/
        {
            PyList_SetItem(py_list, j, Py_BuildValue("d", A[i][j]));
        }
        PyList_SetItem(py_A, i, Py_BuildValue("O", py_list)); /* adding the row */
    }
    
    /*freeing A*/
    for(i=0;i<n;i++){
        free(A[i]);
    }
    free(A);
    return py_A;
}

static PyObject *ddg(PyObject *self, PyObject *args) {
    PyObject *py_X,*py_list,*py_D,*py_tmp;
    int i,j,n,d;
    double **X,**D;
    /* check if we managed to pass them well*/
    if (!PyArg_ParseTuple(args, "O", &py_X)){
        printf("An Error Has Occurred\n");
        exit(1);
    }
    if (!PyList_Check(py_X)){ /*check if its an instance of a subtype of list*/
        printf("An Error Has Occurred\n");
        exit(1);
    }
    /*getting n*/
    n = PyObject_Length(py_X);
    d = PyObject_Length(PyList_GetItem(py_X, 0));
    
    /*allocating spcae*/
    X = (double**)malloc(sizeof(double*)*n);
    if(X == NULL){
        printf("An Error Has Occurred\n");
        exit(1);
    }
    for (i = 0; i < n; i++)
    {
        X[i] = (double*)malloc(sizeof(double)*d);
        if(X[i]==NULL){
            printf("An Error Has Occurred\n");
            exit(1);
        }
    }

    for (i = 0; i < n; i++) {
        py_list = PyList_GetItem(py_X, i); /* get the row */
        /* save element in matrix */
        for (j = 0; j < d; j++) {
            py_tmp = PyList_GetItem(py_list, j);
            X[i][j] = PyFloat_AsDouble(py_tmp);
        }
    }

    D = ddgg(X,n,d);
    /*freeing X*/
    for(i=0;i<n;i++){
        free(X[i]);
    }
    free(X);
    py_D = PyList_New(n);
    for (i = 0; i < n; i++)
    {
        py_list = PyList_New(n); /* create a row */
        if (py_list == NULL){
       	    printf("An Error Has Occurred\n");
            exit(1);
        }
        for (j = 0; j < n; j++)/*copying the row*/
        {
            PyList_SetItem(py_list, j, Py_BuildValue("d", D[i][j]));
        }
        PyList_SetItem(py_D, i, Py_BuildValue("O", py_list)); /* adding the row */
    }
    /*freeing D*/
    for(i=0;i<n;i++){
        free(D[i]);
    }
    free(D);
    return py_D;
}

static PyObject *norm(PyObject *self, PyObject *args) {
    PyObject *py_X,*py_list,*py_W,*py_tmp;
    int i,j,n,d;
    double **X,**W;
    /* check if we managed to pass them well*/
    if (!PyArg_ParseTuple(args, "O", &py_X)){
        printf("An Error Has Occurred\n");
        exit(1);
    }
    if (!PyList_Check(py_X)){ /*check if its an instance of a subtype of list*/
        printf("An Error Has Occurred\n");
        exit(1);
    }
    /*getting n*/
    n = PyObject_Length(py_X);
    d = PyObject_Length(PyList_GetItem(py_X, 0));
    
    /*allocating spcae*/
    X = (double**)malloc(sizeof(double*)*n);
    if(X == NULL){
        printf("An Error Has Occurred\n");
        exit(1);
    }
    for (i = 0; i < n; i++)
    {
        X[i] = (double*)malloc(sizeof(double)*d);
        if(X[i]==NULL){
            printf("An Error Has Occurred\n");
            exit(1);
        }
    }

    for (i = 0; i < n; i++) {
        py_list = PyList_GetItem(py_X, i); /* get the row */
        /* save element in matrix */
        for (j = 0; j < d; j++) {
            py_tmp = PyList_GetItem(py_list, j);
            X[i][j] = PyFloat_AsDouble(py_tmp);
        }
    }

    W = normg(X,n,d);
    /*freeing X*/
    for(i=0;i<n;i++){
        free(X[i]);
    }
    free(X);
    py_W = PyList_New(n);
    for (i = 0; i < n; i++)
    {
        py_list = PyList_New(n); /* create a row */
        if (py_list == NULL){
       	    printf("An Error Has Occurred\n");
            exit(1);
        }
        for (j = 0; j < n; j++)/*copying the row*/
        {
            PyList_SetItem(py_list, j, Py_BuildValue("d", W[i][j]));
        }
        PyList_SetItem(py_W, i, Py_BuildValue("O", py_list)); /* adding the row */
    }
    /*freeing W*/
    for(i=0;i<n;i++){
        free(W[i]);
    }
    free(W);
    return py_W;
}

static PyObject *symnmf(PyObject *self, PyObject *args) {
    PyObject *py_W,*py_IH,*py_list,*py_NH,*py_tmp;
    int i,j,n,k;
    double **W,**IH,**NH;/*IH stands for init H,NH stands for new H*/
    /* check if we managed to pass them well*/
    if (!PyArg_ParseTuple(args, "OOi", &py_W,&py_IH,&k)){
        printf("An Error Has Occurred\n");
        exit(1);
    }
    if (!PyList_Check(py_W)||!PyList_Check(py_IH)){ /*check if its an instance of a subtype of list*/
        printf("An Error Has Occurred\n");
        exit(1);
    }
    /*getting n*/
    n = PyObject_Length(py_W);
    /*allocating space for W,IH*/
    W = (double**)malloc(sizeof(double*)*n);
    if(W== NULL){
        printf("An Error Has Occurred\n");
        exit(1);
    }
    for (i = 0; i < n; i++)
    {
        W[i] = (double*)malloc(sizeof(double)*n);
        if(W[i]== NULL){
            printf("An Error Has Occurred\n");
            exit(1);
        }
    }
    IH = (double**)malloc(sizeof(double*)*n);
    if(IH== NULL){
        printf("An Error Has Occurred\n");
        exit(1);
    }
    for (i = 0; i < n; i++)
    {
        IH[i] = (double*)malloc(sizeof(double)*k);
        if(IH[i]== NULL){
            printf("An Error Has Occurred\n");
            exit(1);
        }
    }

    for (i = 0; i < n; i++) {
        py_list = PyList_GetItem(py_W, i); /* get the row */
        /* save element in matrix */
        for (j = 0; j < n; j++) {
            py_tmp = PyList_GetItem(py_list, j);
            W[i][j] = PyFloat_AsDouble(py_tmp);
        }
    }
    for (i = 0; i < n; i++) {
        py_list = PyList_GetItem(py_IH, i); /* get the row */
        /* save element in matrix */
        for (j = 0; j < k; j++) {
            py_tmp = PyList_GetItem(py_list, j);
            IH[i][j] = PyFloat_AsDouble(py_tmp);
        }
    }

    NH = symnmfg(W,IH,n,k);
    /*freeing space of W,IH*/
    for(i=0;i<n;i++){
        free(W[i]);
    }
    free(W);
    for(i=0;i<n;i++){
        free(IH[i]);
    }
    free(IH);

    py_NH = PyList_New(n);
    for (i = 0; i < n; i++)
    {
        py_list = PyList_New(k); /* create a row */
        if (py_list == NULL){
       	    printf("An Error Has Occurred\n");
            exit(1);
        }
        for (j = 0; j < k; j++)/*copying the row*/
        {
            PyList_SetItem(py_list, j, Py_BuildValue("d", NH[i][j]));
        }
        PyList_SetItem(py_NH, i, Py_BuildValue("O", py_list)); /* adding the row */
    }

    /*freeing space of NH*/
    for(i=0;i<n;i++){
        free(NH[i]);
    }
    free(NH);
    return py_NH;
}

static PyMethodDef capiMethods[] = {
    {"sym",                   
      (PyCFunction) sym,
      METH_VARARGS,         
      PyDoc_STR("Calculates and outputs the similarity matrix as described in 1.1")},
    {"ddg",                   
      (PyCFunction) ddg,
      METH_VARARGS,         
      PyDoc_STR("Calculates and outputs the Diagonal Degree Matrix as described in 1.2")},
    {"norm",                   
      (PyCFunction) norm,
      METH_VARARGS,         
      PyDoc_STR("Calculates and outputs the normalized similarity matrix as described in 1.3")},
    {"symnmf",                   
      (PyCFunction) symnmf,
      METH_VARARGS,         
      PyDoc_STR("Performs the full symNMF as described in 1 and output H")},
    {NULL, NULL, 0, NULL}     
};

static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "mysymnmf",
    NULL,
    -1,  
    capiMethods
};

PyMODINIT_FUNC PyInit_mysymnmf(void)
{
    PyObject *m;
    m = PyModule_Create(&moduledef);
    if (!m) {
        return NULL;
    }
    return m;
}
