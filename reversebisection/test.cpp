#include <iostream>
// #include <algorithm>

using  namespace std;
int i = 0;
int j = 0;
void swap(int array[], int i, int j) {
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}

int find_kth(int *v, int n, int k, int *y) {
    int j0 = 0;
    if (n == 1 && k == 0) return v[0];
 
    int m = (n + 4)/5;
    int *medians = new int[m];
    for (i=0; i<m; i++) {
        if (5*i + 4 < n) {
            int *w = v + 5*i;
            int *w1 = y + 5*i;
            for (j0=0; j0<3; j0++) {
                int jmin = j0;
                for (j=j0+1; j<5; j++) {
                    if (w[j] < w[jmin]) jmin = j;
                }
                swap(w, j0, jmin);
                swap(w1, j0, jmin);
            }
            medians[i] = w[2];
        } else {
            medians[i] = v[5*i];
        }
    }
 
    int pivot = find_kth(medians, m, m/2, medians);
    // delete [] medians;
    delete [] medians; 
    for (i=0; i<n; i++) {
        if (v[i] == pivot) {
            swap(v, i, n-1);
            swap(y, i, n-1);
            break;
        }
    }
 
    int store = 0;
    for (i=0; i<n-1; i++) {
        if (v[i] < pivot) {
            swap(v, i, store);
            swap(y, i, store);
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

int main() {
    int arr[] = {1, 4, 3, 5, 6, 7, 0, 8, 10, 2, 4, 11, 20, 22, 14, 15, 16, 18, 17, 21};
    int arr1[] = {1, 4, 3, 5, 6, 7, 0, 8, 10, 2, 4, 11, 20, 22, 14, 15, 16, 18, 17, 21};
    int res = find_kth(arr + 4, 8, 2, arr1 + 4);
    // cout<<res<<endl;
    for (int i = 0; i < 20; i++) {
        cout<<arr[i]<< " ";
        cout<<arr1[i]<<" ";
    }
}
