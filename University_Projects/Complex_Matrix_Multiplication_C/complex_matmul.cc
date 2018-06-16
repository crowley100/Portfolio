/* Test and timing harness program for developing a dense matrix
   multiplication routine for the CS3014 module */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
#include <omp.h>
#include <x86intrin.h>
  

/* This program was written as an entry for a class competition.
   The criteria for the compeition was the fastest run time to compute
   the correct answer in large scale complex matrix multiplication.

   It therefore emphasises efficiency over readability.
   This solution earned first prize in all matrix size categories. */

#define DEBUGGING(_x)

struct complex {
  float real;
  float imag;
};

struct result {
  __m128 real_sum_vec0;
  __m128 real_sum_vec1;
  __m128 real_sum_vec2;
  __m128 real_sum_vec3;

  __m128 imag_sum_vec0 ;
  __m128 imag_sum_vec1 ;
  __m128 imag_sum_vec2 ;
  __m128 imag_sum_vec3 ;
};

/* write matrix to stdout */
void write_out(struct complex ** a, int dim1, int dim2)
{
  int i, j;

  for ( i = 0; i < dim1; i++ ) {
    for ( j = 0; j < dim2 - 1; j++ ) {
      printf("%.3f + %.3fi ", a[i][j].real, a[i][j].imag);
    }
    printf("%.3f + %.3fi\n", a[i][dim2-1].real, a[i][dim2-1].imag);
  }
}


/* create new empty matrix */
struct complex ** new_empty_matrix(int dim1, int dim2)
{
  struct complex ** result = malloc(sizeof(struct complex*) * dim1);
  struct complex * new_matrix = malloc(sizeof(struct complex) * dim1 * dim2);
  int i;

  for ( i = 0; i < dim1; i++ ) {
    result[i] = &(new_matrix[i*dim2]);
  }

  return result;
}

void free_matrix(struct complex ** matrix) {
  free (matrix[0]); /* free the contents */
  free (matrix); /* free the header */
}

/* take a copy of the matrix and return in a newly allocated matrix */
struct complex ** copy_matrix(struct complex ** source_matrix, int dim1, int dim2)
{
  int i, j;
  struct complex ** result = new_empty_matrix(dim1, dim2);

  for ( i = 0; i < dim1; i++ ) {
    for ( j = 0; j < dim2; j++ ) {
      result[i][j] = source_matrix[i][j];
    }
  }

  return result;
}

/* create a matrix and fill it with random numbers */
struct complex ** gen_random_matrix(int dim1, int dim2)
{
  const int random_range = 512; // constant power of 2
  struct complex ** result;
  int i, j;
  struct timeval seedtime;
  int seed;

  result = new_empty_matrix(dim1, dim2);

  /* use the microsecond part of the current time as a pseudorandom seed */
  gettimeofday(&seedtime, NULL);
  seed = seedtime.tv_usec;
  srandom(seed);

  /* fill the matrix with random numbers */
  for ( i = 0; i < dim1; i++ ) {
    for ( j = 0; j < dim2; j++ ) {
      /* evenly generate values in the range [0, random_range-1)*/
      result[i][j].real =(float)(random() % random_range);
      result[i][j].imag = (float)(random() % random_range);

      /* at no loss of precision, negate the values sometimes */
      /* so the range is now (-(random_range-1), random_range-1)*/
      if (random() & 1) result[i][j].real = -result[i][j].real;
      if (random() & 1) result[i][j].imag = -result[i][j].imag;
    }
  }

  return result;
}

/* check the sum of absolute differences is within reasonable epsilon */
/* returns number of differing values */
void check_result(struct complex ** result, struct complex ** control, int dim1, int dim2)
{
  int i, j;
  double sum_abs_diff = 0.0;
  const double EPSILON = 0.0625;

  for ( i = 0; i < dim1; i++ ) {
    for ( j = 0; j < dim2; j++ ) {
      double diff;
      diff = abs(control[i][j].real - result[i][j].real);
      sum_abs_diff = sum_abs_diff + diff;

      diff = abs(control[i][j].imag - result[i][j].imag);
      sum_abs_diff = sum_abs_diff + diff;
    }
  }

  if ( sum_abs_diff > EPSILON ) {
    fprintf(stderr, "WARNING: sum of absolute differences (%f) > EPSILON (%f)\n",
      sum_abs_diff, EPSILON);
  }
}

