import sys
import mysymnmf as ms
import numpy as np
import pandas as pd
import math

np.random.seed(0)
#get the data
filename= sys.argv[3]
X = pd.read_csv(filename, header=None)
X = X.values.tolist()#converting to list
goal = sys.argv[2]
if(goal == "symnmf"):#in case of goal == "symnmf"
    W = ms.norm(X)#calculating norm
    k = int(sys.argv[1])
    m = np.average(np.array(W))#calculating W avg
    H = []
    for i in range(len(X)):
        H.append([])
        for j in range(k):
            H[i].append(2 * np.sqrt(m / k) * np.random.uniform())
    M = ms.symnmf(W,H,k)
if(goal=="sym"):#in case of goal == "sym"
    M = ms.sym(X)
if(goal=="ddg"):#in case of goal == "ddg"
    M = ms.ddg(X)
if(goal=="norm"):#in case of goal == "norm"
    M = ms.norm(X)
for i in range(len(M)):#printing the final matrix
        for j in range(len(M[i])-1):
            print('{:.4f}'.format(M[i][j]),end=",")
        print('{:.4f}'.format(M[i][len(M[i])-1]))