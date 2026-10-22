#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

typedef struct{
    int Columna_ANIO;
    int Columna_TRIMESTRE;
    int Columna_PAIS;
    int Columna_PAIS_DESC;
    int Columna_OPERACION;
    int Columna_MONTO;
}Columnas_Transferencias_personales;

typedef struct{
    int Columna_PAIS_COD;
    int Columna_PAIS_DESC;
    int Columna_CONTINENTE;
}Columnas_Paises_continentes;

typedef struct
{
    char codigo[10];
    char nombre[100];
    char continente[30];

    int registros;
    double total_credito;
    double total_debito;

} Pais;

typedef struct
{
    char nombre[30];
    int registros;
    double total_credito;
    double total_debito;
} Continente;

FILE *AbrirArchivo(char Nombre[], char Modo[]);

/// Ejercicios
//Ejercicio 1
void Agrupamiento_x_pais(FILE *, FILE *);
void clasificacion_Resumen_de_operaciones_por_continente(FILE *fp_transferencias, FILE *fp_paises);

// Genericas
void Ubicar_Columnas_transferencia(char *linea, Columnas_Transferencias_personales *colm);
void Ubicar_Columnas_pais(char *linea, Columnas_Paises_continentes *colm);
int atoiR(const char*);

// Ordenamiento
void ordenarSeleccion(Pais *vec, int ce);
Pais *buscarMayor(Pais *vec, Pais *fin);
void intercambiar(Pais *e1, Pais *e2);
void mostrarVector(Pais *vec, int ce);



#endif // FUNCIONES_H_INCLUDED
