#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define NUM_POINTS 524288

unsigned int X_axis[NUM_POINTS];
unsigned int Y_axis[NUM_POINTS];

// void find_quadrants_helper (num_quadrants, points);
void  quicksort(int[] array, int size, int[] array1);
void quicksort(int[] array, int start, int end, int[] array1);
int partition(int[] array, int start, int end, int[] array1);
void swap(int[] array, int i, int j);


int top[num_quadrants];
int left[num_quadrants];
int right[num_quadrants];
int bottom[num_quadrants];

unsigned int sorted_x_axis[NUM_POINTS]
unsigned int sorted_y_axis[NUM_POINTS]
unsigned int sorted_x_yaxis[NUM_POINTS]
unsigned int sorted_y_xaxis[NUM_POINTS]
int i = 0;
int j = 0;
int k = 0;
int global_cost = 0;

void find_quadrants (num_quadrants)
int num_quadrants;
{
    /* YOU NEED TO FILL IN HERE */
    //sort X_axis and Y_axis
    for (i = 0; i < NUM_POINTS; i++) {
        sorted_x_axis[i] = X_axis[i];
        sorted_y_axis[i] = Y_axis[i];
    }
    quicksort(sorted_x_axis, NUM_POINTS, sorted_x_yaxis);
    quicksort(sorted_y_axis, NUM_POINTS, sorted_y_xaxis);

    //
    int quadrants = 1;
    top[0] = 0;
    left[0] = 0;
    right[0] = NUM_POINTS-1;
    bottom[0] = NUM_POINTS-1;
    bool x_cut = false;
    while (num_quadrants > quadrants) {
        if (x_cut == false) {
            //cut in x direction
            int current_quadrants = quadrants;
            for (i = 0; i < current_quadrants; i++) {
                int current_top = top[i];
                int current_left = left[i];
                int current_right = right[i];
                int current_bottom = bottom[i];
                j = current_left;
                int points = NUM_POINTS / current_quadrants;
                int count = 0;
                while (sorted_x_axis[j] < sorted_x_axis[current_right] && count < points/2) {
                    j++;
                    count++;
                }
                right[i] = j;
                top[i+quadrants] = top[i];
                bottom[i+quadrants] = bottom[i];
                left[i+quadrants] = j+1;
                right[i+quadrants] = current_right;
            }
         
        } else {
            //cut in y direction
            int current_quadrants = quadrants;
            for (i = 0; i < current_quadrants; i++) {
                int current_top = top[i];
                int current_left = left[i];
                int current_right = right[i];
                int current_bottom = bottom[i];
                j = current_left;
                int points = NUM_POINTS / current_quadrants;
                int count = 0;
                while (sorted_y_axis[j] < sorted_y_axis[current_right] && count < points/2) {
                    j++;
                    count++;
                }
                bottom[i] = j;
                top[i+quadrants] = top[i];
                bottom[i+quadrants] = current_bottom;
                left[i+quadrants] = left[i];
                right[i+quadrants] = right[i];
            }
        }
        quadrants = quadrants * 2;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    //calculate cost
    int local_cost = 0;
    for (i = myid; i <= num_quadrants; i += numprocs)
    {
       int points = NUM_POINTS / num_quadrants;
       for (j = 0; j < points; j++) {
            for (k = j; k < points; k++) {
                int diff_x = sorted_x_axis[left[i]+j] - sorted_x_axis[left[i]+k];
                int diff_y = sorted_x_yaxis[left[i]+j] - sorted_x_yaxis[left[i]+k]; 
                local_cost += sqrt(diff_x * diff_x + diff_y * diff_y);
            }
       }     

    }

     MPI_Reduce(&local_cost, &global_cost, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
}



void quicksort(int[] array, int size, int[] array1) {
    if (size == 0) {
        return;
    }
    quicksort(array, 0, size-1, array1);
}

void quicksort(int[] array, int start, int end, int[] array1) {
    if (start >= end) {
        return;
    }
    int index = partition (array, start, end, array1);
    quicksort(array, start, index-1, array1);
    quicksort(array, index+1, end, array1);
}

int partition(int[] array, int start, int end, int[] array1) {
    int i = start;
    while (i < end) {
        if (array[i] <= array[end]) {
            swap(array, i, start++);
            swap(array1, i, start++);
        }
        i++;
    }
    swap(array, start, end);
    swap(array1, start, end);
    return start;
}

void swap(int[] array, int i, int j) {
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}

int main(argc,argv)
int argc;
char *argv[];
{
    int num_quadrants;
    int myid, numprocs;
    int  namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    
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
        }
    
    MPI_Bcast(&X_axis, NUM_POINTS, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&Y_axis, NUM_POINTS, MPI_INT, 0, MPI_COMM_WORLD);  
    
    find_quadrants (num_quadrants);
    
    MPI_Finalize();
    return 0;
}


