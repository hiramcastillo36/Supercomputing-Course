/*
-Materia: Supercómputo
-Semestre: 5to semestre
-Nombre del alumno: Hiram Jalil Castillo Gutierrez
-Clave del alumno: 0345169
-Carrera: Ingenieria en Sistemas Inteligentes
-Nombre de tarea o programa: MPI: MPI_Reduce
-Periodo de evaluación: Parcial 2
-Descripción: Hacer uso de la funcion MPI_Reduce para realizar una suma de los datos en todos los procesos y almacena el resultado en el proceso 0.
-Avance logrado (0 a 100%): 100%
-Comentarios adicionales: Es una funcion muy util, ya que no es necesario realizar un for para sumar los datos de todos los procesos.
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int *create_array(int n) {
    int *array = (int *)malloc(n * sizeof(int));
    return array;
}


int main(int argc, char** argv) {
    int rank, size;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int N = 10; // Tamaño del arreglo
    int *data=create_array(N); // Arreglo de tamaño N
    int *result=create_array(N); // Arreglo donde se almacenará el resultado en el proceso 0
    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        data[i] = (rand() % 10) * (rank + 1) ;
        printf("%d ", data[i]);
    }    
    printf("\n");

    MPI_Reduce(data, result, N, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("\n");
        printf("Operacion suma, resultado en el proceso 0:\n");
        for (int i = 0; i < N; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
    }

    free(data);
    free(result);
    
    MPI_Finalize();
    return 0;
}
