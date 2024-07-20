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
X = X.values.tolist()
W = ms.norm(X)
m = np.average(np.array(W))
H = []
for i in range(len(X)):
    H.append([])
    for j in range(k):
        H[i].append(2 * np.sqrt(m / k) * np.random.uniform())
M = ms.symnmf(W,H,k)

clusters_nmf = np.argmax(M, axis=1)

DB = km.get_data_points(filename)
mu = km.k_means(k,300,DB)
mu = np.array([[float(val) for val in row] for row in mu])

clusters_km = np.asarray([np.argmin(np.linalg.norm(mu - X[i], axis=1)) for i in range(len(X))])

#print(clusters)
print("nmf:",'{:.4f}'.format(silhouette_score(X,clusters_nmf)))
print("kmeans:",'{:.4f}'.format(silhouette_score(X,clusters_km)))