/* multiply matrix A times matrix B and put result in matrix C */
void matmul(struct complex ** A, struct complex ** B, struct complex ** C, int a_dim1, int a_dim2, int b_dim2)
{
  int i, j, k;

  for ( i = 0; i < a_dim1; i++ ) {
    for( j = 0; j < b_dim2; j++ ) {
      struct complex sum;
      sum.real = 0.0;
      sum.imag = 0.0;
      for ( k = 0; k < a_dim2; k++ ) {
        // the following code does: sum += A[i][k] * B[k][j];
        struct complex product;
        product.real = A[i][k].real * B[k][j].real - A[i][k].imag * B[k][j].imag;
        product.imag = A[i][k].real * B[k][j].imag + A[i][k].imag * B[k][j].real;
        sum.real += product.real;
        sum.imag += product.imag;  
      } 
      C[i][j] = sum;
    }
    
  }
}

// calculate transpose of complex matrix B
struct complex ** transpose(struct complex ** B,int b_rows,int b_cols){
  int i,j;
  struct complex ** result = new_empty_matrix( b_cols,b_rows); 
  for(i=0;i<b_rows;i++){
    for(j=0;j<b_cols;j++){
      result[j][i]=B[i][j];
    }
  }
  return result;
}

/* This code runs if a_rows or a_cols is odd.*/
void my_matmul_odd(int j, int i,struct complex ** A,struct complex ** B ,struct complex ** C ,int max,int a_cols){
  int k;
  struct complex sum0,sum1,sum2,sum3;
  sum0.real, sum1.real,sum2.real,sum3.real = 0.0;
  sum0.imag,sum1.imag,sum2.imag,sum3.imag = 0.0;
  __m128 real_sum_vec0 = _mm_xor_ps(real_sum_vec0, real_sum_vec0);
  __m128 real_sum_vec1 = real_sum_vec0;
  __m128 real_sum_vec2 = real_sum_vec0;
  __m128 real_sum_vec3 = real_sum_vec0;
        
  __m128 imag_sum_vec0 = real_sum_vec0;
  __m128 imag_sum_vec1 = real_sum_vec0;
  __m128 imag_sum_vec2 = real_sum_vec0;
  __m128 imag_sum_vec3 = real_sum_vec0;

   
  for ( k = 0; k < max; k+=2 ) {
    __m128 a_row0 = _mm_loadu_ps(&A[i][k].real);
          
    __m128 a_row1 = _mm_loadu_ps(&A[i+1][k].real);
    __m128 a_row2 = _mm_loadu_ps(&A[i+2][k].real);
    __m128 a_row3 = _mm_loadu_ps(&A[i+3][k].real);
    __m128 b_row = _mm_loadu_ps(&B[j][k].real);
          
    __m128 real_mul0 = _mm_mul_ps(a_row0,b_row);
    __m128 real_mul1 = _mm_mul_ps(a_row1,b_row);
    __m128 real_mul2 = _mm_mul_ps(a_row2,b_row);
    __m128 real_mul3 = _mm_mul_ps(a_row3,b_row);
          
    __m128 c = _mm_shuffle_ps(b_row,b_row, _MM_SHUFFLE(2,3,0,1));
          
    __m128 imag_mul0 = _mm_mul_ps(a_row0, c);
    __m128 imag_mul1 = _mm_mul_ps(a_row1, c);
    __m128 imag_mul2 = _mm_mul_ps(a_row2, c);
    __m128 imag_mul3 = _mm_mul_ps(a_row3, c);
          
    real_sum_vec0 = _mm_add_ps(real_sum_vec0,real_mul0);
    real_sum_vec1 = _mm_add_ps(real_sum_vec1,real_mul1);
    real_sum_vec2 = _mm_add_ps(real_sum_vec2,real_mul2);
    real_sum_vec3 = _mm_add_ps(real_sum_vec3,real_mul3);
          
    imag_sum_vec0 = _mm_add_ps(imag_sum_vec0,imag_mul0);
    imag_sum_vec1 = _mm_add_ps(imag_sum_vec1,imag_mul1);
    imag_sum_vec2 = _mm_add_ps(imag_sum_vec2,imag_mul2);
    imag_sum_vec3 = _mm_add_ps(imag_sum_vec3,imag_mul3);
    }

    // peel and compute last 'odd' column of matrix
    if  (max != a_cols) {
      // the following code does: sum += A[i][k] * B[k][j];
      struct complex product0,product1,product2,product3;
      product0.real = A[i][k].real * B[j][k].real - A[i][k].imag * B[j][k].imag;
      product0.imag = A[i][k].real * B[j][k].imag + A[i][k].imag * B[j][k].real;
      sum0.real = product0.real;
      sum0.imag = product0.imag;
          
      product1.real = A[i+1][k].real * B[j][k].real - A[i+1][k].imag * B[j][k].imag;
      product1.imag = A[i+1][k].real * B[j][k].imag + A[i+1][k].imag * B[j][k].real;
      sum1.real = product1.real;
      sum1.imag = product1.imag;

      product2.real = A[i+2][k].real * B[j][k].real - A[i+2][k].imag * B[j][k].imag;
      product2.imag = A[i+2][k].real * B[j][k].imag + A[i+2][k].imag * B[j][k].real;
      sum2.real = product2.real;
      sum2.imag = product2.imag;

      product3.real = A[i+3][k].real * B[j][k].real - A[i+3][k].imag * B[j][k].imag;
      product3.imag = A[i+3][k].real * B[j][k].imag + A[i+3][k].imag * B[j][k].real;
      sum3.real= product3.real;
      sum3.imag = product3.imag;
        
    } 
    __m128 real_comp = _mm_hsub_ps(real_sum_vec0,real_sum_vec1);
    __m128 real_comp1 =_mm_hsub_ps(real_sum_vec2,real_sum_vec3);
          
    __m128 imag_comp = _mm_hadd_ps(imag_sum_vec0,imag_sum_vec1); 
    __m128 imag_comp1 = _mm_hadd_ps(imag_sum_vec2,imag_sum_vec3); 

    __m128 real_adds = _mm_hadd_ps(real_comp,real_comp);
    __m128 imag_adds = _mm_hadd_ps(imag_comp,imag_comp);

    __m128 real_adds1 = _mm_hadd_ps(real_comp1,real_comp1);
    __m128 imag_adds1 = _mm_hadd_ps(imag_comp1,imag_comp1);
        
    sum0.real += real_adds[0];
    sum0.imag += imag_adds[0];

    sum1.real += real_adds[3];
    sum1.imag +=imag_adds[3];
            
    sum2.real += real_adds1[0];
    sum2.imag += imag_adds1[0];

    sum3.real += real_adds1[3];
    sum3.imag += imag_adds1[3];
      
    C[i][j] = sum0;
    C[i+1][j] = sum1;
    C[i+2][j] = sum2;
    C[i+3][j] = sum3;

 }

 /* optimized multiplication of matrix A times matrix B and put result in matrix C  */
