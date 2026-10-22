#include "funciones.h"

int main()
{
    int Puntos;
    FILE *fp_transferencias = AbrirArchivo("transferencias_personales_clean.csv", "rt");
    FILE *fp_paises = AbrirArchivo("paises_continentes.csv", "rt");
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    do{
        printf("Seleccionar punto:\n");
        printf("1.  Totales por país.\n");
        printf("2.  Totales por continente.\n");
        printf("3.  Saldo trimestral por país.\n");
        printf("4.  Totales anuales y variación interanual.\n");
        printf("5.  Ranking de países.\n");
        printf("6.  Participación porcentual por continente.\n");
        printf("7.  Generación del archivo intermedio transferencias_base_larga.csv.\n");
        printf("8.  Promedio trimestral por país.\n");
        printf("9.  Promedio trimestral por continente, operación y año.\n");
        printf("10. Salir.\n");

        scanf("%d", &Puntos);
        system("cls");
        switch(Puntos)
        {
            case 10:
                break;
            case 1:
                printf("\n1.	Totales por país.\n\n");
                Agrupamiento_x_pais(fp_transferencias, fp_paises);
                system("pause");
                system("cls");
                break;
            case 2:
                printf("\n2.	Totales por continente.\n\n");
                clasificacion_Resumen_de_operaciones_por_continente(fp_transferencias, fp_paises);
                system("pause");
                system("cls");
                break;
            default:
                printf("\nValor Invalido, por favor ingrese otro:\n");
                system("pause");
                system("cls");
                break;
        }
    }while(Puntos!=10);


    if(Puntos==10)
        printf("Fin del programa.");

    fclose(fp_transferencias);
    fclose(fp_paises);
    return 0;
}
