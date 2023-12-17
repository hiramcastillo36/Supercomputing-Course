/*
-Materia: Supercómputo
-Semestre: 5to semestre
-Nombre del alumno: Hiram Jalil Castillo Gutierrez
-Clave del alumno: 0345169
-Carrera: Ingenieria en Sistemas Inteligentes
-Nombre de tarea o programa: MPI: MPI: MPI_TYPE_Struct
-Periodo de evaluación: Parcial 3
-Descripción: Creacion de una estructura con diferentes tipos de campos para almacenar la
informacion de un alumno, y enviarla a los procesos para calcular el promedio de cada uno. 
-Avance logrado (0 a 100%): 100%
-Comentarios adicionales: Fue complicado generar la estructura, ya que no se podia enviar
    la variable promedio con tipo float, por lo que se tuvo que cambiar a int. Y no me dejaba compilar con la funcion MPI_Type_struct, por lo que tuve que cambiar a MPI_Type_create_struct.
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <string.h>

typedef struct {
    int clave;
    int clave_materia;
    int promedio;
    char nombre[255];
    float calificacion[4];
    
} ALUMNO;

char nombres_alumnos[10][255] = {
    "Hiram",
    "Juan",
    "Maria",
    "Jose",
    "Pedro",
    "Ana",
    "Luis",
    "Diana",
    "Carlos",
    "Fernanda"
};

int *create_array(int size) {
    int *array = (int *) malloc(sizeof(int) * size);
    return array;
}

int main(int argc, char *argv[]) {
    int idproc, numproc;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &idproc);
    MPI_Comm_size(MPI_COMM_WORLD, &numproc);

    MPI_Datatype MPI_ALUMNO;
    int tamBloques[5];
    MPI_Aint despBloques[5];
    MPI_Datatype tiposBloques[5];
    
    tamBloques[0] = 1;
    tamBloques[1] = 1;
    tamBloques[2] = 1;
    tamBloques[3] = 255;
    tamBloques[4] = 4;

    despBloques[0] = 0;
    despBloques[1] = sizeof(int)*1;
    despBloques[2] = sizeof(int)*2;
    despBloques[3] = sizeof(int)*2 + sizeof(int)*1;
    despBloques[4] = sizeof(int)*2 + sizeof(int)*1 + sizeof(char)*255;


    tiposBloques[0] = MPI_INT;
    tiposBloques[1] = MPI_INT;
    tiposBloques[2] = MPI_INT;
    tiposBloques[3] = MPI_CHAR;
    tiposBloques[4] = MPI_FLOAT;

    int size_array = 4;
    int suma = 0;
    
    int nDatos = size_array / (numproc - 1);
    int nDatosU = size_array;
    
    int *array_sizes = create_array(numproc+1);
    int *displacements = create_array(numproc+1);

    int aprobados_reprobados[2] = {0, 0};

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
    
    ALUMNO *alumnos = (ALUMNO *) malloc(sizeof(ALUMNO) * size_array);
    ALUMNO *resultados = (ALUMNO *) malloc(sizeof(ALUMNO) * size_array);

    MPI_Type_create_struct(5, tamBloques, despBloques, tiposBloques, &MPI_ALUMNO);
    MPI_Type_commit(&MPI_ALUMNO);

    if (idproc == 0) {
        srand(time(NULL));
        for (int i = 0; i < size_array; i++) {
            for(int j=0; j<4; j++){
                alumnos[i].calificacion[j] = ((float)rand()/(float)(RAND_MAX)) * 100.0;
            }
            alumnos[i].clave = ((float)rand()/(float)(RAND_MAX)) * 100.0;
            alumnos[i].clave_materia = ((float)rand()/(float)(RAND_MAX)) * 100.0;
            strcpy(alumnos[i].nombre, nombres_alumnos[rand() % 9]);
            alumnos[i].promedio = 0; 
        }    
    }
    
    int nDatosLocal = nDatos;
    if(idproc == numproc-1){
        nDatosLocal = nDatosU - (nDatos * (numproc-2));
    }
    
    MPI_Scatterv(alumnos, array_sizes, displacements, MPI_ALUMNO, alumnos, nDatosLocal, MPI_ALUMNO, 0, MPI_COMM_WORLD);
    
    int aprob_repro[2] = {0,0};
    int prom = 0;
    for (int i = 0; i < nDatosLocal; i++) {
        prom = 0;
        for(int j=0; j<4; j++){
            prom += alumnos[i].calificacion[j];
        }
        prom = prom/4;
        alumnos[i].promedio = prom;
        if(prom >= 60){
            aprob_repro[0]++;
        }else{
            aprob_repro[1]++;
        }
    }
        
    MPI_Gatherv(alumnos, nDatosLocal, MPI_ALUMNO, alumnos, array_sizes, displacements, MPI_ALUMNO, 0, MPI_COMM_WORLD);
    MPI_Reduce(aprob_repro, aprobados_reprobados, 2, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (idproc == 0) {
        printf("Array resultados: \n");
        for (int i = 0; i < size_array; i++) {
            float prom = 0;
            printf("(%s, %d, %d, %d) \n Calificaciones ", alumnos[i].nombre, alumnos[i].clave, alumnos[i].promedio, alumnos[i].clave_materia);
            for(int j=0; j<4; j++){
                printf("%f ", alumnos[i].calificacion[j]);
            }
            printf("\n");
        }
        printf("\nAlumnos aprobados y reprobados:");
        printf("%d %d", aprobados_reprobados[0], aprobados_reprobados[1]);
        printf("\n");
    }
    
    MPI_Finalize();    
    return 0;
}