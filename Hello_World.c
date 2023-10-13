#include <stdio.h>
#include <string.h>
#include "mpi.h"

// mpicc archivo.c -o ejecutable
// mpirun -np 4 ./ejecutable

int main(int argc, char *argv[]){
    int idproc, numproc;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &idproc);
    MPI_Comm_size(MPI_COMM_WORLD, &numproc);

    printf("Hello world from process %d of %d\n", idproc, numproc);
    MPI_Finalize();
    return 0;
}