#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define NUM_POINTS 8

unsigned int X_axis[NUM_POINTS];
unsigned int Y_axis[NUM_POINTS];

void swap(int array[], int i, int j);
int find_kth(int *v, int n, int k, int *y);

int i = 0;
int j = 0;
int k = 0;
int z = 0;
double global_cost = 0;

int numprocs;  /* Number of processors to use */
int myid;

int num_quadrants;
void find_quadrants (num_quadrants)
{
    /* YOU NEED TO FILL IN HERE */
    int x_cut = 0;
    int quadrants = 1;

    //coordinates of the quadrants
   
    int top[num_quadrants];
    int left[num_quadrants];
    int right[num_quadrants];
    int bottom[num_quadrants];
    int pivot_arr[num_quadrants];

    while (num_quadrants > quadrants) { 
         int points = NUM_POINTS / quadrants;
        if (!x_cut) {
            for (i = 0; i < quadrants; i++) {
                int x_pivot = find_kth(X_axis + i * points, points, points/2 - 1, Y_axis + i * points);
                // printf("\n xcut %d \n", x_pivot);
                //  for (z = 0; z < NUM_POINTS; z++) {
                //  printf(" %d ", X_axis[z]);

                // }
                // printf("\n");

                // for (z = 0; z < NUM_POINTS; z++) {
                //     printf(" %d ", Y_axis[z]);

                // }
                // printf("\n");
                k = i * points;
                j = i * points + points -1;
                while (k < j && k < k + points/2) {
                    if (X_axis[k] > x_pivot) {
                        while (X_axis[j] > x_pivot) {
                            j--;
                        }
                        if (k < j) {
                            swap(X_axis, k, j);
                            swap(Y_axis, k, j);
                        } 
                    }
                    k++;
                }
                // for (z = 0; z < NUM_POINTS; z++) {
                //  printf(" %d ", X_axis[z]);

                // }
                // printf("\n");

                // for (z = 0; z < NUM_POINTS; z++) {
                //     printf(" %d ", Y_axis[z]);

                // }
                // printf("\n");
                pivot_arr[i+quadrants-1] = x_pivot;
            }
            x_cut = 1;  
        } else {
            // printf("\n num qua  %d", quadrants);
            for (i = 0; i < quadrants; i++) {
                // for (z = 0; z < NUM_POINTS; z++) {
                //     printf(" %d ", Y_axis[z]);

                // }
                // for (z = 0; z < points; z++) {
                //     printf(" index %d, %d ", z+ i* points, Y_axis[z+i * points]);
                // }
                int y_pivot = find_kth(Y_axis + i * points, points, points/2 - 1, X_axis + i * points);
                // printf("\n ycut %d \n", y_pivot);

                // for (z = 0; z < NUM_POINTS; z++) {
                //  printf(" %d ", X_axis[z]);

                // }
                // printf("\n");

                // for (z = 0; z < NUM_POINTS; z++) {
                //     printf(" %d ", Y_axis[z]);

                // }
                // printf("\n");

                k = i * points;
                j = i * points + points - 1;
                while (k < j && k < k + points/2) {
                    if (Y_axis[k] > y_pivot) {
                        while (Y_axis[j] > y_pivot) {
                            j--;
                        }
                        if (k < j) {
                            swap(X_axis, k, j);
                            swap(Y_axis, k, j);
                        } 
                    }
                    k++;
                }
                // for (z = 0; z < NUM_POINTS; z++) {
                //  printf(" %d ", X_axis[z]);

                // }
                // printf("\n");

                // for (z = 0; z < NUM_POINTS; z++) {
                //     printf(" %d ", Y_axis[z]);

                // }
                // printf("\n");
                // printf("\n index i %d", i);
                // printf("\n index  %d", quadrants + i -1);
                pivot_arr[i+quadrants-1] = y_pivot;
            }  
            x_cut = 0;
        }
        quadrants *= 2;

    }

    //find border of initial quadrant
    int min_x = X_axis[0];
    int max_x = X_axis[0];
    int min_y = Y_axis[0];
    int max_y = Y_axis[0];
    for (i = 1; i < NUM_POINTS; i++) {
        if (X_axis[i] < min_x) {
            min_x = X_axis[i];
        }
        if (X_axis[i] > max_x) {
            max_x = X_axis[i];
        }
        if (Y_axis[i] < min_y) {
            min_y = Y_axis[i];
        }
        if (Y_axis[i] > max_y) {
            max_y = Y_axis[i];
        }
    }

    // for (i = 0; i < num_quadrants-1; i++) {
    //     printf(" %d ", pivot_arr[i]);

    // }

    // printf("\nminx maxx %d, %d", min_x, max_x);
    // printf("\nminy maxy %d, %d", min_y, max_y);

    //find coordinates of quadrants
    top[0] = min_y;
    bottom[0] = max_y;
    left[0] = min_x;
    right[0] = max_x;
    i = 0;
    x_cut = 0;
    quadrants = 1;
    while (i < num_quadrants-1) {
        int temp = i;
        for (j = 0; j < quadrants; j++) {
            top[j+quadrants] = top[j];
            bottom[j+quadrants] = bottom[j];
            left[j+quadrants] = left[j];
            right[j+quadrants] = right[j];
        }
        if (!x_cut) {

            for (j = 0; j < quadrants * 2; j+=2) {
                top[j] = top[quadrants + j/2];
                bottom[j] = bottom[quadrants + j/2];
                left[j] = left[quadrants + j/2];
                right[j] = pivot_arr[temp];

                top[j+1] = top[quadrants + j/2];
                bottom[j+1] = bottom[quadrants + j/2];
                left[j+1] = pivot_arr[temp];
                right[j+1] = right[quadrants + j/2];
                temp++;
            }
            x_cut = 1;
        } else{
            for (j = 0; j < quadrants * 2; j+=2) {
                top[j] = top[quadrants + j/2];
                bottom[j] = pivot_arr[temp];
                left[j] = left[quadrants + j/2];
                right[j] = right[quadrants + j/2];
                
                top[j+1] = pivot_arr[temp];
                bottom[j+1] = bottom[quadrants + j/2];
                left[j+1] = left[quadrants + j/2];
                right[j+1] = right[quadrants + j/2];
                temp++;
            }
            x_cut = 0;
        }
        i += quadrants;
        quadrants *= 2;
    }

    //print coordinates of quadrants
    if (myid == 0) {
         printf("\nPrint quadrants coordinates...... format: quarant number: top-left, top-right, bottom-left, bottom-right\n");
        for (i = 0; i < num_quadrants; i++) {
            printf("\nNumber %d : " + i);
            printf(" (%d,%d) ", left[i], top[i]);
            printf(" (%d,%d) ", right[i], top[i]);
            printf(" (%d,%d) ", left[i], bottom[i]);
            printf(" (%d,%d) \n", right[i], bottom[i]);
        }
    }
   
  

    // MPI_Barrier(MPI_COMM_WORLD);

    //calculate cost

    //     for (z = 0; z < NUM_POINTS; z++) {
    //  printf(" %d ", X_axis[z]);

    // }
    // printf("\n");

    // for (z = 0; z < NUM_POINTS; z++) {
    //     printf(" %d ", Y_axis[z]);

    // }
    // printf("\n");

    double local_cost = 0;
    for (i = myid; i <= num_quadrants-1; i += numprocs)
    {
       int points = NUM_POINTS / num_quadrants;
       for (j = 0; j < points - 1; j++) {
            for (k = j+1; k < points; k++) {
                int x1 = points * i + j;
                int x2 = points * i + k;
                int y1 = points * i + j;
                int y2 = points * i + k;

                double diff_x = abs(X_axis[x1] - X_axis[x2]);
                double diff_y = abs(Y_axis[y1] - Y_axis[y2]);
                // printf("\n x f: %d %d %d %d %d %d %d %d %f %f", x1, x2, y1, y2, X_axis[x1], X_axis[x2], Y_axis[y1], Y_axis[y2], diff_x, diff_y);
                // double distance = diff_x * diff_x + diff_y * diff_y; 
                // printf("\ndistance: %f", distance);
                local_cost += sqrt((double)diff_x * diff_x + diff_y * diff_y);
                // printf("\nlocal_cost: %f", local_cost);
            }
       }     
    }
    // printf("\nlocal_cost: %lf", local_cost);

     MPI_Reduce(&local_cost, &global_cost, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
}


int find_kth(int *v, int n, int k, int *y) {
    int j0 = 0;
    int i1 = 0;
    int j1 = 0;
    if (n == 1 && k == 0) return v[0];
 
    int m = (n + 4)/5;
    int *medians =  (int *)malloc(m * sizeof(int));
    for (i1=0; i1<m; i1++) {
        if (5*i1 + 4 < n) {
            int *w = v + 5*i1;
            int *w1 = y + 5*i1;
            for (j0=0; j0<3; j0++) {
                int jmin = j0;
                for (j1=j0+1; j1<5; j1++) {
                    if (w[j1] < w[jmin]) jmin = j1;
                }
                swap(w, j0, jmin);
                swap(w1, j0, jmin);
            }
            medians[i1] = w[2];
        } else {
            medians[i1] = v[5*i1];
        }
    }
 
    int pivot = find_kth(medians, m, m/2, medians);
    // delete [] medians;
    free(medians);
 
    for (i1=0; i1<n; i1++) {
        if (v[i1] == pivot) {
            swap(v, i1, n-1);
            swap(y, i1, n-1);
            break;
        }
    }
 
    int store = 0;
    for (i1=0; i1<n-1; i1++) {
        if (v[i1] < pivot) {
            swap(v, i1, store);
            swap(y, i1, store);
            store++;
        }
    }
    swap(v, store, n-1);
    swap(y, store, n-1);
 
    if (store == k) {
        return pivot;
    } else if (store > k) {
        return find_kth(v, store, k, y);
    } else {
        return find_kth(v+store+1, n-store-1, k-store-1, y+store+1);
    }
}


void swap(int array[], int i, int j) {
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}

int main(argc,argv)
int argc;
char *argv[];
{
    int num_quadrants;
    // int myid, numprocs;
    int  namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    /*Time Variables*/
    double startwtime = 0.0, endwtime;
    
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Get_processor_name(processor_name,&namelen);
    
    if (argc != 2)
    {
        fprintf (stderr, "Usage: recursive_bisection <#of quadrants>\n");
        MPI_Finalize();
        exit (0);
    }
    
    fprintf (stderr,"Process %d on %s\n", myid, processor_name);
    
    num_quadrants = atoi (argv[1]);
    
    if (myid == 0)
        fprintf (stdout, "Extracting %d quadrants with %d processors \n", num_quadrants, numprocs);
        
        if (myid == 0)
        {
            int i;
            
            srand (10000);
            
            for (i = 0; i < NUM_POINTS; i++)
                X_axis[i] = (unsigned int)rand();

            
            for (i = 0; i < NUM_POINTS; i++)
                Y_axis[i] = (unsigned int)rand();

            
            printf("\nComputing Parallely Using MPI.\n");
            startwtime = MPI_Wtime();
            // for (i = 0; i < NUM_POINTS; i++) {
            //      printf(" %d ", X_axis[i]);

            // }
            // printf("\n");

            // for (i = 0; i < NUM_POINTS; i++) {
            //     printf(" %d ", Y_axis[i]);

            // }
               
               
        }
    
    MPI_Bcast(&X_axis, NUM_POINTS, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&Y_axis, NUM_POINTS, MPI_INT, 0, MPI_COMM_WORLD);  
    
    find_quadrants (num_quadrants);

    // MPI_Barrier(MPI_COMM_WORLD);

    if (myid == 0) {
        endwtime = MPI_Wtime();
        printf("\nelapsed time = %f\n", endwtime - startwtime);
        // for (i = 0; i < NUM_POINTS; i++) {
        //      printf(" %d ", X_axis[i]);

        // }
        // printf("\n");

        // for (i = 0; i < NUM_POINTS; i++) {
        //     printf(" %d ", Y_axis[i]);

        // }
        printf("\nTotal cost:  %lf \n", global_cost);

    }
    
    MPI_Finalize();


    return 0;
}


