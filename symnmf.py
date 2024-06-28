import sys
import mysymnmf as ms
import numpy as np
import pandas as pd
import math

np.random.seed(0)
#get the data
filename= sys.argv[3]
X = pd.read_csv(filename, header=None)
#get a legal random value
def lran(x,m,k):
    return np.random.uniform(0,2*math.sqrt(m/k))
rfunc = np.vectorize(lran)
X = X.values.tolist()#converting to list
goal = sys.argv[2]
if(goal == "symnmf"):#in case of goal == "symnmf"
    W = ms.norm(X)#calculating norm
    k = int(sys.argv[1])
    m = np.average(np.array(W))#calculating W avg
    H = np.zeros((len(W),k))#building the frame of H
    H = rfunc(H,m,k)#init each element of H
    H = H.tolist()#converting to list
    M = ms.symnmf(W,H,k)
if(goal=="sym"):#in case of goal == "sym"
    M = ms.sym(X)
if(goal=="ddg"):#in case of goal == "ddg"
    M = ms.sym(X)
    for i in range(len(M)):
        for j in range(len(M)):
             x = M[i][j]
             M[i][j] = M[i][j]-x
             M[i][i] += x
    M = ms.ddg(X)
if(goal=="norm"):#in case of goal == "norm"
    M = ms.norm(X)
for i in range(len(M)):#printing the final matrix
        for j in range(len(M[i])-1):
            print('{:.4f}'.format(M[i][j]),end=",")
        print('{:.4f}'.format(M[i][len(M[i])-1]))