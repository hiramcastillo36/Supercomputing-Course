/*
-Materia: Supercómputo
-Semestre: 5to semestre
-Nombre del alumno: Hiram Jalil Castillo Gutierrez
-Clave del alumno: 0345169
-Carrera: Ingenieria en Sistemas Inteligentes
-Nombre de tarea o programa: MPI: MPI_Gatherv y MPI_Scatterv
-Periodo de evaluación: Parcial 2
-Descripción: Suma de un arreglo utilizando N procesos, el proceso 0 es el encargado de enviar y recibir 
              la parte del arreglo que le corresponde a los n-1 procesos disponible. Utilizando las funciones
                MPI_Gatherv y MPI_Scatterv.
-Avance logrado (0 a 100%): 100%
-Comentarios adicionales: Conocer estas funciones ayudan a tener un codigo mas simple y entendible, ya que
                          no es necesario realizar un for para enviar y recibir los datos.
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <math.h>

int *create_array(int n) {
    int *array = (int *)malloc(n * sizeof(int));
    return array;
}

int main(int argc, char *argv[]) {
    int idproc, numproc;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &idproc);
    MPI_Comm_size(MPI_COMM_WORLD, &numproc);

    int size_array = 13;
    int suma = 0;
    
    int nDatos = size_array / (numproc - 1);
    int nDatosU = size_array;
    
    int *array_a = create_array(size_array);
    int *array_b = create_array(ceil(sqrt(size_array) + 1));
    int *array_results = create_array(size_array);
    int *array_sizes = create_array(numproc+1);
    int *displacements = create_array(numproc+1);

    array_sizes[0] = 0;
    displacements[0] = 0;
    
    for (int i = 1; i < numproc; i++) {
        array_sizes[i] = nDatos;
        displacements[i] = displacements[i - 1] + nDatos;
        if(i == numproc-1){
            array_sizes[i] = nDatosU - (nDatos * (numproc-2));
            displacements[i] =   (nDatos * (numproc-2)) ;
        }
    }
    int nDatosLocal = nDatos;
    if(idproc == numproc-1){
        nDatosLocal = nDatosU - (nDatos * (numproc-2));
    } else {
        if(idproc == 0){
            nDatosLocal = 0;
        }
    }

    if (idproc == 0) {
        
        array_a[0] = 0;
        
        for (int i = 2; i < size_array; i++) {
            array_a[i] =1;
            
        }    
        for(int i=0; i<=ceil(sqrt(size_array)); i++){
            array_b[i] = i+2;
        }

        MPI_Scatterv(array_a, array_sizes, displacements, MPI_INT, array_a, nDatosLocal, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatterv(array_b, array_sizes, displacements, MPI_INT, array_b, nDatosLocal, MPI_INT, 0, MPI_COMM_WORLD);
    }

    //printf("%d ", nDatosLocal);
    printf("%d", idproc);
    for (int i = 0; i <= nDatosLocal; i++) {
        printf("s%d ", array_b[i]);
        //printf("%d ", array_results[i]);
    }

    

    if (idproc == 0) {
        printf("\nArray resultados: ");
        for (int i = 0; i < size_array; i++) {
            printf("%d ", array_results[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    free(array_a);
    free(array_b);
    free(array_results);
    free(array_sizes);
    free(displacements);
    
    return 0;
}
