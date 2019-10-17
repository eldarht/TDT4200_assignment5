#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <omp.h>
#include <cblas.h>

//Threshold for testing validity of matrix matrix multiplication
#define ERROR_THRESHOLD 0.0001

//For measuring wall time using omp_get_wtime()
static double start;
static double end;

//Serial version. Do not change this!
void serial_mxm(const double *A, const double *B, double *C, int m, int n, int k)
{
  start = omp_get_wtime();

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      C[i*n + j] = 0;
      for (int l = 0; l < k; l++) {
        C[i*n + j] += A[i*k + l] * B[l*n + j];
      }
    }
  }

  end = omp_get_wtime();
}

void omp_mxm(double *A, double *B, double *C, int m, int n, int k)
{
  start = omp_get_wtime();

  int i, j, l;

  #pragma omp parallel for default(shared) private(i, j, l) 
  for (int i = 0; i < m; i++) {
    
    for (int j = 0; j < n; j++) {
      C[i*n + j] = 0;
      for (int l = 0; l < k; l++) {
        C[i*n + j] += A[i*k + l] * B[l*n + j];
      }
    }
  }

  end = omp_get_wtime();
}

void blas_mxm(double *A, double *B, double *C, int m, int n, int k)
{
  start = omp_get_wtime();

  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
    m, n, k, 1.0, A, k, B, n, 1.0, C, n);

  end = omp_get_wtime();
}

int main(const unsigned int argc, char **argv)
{
  if (argc <= optind) {
    printf("Please provide version:\n");
    printf("\ts(serial),\n");
    printf("\to(penmp) or\n");
    printf("\tb(las)\n");
    return 0;
  }
  char input = argv[optind][0];
  optind++;
  //Simple assumptions that any additional arguments means we want to test the results
  bool test = !(argc <= optind);

  int m = 2000;
  int n = 1000;
  int k = 200;

  double *A = (double *)malloc( m*k*sizeof( double ));
  double *B = (double *)malloc( k*n*sizeof( double ));
  double *C = (double *)malloc( m*n*sizeof( double ));

  //Intializing matrix data
  for (int i = 0; i < (m*k); i++) {
    A[i] = (double)(i+1);
  }

  for (int i = 0; i < (k*n); i++) {
    B[i] = (double)(-i-1);
  }

  for (int i = 0; i < (m*n); i++) {
    C[i] = 0.0;
  }

  switch (input) {
    case 's':
      serial_mxm(A, B, C, m, n, k);
      break;
    case 'o':
      omp_mxm(A, B, C, m, n, k);
      break;
    case 'b':
      blas_mxm(A, B, C, m, n, k);
      break;
    default:
      printf("Please provide version:\n");
      printf("\ts(serial),\n");
      printf("\to(penmp) or\n");
      printf("\tb(las)\n");
      return 0;
  }

  printf("\nTop left of A:\n");
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
    printf("%8.2f\t", A[i*k + j]);
    }
    printf("\n");
  }

  printf("\nTop left of B:\n");
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
    printf("%8.2f\t", B[i*n + j]);
    }
    printf("\n");
  }

  printf("\nTop left of C:\n");
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      printf("%8.2f\t", C[i*n + j]);
    }
    printf("\n");
  }

  if (test) {
    double *C2 = (double *)malloc( m*n*sizeof( double ));
    serial_mxm(A, B, C2, m, n, k);
    bool correct = true;
    for (int i = 0; i < (m*n); i++) {
      if (abs(C[i] - C2[i]) > ERROR_THRESHOLD) {
        correct = false;
      }
    }
    if (correct) {
      printf("\nMatrix multiplication succeeded\n");
    } else {
      printf("\nMatrix multiplication failed\n");
      printf("Top left of correct C:\n");
      for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
          printf("%8.2f\t", C2[i*n + j]);
        }
        printf("\n");
      }
    }
  }

  printf("\nVersion: %c, time: %.4f\n", input, end-start);

  return 0;
}
