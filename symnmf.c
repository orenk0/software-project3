#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int n, d;
// Function to calculate squared Euclidean distance
double squared_euclidean_distance(double *x1, double *x2, int d) {
    double distance = 0.0;
    for (int i = 0; i < d; i++) {
        distance += pow(x1[i] - x2[i], 2);
    }
    return distance;
}

// Function to print a general matrix
void print_matrix(double **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%.4f", matrix[i][j]);
            if (j != cols - 1) {
                printf(",");
            }
        }
        printf("\n");
    }
}

// Function to scan points from file and store them in a 2D array
double **scan_points(FILE *file) {
    n = 0;
    d = 0;
    double temp;
    while (fscanf(file, "%lf", &temp) == 1) {
        d += 1;
        if (fgetc(file) == '\n') {
            n += 1;
        }
    }
    
    d = d / n;
    rewind(file);
    double **points = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        points[i] = (double *)malloc(d * sizeof(double));
        for (int j = 0; j < d; j++) {
            fscanf(file, "%lf", &points[i][j]);
        }
    }
    return points;
}

// Function to calculate similarity matrix
double **sym(FILE *file) {
    double **points = scan_points(file);
    double **similarity_matrix = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        similarity_matrix[i] = (double *)malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) {
            double dist = squared_euclidean_distance(points[i], points[j], d);
            if (i != j) {
                similarity_matrix[i][j] = exp(-dist / 2);
            } else {
                similarity_matrix[i][j] = 0.0;
            }
        }
    }
    return similarity_matrix;
}




// Function to calculate Diagonal Degree Matrix
double **ddg(FILE *file) {
    double **degree_matrix = (double **)malloc(n * sizeof(double *));
    double **similarity_matrix = sym(file);
    for (int i = 0; i < n; i++) {
        degree_matrix[i] = (double *)malloc(n * sizeof(double));
        double degree = 0.0;
        for (int j = 0; j < n; j++) {
            degree += similarity_matrix[i][j];
        }
        degree_matrix[i][i] = degree;
    }
    return degree_matrix;
}


// Function to calculate normalized similarity matrix
double **norm(FILE *file) {
    double **similarity_matrix = sym(file);
    rewind(file);
    double **degree_matrix = ddg(file);
    double **normalized_similarity_matrix = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        normalized_similarity_matrix[i] = (double *)malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) {
            normalized_similarity_matrix[i][j] = similarity_matrix[i][j] / (sqrt(degree_matrix[i][i]) * sqrt(degree_matrix[j][j]));//equivalent to the product.
        }
    }
    return normalized_similarity_matrix;
}


double calculate_average(double **W) {
    double sum = 0.0;
    int total_elements = n*n;

    // Traverse through each element of the 2D array
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            sum += W[i][j];
        }
    }

    // Calculate the average
    double average = sum / total_elements;

    return average;
}

double **symnmf(FILE *file, int k){
    //1.4.1
    double **W = norm(file);
    double m = calculate_average(W);
    double max_value = 2.0 * sqrt(m / k);
    double **H = (double **)malloc(n * sizeof(double *));
    double **H_old = (double **)malloc(n * sizeof(double *));
    double **H_tmp = (double **)malloc(n * sizeof(double *));
    // Generate and print a random real number between 0 and max_value:
    for(int i = 0 ; i < n ; i++){ 
        H[i] = (double *)malloc(k * sizeof(double));
        H_old[i] = (double *)malloc(k * sizeof(double));
        H_tmp[i] = (double *)malloc(k * sizeof(double));
        for(int j = 0 ; j < k ; j++){
            H_old[i][j] = ((double)rand() / RAND_MAX) * max_value;
            H_tmp[i][j] = ((double)rand() / RAND_MAX) * max_value;
            H[i][j] = ((double)rand() / RAND_MAX) * max_value;
            //printf("%lf",H[i][j]);
        }
    }
    //1.4.2
    double beta = 0.5;
    int iter = 0;
    int max_iter = 300;
    double eps = 0.0001;
    double criteria = 0;
    do{
        //print_matrix(H,n,k);
        //printf("\n\n");
        //calc new H
        for(int i = 0 ; i < n ; i++){
            for(int j = 0 ; j < k ; j++){
                //calculate the ith jth entrance of the numerator:
                double x1 = 0;
                for(int a = 0 ; a < n ; a++){
                    x1+=W[i][a]*H[a][j];
                }
                //printf("x1 = %lf\n",x1);
                //print_matrix(H,n,k);
                //calculate the ith jth entrance of the denominator:
                double x2 = 0;
                for(int a = 0 ; a < n ; a++){
                    double x3 = 0;
                    for(int b = 0 ; b < k ; b++){
                        x3+=H[i][b]*H[a][b];
                    }
                    x2+=x3*H[a][j];
                }

                H_old[i][j] = H[i][j];
                
                H_tmp[i][j] = H[i][j] * (1-beta+beta*(x1/x2));
                //printf("%lf", H[0][0]);
            }
        }
        for(int i = 0 ; i < n ; i++){
            for(int j = 0 ; j < k ; j++){
                H[i][j] = H_tmp[i][j];
            }
        }
        iter++;
        //check the change isnt small
        criteria = 0;
        for(int i = 0 ; i < n ; i++){
            for(int j = 0 ; j < k ; j++){
                criteria += (H[i][j]-H_old[i][j])*(H[i][j]-H_old[i][j]);
                //printf("%lf\n",H[i][j]-H_old[i][j]);
            }
        }
    }while(iter < max_iter && criteria > eps);
    //printf("%lf", H[0][0]);
    return H;
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("An Error Has Occurred\n");//printf("Usage: ./symnmf <goal> <file_name>\n");
        return 1;
    }
    char *goal = argv[1];
    char *file_name = argv[2];

    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("An Error Has Occurred\n");//Error: Unable to open file.
        return 1;
    }

    double **mat;

    if (strcmp(goal, "sym") == 0) {
        mat = sym(file);
    } else if (strcmp(goal, "ddg") == 0) {
        mat = ddg(file);
    } else if (strcmp(goal, "norm") == 0) {
        mat = norm(file);
    } else {
        printf("An Error Has Occurred\n");//printf("Error: Invalid goal.\n");
        return 1;
    }
    print_matrix(mat, n, n);
    rewind(file);
    printf("\n\n");
    print_matrix(symnmf(file,2),n,2);

    fclose(file);
    return 0;
}
