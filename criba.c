#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

int *create_array(int n){
    int *array = (int *)malloc(n * sizeof(int));
    return array;
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 10;
    int *a = create_array(ceil(sqrt(n) + 1));
    int nDatos = ceil(sqrt(n) + 1) / (size-1);
    int nDatosU = ceil(sqrt(n) + 1);
    int *primes = create_array(n+1);

    int sendcounts[size+1];
    int displs[size+1];
    
    if(rank == 0){
        primes[0] = 1;
        primes[1] = 1;
        for(int i=2; i<=n; i++){
            primes[i] = 1;
        }
        for(int i=0; i<=ceil(sqrt(n)); i++){
            a[i] = i+2;
        }
    }

    displs[0] = 0;
    sendcounts[0] = 0;
    displs[1] = 0;
    sendcounts[1] = nDatos;

    for (int i = 2; i < size; i++) {
        sendcounts[i] = nDatos;
        displs[i] = displs[i - 1] + nDatos;
        if(i == size-1){
            sendcounts[i] = nDatosU - (nDatos * (size-2));
            displs[i] =   (nDatos * (size-2)) ;
        }
    }

    int nDatosLocal = nDatos;
    if(rank == size-1){
        nDatosLocal = nDatosU - (nDatos * (size-2));
    } 
    int localData[nDatosLocal];
    
    MPI_Scatterv(a, sendcounts, displs, MPI_INT, localData, nDatosLocal, MPI_INT, 0, MPI_COMM_WORLD);
    
    int localPrimes[n+1];

    for(int i=0; i<=n; i++){
        localPrimes[i] = 1;
    }
    
    for (int i = 0; i < sendcounts[rank]; i++) {
        for(int j=localData[i]; j<= n+1; j+=localData[i]){
            if(j<=n+1 && j != localData[i])
                localPrimes[j] = 0;
        }
    }

    MPI_Reduce(localPrimes, primes, n+1, MPI_INT, MPI_LAND, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Primos en proceso 0: \n");
        for (int i = 2; i <= n; i++) {
            if(primes[i] == 1)
                printf("%d ", i);
        }
        printf("\n");   
    }

    MPI_Finalize();

    return 0;
}
