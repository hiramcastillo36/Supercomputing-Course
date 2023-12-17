/*
-Materia: Supercómputo
-Semestre: 5to semestre
-Nombre del alumno: Hiram Jalil Castillo Gutierrez
-Clave del alumno: 0345169
-Carrera: Ingenieria en Sistemas Inteligentes
-Nombre de tarea o programa: MPI: Suma de dos arreglos
-Periodo de evaluación: Parcial 2
-Descripción: Suma de dos arreglos utilizando N procesos, el proceso 0 es el encargado de enviar y recibir 
              la parte del arreglo que le corresponde a los n-1 procesos disponible.
-Avance logrado (0 a 100%): 100%
-Comentarios adicionales: Fue muy parecido al problema enterior, unicamente fue agregar funciones de enviar 
                          y recibir los dos arreglos.
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

// mpicc archivo.c -o ejecutable
// mpirun -np 4 ./ejecutable

int *create_array(int n) {
    int *array = (int *)malloc(n * sizeof(int));
    return array;
}

int main(int argc, char *argv[]) {
    int idproc, numproc;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &idproc);
    MPI_Comm_size(MPI_COMM_WORLD, &numproc);

    int size_array = 10;
    int suma = 0;
    
    int nDatos = size_array / (numproc-1);
    int nDatosU = size_array;
    
    if (idproc == 0) {
        int *array_a = create_array(size_array);
        int *array_b = create_array(size_array);
        srand(time(NULL));
        for (int i = 0; i < size_array; i++) {
            array_a[i] = rand() % 100;
            array_b[i] = rand() % 100;
        }
        printf("Array 1: ");
        for(int i=0; i<size_array; i++){
            printf("%d ", array_a[i]);
        }
        printf("\n");
        printf("Array 2: ");
        for(int i=0; i<size_array; i++){
            printf("%d ", array_b[i]);
        }
        printf("\n");
        for (int i = 1; i < numproc - 1; i++) {
            MPI_Send(&array_a[(i - 1) * nDatos], nDatos, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&array_b[(i - 1) * nDatos], nDatos, MPI_INT, i, 1, MPI_COMM_WORLD);
        }
        MPI_Send(&array_a[(numproc-2)*nDatos], nDatosU - (nDatos * (numproc-2)), MPI_INT, numproc-1, 1, MPI_COMM_WORLD);
        MPI_Send(&array_b[(numproc-2)*nDatos], nDatosU - (nDatos * (numproc-2)), MPI_INT, numproc-1, 1, MPI_COMM_WORLD);
        
        int *aux = create_array(size_array);
        for(int i=0; i<size_array; i++){
            aux[i] = 0;
        }
        for (int i = 1; i < numproc-1; i++) {
            MPI_Recv(&aux[(i - 1) * nDatos], nDatos, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        MPI_Recv(&aux[(numproc-2)*nDatos], nDatosU - (nDatos * (numproc-2)), MPI_INT, numproc-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        printf("Array resultados: ");
        for(int i = 0; i < size_array; i++){
            printf("%d ", aux[i]);
        }
        free(array_a);
        free(array_b);
        free(aux);
    } else {
        int nDatosLocal = nDatos;
        if(idproc == numproc-1){
            nDatosLocal = nDatosU - (nDatos * (numproc-2));
        }
        int *resultados_a = create_array(nDatosLocal);
        int *resultados_b = create_array(nDatosLocal);
        int *env = create_array(nDatosLocal);
        MPI_Recv(resultados_a, nDatosLocal, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(resultados_b, nDatosLocal, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        for (int i = 0; i < nDatosLocal; i++) {
            env[i] = resultados_a[i] + resultados_b[i];
        }
        MPI_Send(env, nDatosLocal, MPI_INT, 0, 0, MPI_COMM_WORLD);
        free(resultados_a);
        free(resultados_b);
        free(env);  
    }
    

    MPI_Finalize();
    return 0;
}
