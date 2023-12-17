/*
-Materia: Supercómputo
-Semestre: 5to semestre
-Nombre del alumno: Hiram Jalil Castillo Gutierrez
-Clave del alumno: 0345169
-Carrera: Ingenieria en Sistemas Inteligentes
-Nombre de tarea o programa: MPI: MPI_TYPE_Contiguos
-Periodo de evaluación: Parcial 3
-Descripción: Suma de un arreglo utilizando una estructura que contiene X, Y, Z, haciendo uso de las funciones 
MPI_Datatype, MPI_Type_contiguous, MPI_Type_commit
-Avance logrado (0 a 100%): 100%
-Comentarios adicionales: Crear mis propias estructuras tiene muchos beneficios, ya que ahora se puede enviar todo
    tipo de estructuras a los procesos, y personalizar nuestros programas.
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

typedef struct {
    float x;
    float y;
    float z;
} COORDENADA;

int *create_array(int size) {
    int *array = (int *) malloc(sizeof(int) * size);
    return array;
}

int main(int argc, char *argv[]) {
    int idproc, numproc;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &idproc);
    MPI_Comm_size(MPI_COMM_WORLD, &numproc);

    MPI_Datatype MPI_COORDENADA;
    MPI_Type_contiguous(3, MPI_FLOAT, &MPI_COORDENADA);
    MPI_Type_commit(&MPI_COORDENADA);

    int size_array = 3;
    int suma = 0;
    
    int nDatos = size_array / (numproc - 1);
    int nDatosU = size_array;
    
    COORDENADA *array_a = (COORDENADA *) malloc(sizeof(COORDENADA) * size_array);
    COORDENADA *array_b = (COORDENADA *) malloc(sizeof(COORDENADA) * size_array);
    COORDENADA *array_results = (COORDENADA *) malloc(sizeof(COORDENADA) * size_array);
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

    if (idproc == 0) {
        srand(time(NULL));
        for (int i = 0; i < size_array; i++) {
            array_a[i].x = ((float)rand()/(float)(RAND_MAX)) * 100.0;
            array_b[i].x = ((float)rand()/(float)(RAND_MAX)) * 100.0;
            array_a[i].y = ((float)rand()/(float)(RAND_MAX)) * 100.0;
            array_b[i].y = ((float)rand()/(float)(RAND_MAX)) * 100.0;
            array_a[i].z = ((float)rand()/(float)(RAND_MAX)) * 100.0;
            array_b[i].z = ((float)rand()/(float)(RAND_MAX)) * 100.0;  
        }    
        for(int i=0; i<size_array; i++){
            printf("(%.2f %.2f %.2f) ", array_a[i].x, array_a[i].y, array_a[i].z);
        }
        printf("\n");
        for(int i=0; i<size_array; i++){
            printf("(%.2f %.2f %.2f)", array_b[i].x, array_b[i].y, array_b[i].z);
        }
    }
    
    int nDatosLocal = nDatos;
    if(idproc == numproc-1){
        nDatosLocal = nDatosU - (nDatos * (numproc-2));
    }
    
    MPI_Scatterv(array_a, array_sizes, displacements, MPI_COORDENADA, array_a, nDatosLocal, MPI_COORDENADA, 0, MPI_COMM_WORLD);
    MPI_Scatterv(array_b, array_sizes, displacements, MPI_COORDENADA, array_b, nDatosLocal, MPI_COORDENADA, 0, MPI_COMM_WORLD);
    
    for (int i = 0; i < nDatosLocal; i++) {
        array_results[i].x = array_a[i].x + array_b[i].x;
        array_results[i].y = array_a[i].y + array_b[i].y;
        array_results[i].z = array_a[i].z + array_b[i].z;
    }

    MPI_Gatherv(array_results, nDatosLocal, MPI_COORDENADA, array_results, array_sizes, displacements, MPI_COORDENADA, 0, MPI_COMM_WORLD);

    if (idproc == 0) {
        printf("\nArray resultados: ");
        for (int i = 0; i < size_array; i++) {
            printf("(%.2f %.2f %.2f) ", array_results[i].x, array_results[i].y, array_results[i].z);
        }
        printf("\n");
    }

    MPI_Type_free(&MPI_COORDENADA);
    MPI_Finalize();

    free(array_a);
    free(array_b);
    free(array_results);
    free(array_sizes);
    free(displacements);
    
    return 0;
}
