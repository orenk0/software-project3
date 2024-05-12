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
X = X.values.tolist()
goal = sys.argv[2]
if(goal == "symnmf"):
    W = ms.norm(X)
    k = int(sys.argv[1])
    m = np.average(np.array(W))
    H = np.zeros((len(W),k))
    H = rfunc(H,m,k)
    H = H.tolist()
    M = ms.symnmf(W,H,k)
if(goal=="sym"):
    M = ms.sym(X)
if(goal=="ddg"):
    M = ms.ddg(X)
if(goal=="norm"):
    M = ms.norm(X)
for i in range(len(M)):
        for j in range(len(M[i])-1):
            print('{:.4f}'.format(M[i][j]),end=",")
        print('{:.4f}'.format(M[i][len(M[i])-1]))