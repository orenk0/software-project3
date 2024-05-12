#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Global variables for dimensions of the input: */
int n, d;
/* Function to calculate the squared Euclidean distance between two points */
double squared_euclidean_distance(double *x1, double *x2, int d) {
    /* Defining variables for future use: */
    double distance = 0.0; /* Initialize distance variable */
    int i;
    for (i = 0; i < d; i++) { /* Loop through dimensions */
        distance += pow(x1[i] - x2[i], 2); /* Add squared difference of each dimension */
    }
    return distance; /* Return the calculated distance */
}

/* Function to print a general matrix */
void print_matrix(double **matrix, int rows, int cols) {
    /* Defining variables for future use: */
    int i;
    int k;
    for (i = 0; i < rows; i++) { /* Loop through rows */
        for (j = 0; j < cols; j++) { /* Loop through columns */
            printf("%.4f", matrix[i][j]); /* Print each element */
            if (j != cols - 1) { /* If not the last column, print comma */
                printf(",");
            }
        }
        printf("\n"); /* Move to the next line after each row */
    }
}



/* Function to scan points from file and store them in a 2D array */
double **scan_points(FILE *file) {
    n = 0; /* Initialize number of points */
    d = 0; /* Initialize number of dimensions */
    /* Defining variables for future use: */
    int i; /* Declare loop variable */
    int j; /* Declare loop variable */
    double temp; /* Temporary variable to store scanned values */
    double **points;
    
    /* Count dimensions and points */
    while (fscanf(file, "%lf", &temp) == 1) {
        d += 1; /* Increment dimension count */
        if (fgetc(file) == '\n') {
            n += 1; /* Increment point count */
        }
    }
    d = d / n; /* Calculate actual dimension */
    
    /* Allocate memory for points array */
    points = (double **)malloc(n * sizeof(double *));
    for (i = 0; i < n; i++) {
        points[i] = (double *)malloc(d * sizeof(double));
    }
    
    /* Reset file pointer */
    rewind(file);
    i = 0;
    j = 0;
    /* Read points from file into array */
    while (fscanf(file, "%lf", &temp) == 1) {
        fgetc(file); /* Skip comma */
        points[i][j] = temp; /* Assign value to array */
        j++; /* Move to the next dimension */
        if(j == d) { /* If all dimensions filled */
            i++; /* Move to the next point */
            j = 0; /* Reset dimension index */
        }
    }
    return points; /* Return the array of points */
}


/* Function to calculate similarity matrix */
double **sym(FILE *file) {
    /* Defining variables for future use: */
    int i;
    int j;
    double **points;
    double **similarity_matrix;
    double dist;
    /* Obtain points from file */
    points = scan_points(file);
    /* Allocate memory for similarity matrix */
    similarity_matrix = (double **)malloc(n * sizeof(double *));
    for (i = 0; i < n; i++) {
        similarity_matrix[i] = (double *)malloc(n * sizeof(double));
        for (j = 0; j < n; j++) {
            /* Calculate squared Euclidean distance */
            dist = squared_euclidean_distance(points[i], points[j], d);
            if (i != j) { /* If points are different */
                similarity_matrix[i][j] = exp(-dist / 2); /* Calculate similarity */
            } else { /* If points are the same */
                similarity_matrix[i][j] = 0.0; /* Set similarity to 0 */
            }
        }
    }
    return similarity_matrix; /* Return the similarity matrix */
}





/* Function to calculate Diagonal Degree Matrix */
double **ddg(FILE *file) {
    /* Defining variables for future use: */
    int i;
    int j;
    double degree;
    double **degree_matrix;
    double **similarity_matrix;
    /* Allocate memory for degree matrix */
    degree_matrix = (double **)malloc(n * sizeof(double *));
    /* Obtain similarity matrix */
    similarity_matrix = sym(file);
    for (i = 0; i < n; i++) {
        degree_matrix[i] = (double *)malloc(n * sizeof(double));
        degree = 0.0; /* Initialize degree for each node */
        for (j = 0; j < n; j++) {
            degree += similarity_matrix[i][j]; /* Accumulate similarity */
        }
        degree_matrix[i][i] = degree; /* Assign degree to diagonal element */
    }
    return degree_matrix; /* Return the degree matrix */
}



/* Function to calculate normalized similarity matrix */
double **norm(FILE *file) {
    /* Defining variables for future use: */
    double **similarity_matrix;
    double **degree_matrix;
    double **normalized_similarity_matrix;
    int i;
    int j;
    /* Obtain similarity matrix */
    similarity_matrix = sym(file);
    rewind(file); /* Reset file pointer */
    /* Obtain degree matrix */
    degree_matrix = ddg(file);
    /* Allocate memory for normalized similarity matrix */
    normalized_similarity_matrix = (double **)malloc(n * sizeof(double *));
    for (i = 0; i < n; i++) {
        normalized_similarity_matrix[i] = (double *)malloc(n * sizeof(double));
        for (j = 0; j < n; j++) {
            /* Calculate normalized similarity */
            normalized_similarity_matrix[i][j] = similarity_matrix[i][j] / (sqrt(degree_matrix[i][i]) * sqrt(degree_matrix[j][j]));
        }
    }
    return normalized_similarity_matrix; /* Return the normalized similarity matrix */
}



