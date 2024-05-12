import sys
#from mpl_toolkits import Axes3D
#gets the data in a matrix
def get_data_points2(file_name):
    f = open(file_name,"r")
    DB = []
    for x in f:
        y = x.split(", ")
        y= (int(y[i]) for i in range(len(y)))
        DB.append(tuple(y))
    f.close()
    return DB
#second gets the data in a matrix
def get_data_points(file_name):
    with open(file_name, 'r') as f:
        data = f.read()
    rows = data.strip().split('\n')
    rows = [list(map(float, row.split(','))) for row in rows]
    tuples_data = [tuple(row) for row in rows]
    return tuples_data
#euclidean distance
def vecadd(t1, t2, alpha=1):#tuple vector addition
    assert len(t1)==len(t2),"vectors must be of the same length"
    return tuple([t1[i]+alpha*t2[i] for i in range(len(t1))])
def dist(p, q):
    sum = 0
    for i in range(len(p)):
        sum+= (p[i]-q[i])**2
    return sum**0.5
def argmin(l):
    return [i for i in range(len(l)) if l[i]==min(l)]
def delta(mu, epsilon):
    for i in range(len(mu)):
        if(dist(mu[i],mu[i-1])>=epsilon):
            return False
    return True
def k_means(K, iter=200, input_data=""):
    N = len(input_data)
    x = input_data #rename
    mu = [x[k] for k in range(K)]
    epsilon = 0.001
    iteration_number = 0
    assignment = []
    while(delta(mu, epsilon) or iteration_number<iter):
        assignment = [] #assign to every x the closest cluster
        for x_i in x:
            assignment.append(argmin([dist(x_i,mu[k]) for k in range(K)])[0])#the index of the closest cluster
        for k in range(K):
            mu[k] = vecadd(mu[k],mu[k],-1)
            num = 0#cluster size
            for i in range(N):
                if(assignment[i] == k):#x_i belongs to cluster k
                    num = num +1
                    mu[k] = vecadd(mu[k], input_data[i])
            mu[k] = vecadd(vecadd(mu[k],mu[k],-1),mu[k],1/num)
        iteration_number = iteration_number + 1
    for i in range(len(mu)):
        mu[i] = ['{:.4f}'.format(mu[i][j]) for j in range(len(mu[i]))]
    return mu
    
def main():
        assert((len(sys.argv)==6)or(len(sys.argv)==5)), "invalid number of arguments! Expected 4 or 5 but "+str(len(sys.argv))+" were given."
        if(len(sys.argv)==6):
            K = float(sys.argv[1])
            n = float(sys.argv[2])
            d = float(sys.argv[3])
            iter = float(sys.argv[4])
            file_name = sys.argv[5]
            assert((n>1)and(n == int(n))), "invalid number of points!"
            assert((n>K>1)and(K==int(K))), "invalid number of clusters!"
            assert(d==int(d)), "Invalid dimension of point!"
            assert((1000>iter>1)and(iter==int(iter))), "Invalid maximum iteration!"
            assert((file_name[len(file_name)-1]=="t")
                   and(file_name[len(file_name)-2]=="x")
                   and(file_name[len(file_name)-3]=="t")
                   and(file_name[len(file_name)-4]==".")),"NA"
            K = int(K)
            n= int(n)
            d = int(d)
            iter = int(iter)
            DB = get_data_points(file_name)
            for row in k_means(K,iter,DB):
                print(','.join(map(str, row)))
        if(len(sys.argv)==5):
            K = float(sys.argv[1])
            n = float(sys.argv[2])
            d = float(sys.argv[3])
            file_name = sys.argv[4]
            assert((n>1)and(n == int(n))), "invalid number of points!"
            assert((n>K>1)and(K==int(K))), "invalid number of clusters!"
            assert(d==int(d)), "Invalid dimension of point!"
            assert((file_name[len(file_name)-1]=="t")
                   and(file_name[len(file_name)-2]=="x")
                   and(file_name[len(file_name)-3]=="t")
                   and(file_name[len(file_name)-4]==".")),"NA"
            K = int(K)
            n= int(n)
            d = int(d)
            DB = get_data_points(file_name)
            for row in k_means(K,200,DB):
                print(','.join(map(str, row)))
if __name__== "__main__":
    main()
