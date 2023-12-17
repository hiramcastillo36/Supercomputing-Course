/*
-Materia: Supercómputo
-Semestre: 5to semestre
-Nombre del alumno: Hiram Jalil Castillo Gutierrez
-Clave del alumno: 0345169
-Carrera: Ingenieria en Sistemas Inteligentes
-Nombre de tarea o programa: MPI: Suma de elementos de un arreglo
-Periodo de evaluación: Parcial 2
-Descripción: Suma de un arreglo utilizando N procesos, el proceso 0 es el encargado de enviar y recibir 
              la parte del arreglo que le corresponde a los n-1 procesos disponible.
-Avance logrado (0 a 100%): 100%
-Comentarios adicionales: Es un paradigma diferente de la programación secuencial, el analizar como enviarlo y recibir
                          entre procesos es la parte que se debe analizar y tener más cuidado. Por el momento me gusto 
                          realizar programación con MPI.
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

// mpicc archivo.c -o ejecutable
// mpirun -np 4 ./ejecutable

int *create_array(int n) {
    int *array = (int *)malloc(n * sizeof(int));
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        array[i] = rand() % 10000;
    }
    return array;
}

int main(int argc, char *argv[]) {
    int idproc, numproc;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &idproc);
    MPI_Comm_size(MPI_COMM_WORLD, &numproc);

    int size_array = 10;
    int suma = 0;
    int *array = create_array(size_array);
    
    int nDatos = size_array / (numproc-1);
    int nDatosU = size_array;

    if (idproc == 0) {
        for (int i = 1; i < numproc - 1; i++) {
            MPI_Send(&array[(i - 1) * nDatos], nDatos, MPI_INT, i, 1, MPI_COMM_WORLD);
            //printf("%d %d\n", (i-1)*nDatos, i*nDatos-1);
        }
        MPI_Send(&array[(numproc-2)*nDatos], nDatosU - (nDatos * (numproc-2)), MPI_INT, numproc-1, 1, MPI_COMM_WORLD);
        
        int sumap = 0;
        for (int i = 1; i < numproc; i++) {
            MPI_Recv(&sumap, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            suma += sumap;
        }
        printf("Array 1: ");
        for(int i = 0; i < size_array; i++){
            printf("%d ", array[i]);
        }
        printf("\nLa suma es: %d\n", suma);
    } else {
        int nDatosLocal = nDatos;
        if(idproc == numproc-1){
            nDatosLocal = nDatosU - (nDatos * (numproc-2));
        }
        int *result = (int *)malloc(nDatosLocal * sizeof(int));

        MPI_Recv(result, nDatosLocal, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        //printf("%d", nDatosLocal);
        for (int i = 0; i < nDatosLocal; i++) {
            suma += result[i];
        }
        MPI_Send(&suma, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        free(result);
    }
    free(array);

    MPI_Finalize();
    return 0;
}