/* Function to calculate the average of elements in a 2D array */
double calculate_average(double **W) {
    double sum = 0.0; /* Initialize sum */
    int total_elements = n * n; /* Calculate total elements */

    /* Traverse through each element of the 2D array */
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            sum += W[i][j]; /* Add element to sum */
        }
    }

    /* Calculate the average */
    double average = sum / total_elements;

    return average; /* Return the average */
}


/* Function to perform symmetric NMF */
double **symnmf(FILE *file, int k){
    /* 1.4.1: Initialization */
    double **W = norm(file); /* Calculate normalized similarity matrix */
    double m = calculate_average(W); /* Calculate average value of W */
    double max_value = 2.0 * sqrt(m / k); /* Calculate maximum value for initialization */
    double **H = (double **)malloc(n * sizeof(double *));
    double **H_old = (double **)malloc(n * sizeof(double *));
    double **H_tmp = (double **)malloc(n * sizeof(double *));
    /* Defining variables for future use: */
    int i;
    int j;
    double beta;
    int iter;
    int max_iter;
    double eps;
    double criteria;
    double x1;
    double x2;
    double x3;
    int a;
    int b;
    
    /* Initialize H with random values */
    for(int i = 0 ; i < n ; i++){ 
        H[i] = (double *)malloc(k * sizeof(double));
        H_old[i] = (double *)malloc(k * sizeof(double));
        H_tmp[i] = (double *)malloc(k * sizeof(double));
        for(int j = 0 ; j < k ; j++){
            H_old[i][j] = ((double)rand() / RAND_MAX) * max_value;
            H_tmp[i][j] = ((double)rand() / RAND_MAX) * max_value;
            H[i][j] = ((double)rand() / RAND_MAX) * max_value;
        }
    }
    
    /* 1.4.2: Update H iteratively */
    beta = 0.5;
    iter = 0;
    max_iter = 300;
    eps = 0.0001;
    criteria = 0;
    do{
        /* Update H */
        for(i = 0 ; i < n ; i++){
            for(j = 0 ; j < k ; j++){
                /* Calculate numerator */
                x1 = 0;
                for(a = 0 ; a < n ; a++){
                    x1 += W[i][a] * H[a][j];
                }
                /* Calculate denominator */
                x2 = 0;
                for(a = 0 ; a < n ; a++){
                    x3 = 0;
                    for(b = 0 ; b < k ; b++){
                        x3 += H[i][b] * H[a][b];
                    }
                    x2 += x3 * H[a][j];
                }
                /* Update H */
                H_old[i][j] = H[i][j];
                H_tmp[i][j] = H[i][j] * (1 - beta + beta * (x1 / x2));
            }
        }
        /* Copy H_tmp to H */
        for(i = 0 ; i < n ; i++){
            for(j = 0 ; j < k ; j++){
                H[i][j] = H_tmp[i][j];
            }
        }
        iter++;
        /* Check convergence */
        criteria = 0;
        for(i = 0 ; i < n ; i++){
            for(j = 0 ; j < k ; j++){
                criteria += (H[i][j] - H_old[i][j]) * (H[i][j] - H_old[i][j]);
            }
        }
    }while(iter < max_iter && criteria > eps);
    
    return H; /* Return H */
}



int main(int argc, char *argv[]) {
    char *goal;
    char *file_name;
    FILE *file;
    double **mat; /* Declare matrix variable */
    if (argc != 3) {
        printf("An Error Has Occurred\n"); /* Print error message */
        return 1; /* Exit with error code */
    }
    goal = argv[1]; /* Extract goal from command line arguments */
    file_name = argv[2]; /* Extract file name from command line arguments */

    file = fopen(file_name, "r"); /* Open file for reading */
    if (file == NULL) {
        printf("An Error Has Occurred\n"); /* Print error message */
        return 1; /* Exit with error code */
    }


    if (strcmp(goal, "sym") == 0) {
        mat = sym(file); /* Calculate similarity matrix */
    } else if (strcmp(goal, "ddg") == 0) {
        mat = ddg(file); /* Calculate degree matrix */
    } else if (strcmp(goal, "norm") == 0) {
        mat = norm(file); /* Calculate normalized similarity matrix */
    } else {
        printf("An Error Has Occurred\n"); /* Print error message */
        return 1; /* Exit with error code */
    }
    print_matrix(mat, n, n); /* Print the matrix */
    rewind(file); /* Reset file pointer */
    print_matrix(symnmf(file,2),n,2); /* Print symmetric NMF result */

    fclose(file); /* Close file */
    return 0; /* Exit with success */
}