void my_matmul_even(int j, int i,struct complex ** A,struct complex ** B ,struct complex ** C ,int max,int a_cols){
  int k;
  struct complex sum0,sum1,sum2,sum3;
  sum0.real, sum1.real,sum2.real,sum3.real = 0.0;
  sum0.imag,sum1.imag,sum2.imag,sum3.imag = 0.0;
  __m128 real_sum_vec0 = _mm_xor_ps(real_sum_vec0, real_sum_vec0); //initialising  vectors to zero
  __m128 real_sum_vec1 = real_sum_vec0;// this is to avoid using xor again
  __m128 real_sum_vec2 = real_sum_vec0;
  __m128 real_sum_vec3 = real_sum_vec0;
        
  __m128 imag_sum_vec0 = real_sum_vec0;
  __m128 imag_sum_vec1 = real_sum_vec0;
  __m128 imag_sum_vec2 = real_sum_vec0;
  __m128 imag_sum_vec3 = real_sum_vec0;

   // running up to greatest even row 
  for (k = 0; k < max; k+=2) {//  4 rows and 2 columns at a time
    __m128 a_row0 = _mm_load_ps(&A[i][k].real);
          
    __m128 a_row1 = _mm_load_ps(&A[i+1][k].real);
    __m128 a_row2 = _mm_load_ps(&A[i+2][k].real);
    __m128 a_row3 = _mm_load_ps(&A[i+3][k].real);
    __m128 b_row = _mm_load_ps(&B[j][k].real);
          
    __m128 real_mul0 = _mm_mul_ps(a_row0,b_row);// computing A[i][k].real * B[j][k].real and  A[i][k].imag * B[j][k].imag for the four rows
    __m128 real_mul1 = _mm_mul_ps(a_row1,b_row);
    __m128 real_mul2 = _mm_mul_ps(a_row2,b_row);
    __m128 real_mul3 = _mm_mul_ps(a_row3,b_row);
          
    __m128 c = _mm_shuffle_ps(b_row,b_row, _MM_SHUFFLE(2,3,0,1));// shuffle [real1 imag1 real2 imag2] into [imag1 real1 imag2 real2]
           
    __m128 imag_mul0 = _mm_mul_ps(a_row0, c);// computing A[i][k].real * B[k][j].imag and A[i][k].imag * B[k][j].real for the four rows
    __m128 imag_mul1 = _mm_mul_ps(a_row1, c);
    __m128 imag_mul2 = _mm_mul_ps(a_row2, c);
    __m128 imag_mul3 = _mm_mul_ps(a_row3, c);
          
    real_sum_vec0 = _mm_add_ps(real_sum_vec0,real_mul0); // accumulating    A[i][k].real * B[j][k].real and  A[i][k].imag * B[j][k].imag  for real values
    real_sum_vec1 = _mm_add_ps(real_sum_vec1,real_mul1);
    real_sum_vec2 = _mm_add_ps(real_sum_vec2,real_mul2);
    real_sum_vec3 = _mm_add_ps(real_sum_vec3,real_mul3);
          
    imag_sum_vec0 = _mm_add_ps(imag_sum_vec0,imag_mul0); // accumulating   (A[i][k].real * B[k][j].imag) and (A[i][k].imag * B[k][j].real) for imaginary values
    imag_sum_vec1 = _mm_add_ps(imag_sum_vec1,imag_mul1);
    imag_sum_vec2 = _mm_add_ps(imag_sum_vec2,imag_mul2);
    imag_sum_vec3 = _mm_add_ps(imag_sum_vec3,imag_mul3);
    }

    // peel and compute last 'odd' column of matrix    
    if  (max != a_cols) { 
         
      struct complex product0,product1,product2,product3;
      product0.real = A[i][k].real * B[j][k].real - A[i][k].imag * B[j][k].imag;
      product0.imag = A[i][k].real * B[j][k].imag + A[i][k].imag * B[j][k].real;
      sum0.real = product0.real;
      sum0.imag = product0.imag;
          
      product1.real = A[i+1][k].real * B[j][k].real - A[i+1][k].imag * B[j][k].imag;
      product1.imag = A[i+1][k].real * B[j][k].imag + A[i+1][k].imag * B[j][k].real;
      sum1.real = product1.real;
      sum1.imag = product1.imag;

      product2.real = A[i+2][k].real * B[j][k].real - A[i+2][k].imag * B[j][k].imag;
      product2.imag = A[i+2][k].real * B[j][k].imag + A[i+2][k].imag * B[j][k].real;
      sum2.real = product2.real;
      sum2.imag = product2.imag;

      product3.real = A[i+3][k].real * B[j][k].real - A[i+3][k].imag * B[j][k].imag;
      product3.imag = A[i+3][k].real * B[j][k].imag + A[i+3][k].imag * B[j][k].real;
      sum3.real= product3.real;
      sum3.imag = product3.imag;
    } 

    __m128 real_comp = _mm_hsub_ps(real_sum_vec0,real_sum_vec1);// computing (A[i][k].real * B[k][j].real) - (A[i][k].imag * B[k][j].imag) 
    __m128 real_comp1 =_mm_hsub_ps(real_sum_vec2,real_sum_vec3);
          
    __m128 imag_comp = _mm_hadd_ps(imag_sum_vec0,imag_sum_vec1); // computing (A[i][k].real * B[k][j].imag) + (A[i][k].imag * B[k][j].real). Optimised it, so that it utilises the second horizontal add 
    __m128 imag_comp1 = _mm_hadd_ps(imag_sum_vec2,imag_sum_vec3); 

    __m128 real_adds = _mm_hadd_ps(real_comp,real_comp);// accummulating  real computations
    __m128 imag_adds = _mm_hadd_ps(imag_comp,imag_comp);// accummulating  imaginary computations

    __m128 real_adds1 = _mm_hadd_ps(real_comp1,real_comp1);
    __m128 imag_adds1 = _mm_hadd_ps(imag_comp1,imag_comp1);

    // storing the four results
    sum0.real += real_adds[0];
    sum0.imag += imag_adds[0];

    sum1.real += real_adds[3];
    sum1.imag +=imag_adds[3];
            
    sum2.real += real_adds1[0];
    sum2.imag += imag_adds1[0];

    sum3.real += real_adds1[3];
    sum3.imag += imag_adds1[3];
      
    C[i][j] = sum0;
    C[i+1][j] = sum1;
    C[i+2][j] = sum2;
    C[i+3][j] = sum3;

 }

