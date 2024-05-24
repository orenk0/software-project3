import sys
import mysymnmf as ms
import numpy as np
import pandas as pd
import math
from sklearn.metrics import silhouette_score
import kmeans as km

np.random.seed(0)
#get the data
k = int(sys.argv[1])
filename= sys.argv[2]
X = pd.read_csv(filename, header=None)
#get a legal random value
def lran(x,m,k):
    return np.random.uniform(0,2*math.sqrt(m/k))
rfunc = np.vectorize(lran)
X = X.values.tolist()
W = ms.norm(X)
m = np.average(np.array(W))
H = np.zeros((len(W),k))
H = rfunc(H,m,k)
H = H.tolist()
M = ms.symnmf(W,H,k)

clusters_nmf = np.argmax(M, axis=1)
#print(clusters)

DB = km.get_data_points(filename)
mu = km.k_means(k,300,DB)
mu = np.array([[float(val) for val in row] for row in mu])

clusters_km = np.asarray([np.argmin(np.linalg.norm(mu - X[i], axis=1)) for i in range(len(X))])


print("nmf:",silhouette_score(X,clusters_nmf))
print("kmeans:",silhouette_score(X,clusters_km))


    

