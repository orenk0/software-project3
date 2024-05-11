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
    printf("%d\n",d);
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
            printf("%lf\n", dist);
            if (i != j) {
                similarity_matrix[i][j] = exp(-dist / 2);
            } else {
                similarity_matrix[i][j] = 0.0;
            }
        }
    }
    print_matrix(similarity_matrix, n, n);
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
    print_matrix(degree_matrix, n, n);
    return degree_matrix;
}


// Function to calculate normalized similarity matrix
double **norm(FILE *file) {
    double **similarity_matrix = sym(file);
    double **degree_matrix = ddg(file);
    double **normalized_similarity_matrix = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        normalized_similarity_matrix[i] = (double *)malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) {
            normalized_similarity_matrix[i][j] = similarity_matrix[i][j] / (sqrt(degree_matrix[i][i]) * sqrt(degree_matrix[j][j]));//equivalent to the product.
        }
    }
    print_matrix(normalized_similarity_matrix, n, n);
    return normalized_similarity_matrix;
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

    if (strcmp(goal, "sym") == 0) {
        sym(file);
    } else if (strcmp(goal, "ddg") == 0) {
        ddg(file);
    } else if (strcmp(goal, "norm") == 0) {
        norm(file);
    } else {
        printf("An Error Has Occurred\n");//printf("Error: Invalid goal.\n");
        return 1;
    }

    fclose(file);
    return 0;
}