/*The function does 4 rows at the same time*/
void my_matmul(struct complex ** A, struct complex ** B, struct complex ** C, int a_rows, int a_cols, int b_cols)
{
  int i,j,k,counter,counter1,counter2;
  B = transpose(B, a_cols, b_cols); 
  int check = a_rows%4;
  int max=a_cols-(a_cols%2);// to run up to even rows 
  int a_cols_odd=a_cols%2;
  int a_rows_odd=a_rows%2;
  // loop peeling for rows that is not divisible by 4
   for (counter = 0; counter < check; counter++)
  {
      for(counter1 = 0; counter1 < b_cols; counter1++)
      {
          struct complex sum;
          sum.real = 0.0;
          sum.imag = 0.0;
          for(counter2 = 0; counter2 < a_cols; counter2++)
          {
              struct complex product;
              product.real = A[counter][counter2].real * B[counter1][counter2].real - A[counter][counter2].imag * B[counter1][counter2].imag;
              product.imag = A[counter][counter2].real * B[counter1][counter2].imag + A[counter][counter2].imag * B[counter1][counter2].real;
              sum.real += product.real;
              sum.imag += product.imag;
          }
          C[counter][counter1] = sum;
      }
  }
    #pragma omp parallel for collapse(2) shared(A,B,C) private (i,j,k) if((a_rows+a_cols+b_cols) > 400)
    for (i = check; i < a_rows; i +=4)
    {
      for(j = 0; j < b_cols; j++)
      {/* Only difference is my_matmul_odd uses loadu and my_matmul_even uses load. Since loadu is slower than load, we use load for even cols and rows, and loadu for odd cols and rows.  
      */ 
        if(a_rows_odd||a_cols_odd ){
          my_matmul_odd(j,i,A,B ,C ,max,a_cols);
        }
        else{
          my_matmul_even(j,i,A,B ,C ,max,a_cols);     
        } 
      }
    }
}

