#include <iostream>
#include <thread>
#include <pthread.h>

using namespace std;

long long **matrix1, **matrix2, **matrix3;
int r1, c1;
int r2, c2;
int r3, c3;
int N_THREADS;

long long ** cache_opt_alloc(int r, int c){
    long long *aux = new long long[r*c];
    long long ** mat  = new long long*[r];
    for(int i = 0; i<r; i++)
        mat[i] = aux + i*c;
    return mat;
}

void* matrix_multiplication(void* psp){
    long starting_point = (long) psp;
    for(long i = starting_point; i < r3*c3; i += N_THREADS){
        long x = i/c3;
        long y = i%c3;
        matrix3[x][y] = 0;
        for(long j = 0; j<c1; j++){
            matrix3[x][y] += matrix1[x][j]*matrix2[j][y];
        }
    }

    return nullptr;
}

void print_matrix(long long **matrix, int r, int c){
    for(int i = 0; i<r; i++){
        for(int j = 0; j<c; j++){
            cout<<matrix[i][j]<<" ";
        }
        cout<<endl;
    }
}

void fill_matrix(long long **matrix, int r, int c){
    for(int i = 0; i<r; i++){
        for(int j = 0; j<c; j++){
            matrix[i][j] = rand()%21;
        }
    }
}

int main(){
    cin>>r1>>c1;
    cin>>r2>>c2;
    if(c1 != r2) 
        throw "Can't multiply matrices with this dimensions.";

    r3 = r1;
    c3 = c2;

    matrix1 = cache_opt_alloc(r1, c1);
    matrix2 = cache_opt_alloc(r2, c2);
    matrix3 = cache_opt_alloc(r3, c3);

    fill_matrix(matrix1, r1, c1);
    fill_matrix(matrix2, r2, c2);

    N_THREADS = std::thread::hardware_concurrency();

    cout<<"Matrix 2:"<<endl;
    print_matrix(matrix1, r3, c3);

    cout<<endl;

    cout<<"Matrix 2:"<<endl;
    print_matrix(matrix2, r3, c3);

	int rc;
	pthread_t threads[N_THREADS];

	for(long i = 0; i<N_THREADS; i++){
		rc = pthread_create(threads + i, nullptr, matrix_multiplication, (void *) i);
		if (rc) throw "ERROR";
	}

	for (long i=0; i<N_THREADS; i++){
		rc = pthread_join(threads[i], nullptr);
		if (rc) throw "ERROR";
	}
    
    cout<<endl;

    cout<<"Matrix 3:"<<endl;
    print_matrix(matrix3, r3, c3);

    return 0;
}
