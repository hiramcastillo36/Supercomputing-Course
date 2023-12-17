/*
-Materia: Supercómputo
-Semestre: 5to semestre
-Nombre del alumno: Hiram Jalil Castillo Gutierrez
-Clave del alumno: 0345169
-Carrera: Ingenieria en Sistemas Inteligentes
-Nombre de tarea o programa: MPI: MPI_Bcast
-Periodo de evaluación: Parcial 2
-Descripción: Uso de la función MPI_Bcast para enviar un arreglo de un proceso a todos los demás procesos.
-Avance logrado (0 a 100%): 100%
-Comentarios adicionales: El programa es muy sencillo, el proceso 0 crea un arreglo de tamaño n y lo llena 
                            con números aleatorios, después se utiliza la función MPI_Bcast para enviar 
                            el arreglo a todos los demás procesos.
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

// mpicc archivo.c -o ejecutable
// mpirun -np 4 ./ejecutable

float *create_array_float(int n) {
    float *array = (float *)malloc(n * sizeof(float));
    return array;
}

int main(int argc, char *argv[]) {
    int idproc, numproc;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &idproc);
    MPI_Comm_size(MPI_COMM_WORLD, &numproc);

    int size_array = 5;
    int suma = 0;
    
    int nDatos = size_array / (numproc-1);
    int nDatosU = size_array;
    float *array_a = create_array_float(size_array);
    if (idproc == 0) {
        srand(time(NULL));
        for (int i = 0; i < size_array; i++) {
            array_a[i] = ((float)rand()/(float)(RAND_MAX)) * 100.0;
        }
    }
    
    MPI_Bcast(array_a, size_array, MPI_INT, 0, MPI_COMM_WORLD);

    if(idproc != 0){
        for(int i=0; i<size_array; i++){
            printf("%f ", array_a[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