/*************************************************************************************/

long long time_diff(struct timeval * start, struct timeval * end) {
  return (end->tv_sec - start->tv_sec) * 1000000L + (end->tv_usec - start->tv_usec);
}

int main(int argc, char ** argv)
{
  struct complex ** A, ** B, ** C;
  struct complex ** control_matrix;
  long long control_time, mul_time;
  double speedup;
  int a_dim1, a_dim2, b_dim1, b_dim2, errs;
  struct timeval pre_time, start_time, stop_time;

  if (argc != 5) {
    fprintf(stderr, "Usage: matmul-harness <A nrows> <A ncols> <B nrows> <B ncols>\n");
    exit(1);
  }
  else {
    a_dim1 = atoi(argv[1]);
    a_dim2 = atoi(argv[2]);
    b_dim1 = atoi(argv[3]);
    b_dim2 = atoi(argv[4]);
  }

  /* check the matrix sizes are compatible */
  if (a_dim2 != b_dim1) {
    fprintf(stderr,
      "FATAL number of columns of A (%d) does not match number of rows of B (%d)\n",
      a_dim2, b_dim1);
    exit(1);
  }

  /* allocate the matrices */
  A = gen_random_matrix(a_dim1, a_dim2);
  B = gen_random_matrix(b_dim1, b_dim2);
  C = new_empty_matrix(a_dim1, b_dim2);
  control_matrix = new_empty_matrix(a_dim1, b_dim2);

  DEBUGGING( {
    printf("matrix A:\n");
    write_out(A, a_dim1, a_dim2);
    printf("\nmatrix B:\n");
    write_out(A, a_dim1, a_dim2);
    printf("\n");
  } )

  /* record control start time */
  gettimeofday(&pre_time, NULL);

  /* use a simple matmul routine to produce control result */
  matmul(A, B, control_matrix, a_dim1, a_dim2, b_dim2);

  /* record starting time */
  gettimeofday(&start_time, NULL);

  /* perform matrix multiplication */
  my_matmul(A, B, C, a_dim1, a_dim2, b_dim2);

  /* record finishing time */
  gettimeofday(&stop_time, NULL);

  /* compute elapsed times and speedup factor */
  control_time = time_diff(&pre_time, &start_time);
  mul_time = time_diff(&start_time, &stop_time);
  speedup = (float) control_time / mul_time;

  printf("Matmul time: %lld microseconds\n", mul_time);
  printf("control time : %lld microseconds\n", control_time);
  if (mul_time > 0 && control_time > 0) {
    printf("speedup: %.2fx\n", speedup);
  }

  check_result(C, control_matrix, a_dim1, b_dim2);

  /* free all matrices */
  free_matrix(A);
  free_matrix(B);
  free_matrix(C);
  free_matrix(control_matrix);

  return 0;
}
