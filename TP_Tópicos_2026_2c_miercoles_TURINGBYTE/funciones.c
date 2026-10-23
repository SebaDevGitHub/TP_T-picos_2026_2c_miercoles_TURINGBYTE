#include "funciones.h"

FILE *AbrirArchivo(char Nombre[], char Modo[])
{
    FILE *fp;
    if((fp=fopen(Nombre, Modo))==NULL){
        printf("Error al abrir el archivo\n");
        exit(1);
    }
return fp;
}

void Agrupamiento_x_pais(FILE *fp_transferencias, FILE *fp_paises) /// EJERCICIO 1
{
    rewind(fp_transferencias);
    rewind(fp_paises);

    /// Archivo Paises y Continentes
    Columnas_Paises_continentes EnumeracionPais = {0};
    Columnas_Paises_continentes *colmPais = &EnumeracionPais;

    int Numero_columnaPais;
    char nombre_pais[100];
    char codigo_pais[10];
    char continente[30];
    char lineaPais[500];
    char *tokenPais;

    /// Vector de paises
    Pais paises[250];
    Pais *aux = paises;
    Pais *fin;
    int cantPaises = 0;

    /// Archivo Transferencias
    Columnas_Transferencias_personales Enumeracion = {0};
    Columnas_Transferencias_personales *colm = &Enumeracion;

    int Numero_columna;
    double monto;
    char operacion[2];
    char codigo_pais_transferencia[10];
    char linea[1000];
    char *token;

    /// PASO 1: Ubicar columnas del archivo paises
    if(!fgets(lineaPais, sizeof(lineaPais), fp_paises))
        return;

    Ubicar_Columnas_pais(lineaPais, colmPais);

    /// PASO 2: Cargar todos los paises en el vector
    while(cantPaises < 250 && fgets(lineaPais, sizeof(lineaPais), fp_paises))
    {
        Numero_columnaPais = 0;
        tokenPais = strtok(lineaPais, ";");

        codigo_pais[0] = '\0';
        nombre_pais[0] = '\0';
        continente[0] = '\0';

        while(tokenPais != NULL)
        {
            if(colmPais->Columna_PAIS_COD == Numero_columnaPais)
            {
                if(strlen(tokenPais) < sizeof(codigo_pais))
                    strcpy(codigo_pais, tokenPais);
            }

            if(colmPais->Columna_PAIS_DESC == Numero_columnaPais)
            {
                if(strlen(tokenPais) < sizeof(nombre_pais))
                    strcpy(nombre_pais, tokenPais);
            }

            if(colmPais->Columna_CONTINENTE == Numero_columnaPais)
            {
                if(strlen(tokenPais) < sizeof(continente))
                    strcpy(continente, tokenPais);
            }

            tokenPais = strtok(NULL, ";");
            Numero_columnaPais++;
        }

        /// Guardar los valores en el vector
        strcpy(aux->codigo, codigo_pais);
        strcpy(aux->nombre, nombre_pais);
        strcpy(aux->continente, continente);

        aux->registros = 0;
        aux->total_credito = 0;
        aux->total_debito = 0;

        aux++;
        cantPaises++;
    }

    /// PASO 3: Ubicar columnas de transferencias
    if(!fgets(linea, sizeof(linea), fp_transferencias))
        return;

    Ubicar_Columnas_transferencia(linea, colm);

    /// PASO 4: Recorrer transferencias una sola vez
    while(fgets(linea, sizeof(linea), fp_transferencias))
    {
        Numero_columna = 0;
        token = strtok(linea, ";");

        codigo_pais_transferencia[0] = '\0';
        operacion[0] = '\0';
        monto = 0;

        while(token != NULL)
        {
            if(colm->Columna_PAIS == Numero_columna)
            {
                if(strlen(token) < sizeof(codigo_pais_transferencia))
                    strcpy(codigo_pais_transferencia, token);
            }

            if(colm->Columna_OPERACION == Numero_columna)
            {
                if(strlen(token) < sizeof(operacion))
                    strcpy(operacion, token);
            }

            if(colm->Columna_MONTO == Numero_columna)
            {
                monto = atof(token);
            }

            token = strtok(NULL, ";");
            Numero_columna++;
        }

        /// PASO 5: Buscar el pais en el vector
        aux = paises;
        fin = paises + cantPaises;

        while(aux < fin && strcmp(aux->codigo, codigo_pais_transferencia) != 0)
        {
            aux++;
        }

        /// PASO 6: Acumular resultados
        if(aux < fin)
        {
            aux->registros++;

            if(strcmp(operacion, "C") == 0)
            {
                aux->total_credito += monto;
            }

            if(strcmp(operacion, "D") == 0)
            {
                aux->total_debito += monto;
            }
        }
    }

    /// PASO 7: Ordenar y mostrar resultados
    ordenarSeleccion(paises, cantPaises);
    mostrarVector(paises, cantPaises);
}

void clasificacion_Resumen_de_operaciones_por_continente(FILE *fp_transferencias, FILE *fp_paises) /// EJERCICIO 2
{
    Continente continente[10];
    Continente *auxCont = continente;
    Continente *finCont = continente + 10;
    Continente *finContCargados;

    Continente auxiliar;
    Continente *actual;
    Continente *mayor;
    Continente *siguiente;

    Columnas_Paises_continentes EnumeracionPais;
    Columnas_Paises_continentes *colmPais = &EnumeracionPais;

    Pais paises[250];
    Pais *auxPais = paises;
    Pais *finPais;

    char *separador;
    char *separadorPais;

    Columnas_Transferencias_personales Enumeracion;
    Columnas_Transferencias_personales *colm = &Enumeracion;

    int cantPaises = 0;
    int cantContinentes = 0;
    int Numero_columnaPais;
    int Numero_columna;
//    int i;

    char lineaPais[500];
    char *tokenPais;

    char codigo_pais[10];
    char nombre_pais[100];
    char nombre_continente[30];

    char linea[1000];
    char *token;

    char codigo_pais_transferencia[10];
    char operacion[2];
    double monto;

    rewind(fp_transferencias);
    rewind(fp_paises);

    /// INICIALIZAR VECTOR DE CONTINENTES
    while(auxCont < finCont)
    {
        auxCont->nombre[0] = '\0';
        auxCont->registros = 0;
        auxCont->total_credito = 0;
        auxCont->total_debito = 0;

        auxCont++;
    }

    /// LEER ENCABEZADO DEL ARCHIVO DE PAISES
    if(!fgets(lineaPais, sizeof(lineaPais), fp_paises))
    {
        printf("Error al leer el archivo de paises.\n");
        return;
    }

    Ubicar_Columnas_pais(lineaPais, colmPais);

    /// CARGAR VECTOR DE PAISES
    while(cantPaises < 250 && fgets(lineaPais, sizeof(lineaPais), fp_paises))
    {
        Numero_columnaPais = 0;
        // tokenPais = strtok(lineaPais, ";");
        tokenPais = lineaPais;

        codigo_pais[0] = '\0';
        nombre_pais[0] = '\0';
        nombre_continente[0] = '\0';

        while(tokenPais != NULL)
        {
            separadorPais = strchr(tokenPais, ';');

            if(separadorPais != NULL)
            {
                *separadorPais = '\0';
            }
            if(colmPais->Columna_PAIS_COD == Numero_columnaPais)
            {
                if(strlen(tokenPais) < sizeof(codigo_pais))
                    strcpy(codigo_pais, tokenPais);
            }

            if(colmPais->Columna_PAIS_DESC == Numero_columnaPais)
            {
                if(strlen(tokenPais) < sizeof(nombre_pais))
                    strcpy(nombre_pais, tokenPais);
            }

            if(colmPais->Columna_CONTINENTE == Numero_columnaPais)
            {
                if(strlen(tokenPais) < sizeof(nombre_continente))
                    strcpy(nombre_continente, tokenPais);
            }

            if(separadorPais != NULL)
                tokenPais = separadorPais + 1;
            else
                tokenPais = NULL;

            Numero_columnaPais++;
        }
        /// ELIMINAR SALTOS DE LINEA DEL CONTINENTE
        nombre_continente[strcspn(nombre_continente, "\r\n")] = '\0';

        /// GUARDAR PAIS EN EL VECTOR
        if(nombre_pais[0] != '\0' && nombre_continente[0] != '\0')
        {
            strcpy(auxPais->codigo, codigo_pais);
            strcpy(auxPais->nombre, nombre_pais);
            strcpy(auxPais->continente, nombre_continente);

            auxPais++;
            cantPaises++;
        }
    }

    /// LEER ENCABEZADO DEL ARCHIVO DE TRANSFERENCIAS
    if(!fgets(linea, sizeof(linea), fp_transferencias))
    {
        printf("Error al leer el archivo de transferencias.\n");
        return;
    }

    Ubicar_Columnas_transferencia(linea, colm);

    /// RECORRER TRANSFERENCIAS
    while(fgets(linea, sizeof(linea), fp_transferencias))
    {
        Numero_columna = 0;
        // token = strtok(linea, ";");
        token = linea;

        codigo_pais_transferencia[0] = '\0';
        operacion[0] = '\0';
        monto = 0;

        while(token != NULL)
        {
            separador = strchr(token, ';');

            if(separador != NULL)
            {
                *separador = '\0';
            }
            if(colm->Columna_PAIS == Numero_columna)
            {
                if(strlen(token) < sizeof(codigo_pais_transferencia))
                    strcpy(codigo_pais_transferencia, token);
            }

            if(colm->Columna_OPERACION == Numero_columna)
            {
                if(strlen(token) < sizeof(operacion))
                    strcpy(operacion, token);
            }

            if(colm->Columna_MONTO == Numero_columna)
            {
                monto = atof(token);
            }

            if(separador != NULL)
                token = separador + 1;
            else
                token = NULL;

            Numero_columna++;
        }

        /// BUSCAR PAIS DE LA TRANSFERENCIA
        auxPais = paises;
        finPais = paises + cantPaises;

        while(auxPais < finPais && strcmp(auxPais->codigo, codigo_pais_transferencia) != 0)
        {
            auxPais++;
        }
        if(auxPais == finPais)
        {
            printf("PAIS NO ENCONTRADO: [%s]\n", codigo_pais_transferencia);
        }

        /// SI ENCONTRO EL PAIS
        if(auxPais < finPais)
        {
            /// BUSCAR CONTINENTE
            auxCont = continente;
            finContCargados = continente + cantContinentes;

            while(auxCont < finContCargados && strcmp(auxCont->nombre, auxPais->continente) != 0)
            {
                auxCont++;
            }

            /// SI EL CONTINENTE NO EXISTE, AGREGARLO
            if(auxCont == finContCargados && cantContinentes < 10)
            {
                strcpy(auxCont->nombre, auxPais->continente);
                cantContinentes++;
            }

            /// ACUMULAR DATOS DEL CONTINENTE
            if(auxCont < continente + cantContinentes)
            {
                auxCont->registros++;

                if(strcmp(operacion, "C") == 0)
                {
                    auxCont->total_credito += monto;
                }

                if(strcmp(operacion, "D") == 0)
                {
                    auxCont->total_debito += monto;
                }
            }
        }
    }
    /// ORDENAR CONTINENTES POR TOTAL DE CREDITOS (DESCENDENTE)

    finContCargados = continente + cantContinentes;
    actual = continente;

    while(actual < finContCargados)
    {
        mayor = actual;
        siguiente = actual + 1;

        while(siguiente < finContCargados)
        {
            if(siguiente->total_credito > mayor->total_credito)
            {
                mayor = siguiente;
            }

            siguiente++;
        }

        if(mayor != actual)
        {
            auxiliar = *actual;
            *actual = *mayor;
            *mayor = auxiliar;
        }

        actual++;
    }

    /// MOSTRAR RESUMEN POR CONTINENTE
    printf("\n%-20s %12s %18s %18s\n", "CONTINENTE", "REGISTROS", "CREDITOS", "DEBITOS");

    printf("-----------------------------------------------------------------------\n");

    auxCont = continente;
    finContCargados = continente + cantContinentes;

    while(auxCont < finContCargados)
    {
        printf("%-20s %12d %18.2f %18.2f\n",
               auxCont->nombre,
               auxCont->registros,
               auxCont->total_credito,
               auxCont->total_debito);

        auxCont++;
    }

    printf("\nCantidad de continentes: %d\n", cantContinentes);
}

void Saldo_Trimestral_Por_Pais(FILE *fp_transferencias) /// EJERCICIO 3
{
    int Numero_columna;
    int anio;
    int trimestre;

    char codigo_pais[10];
    char nombre_pais[100];
    char operacion[2];

    double monto;

    double saldo;

    char *token;
    char *separador;

    int j, i;

    anio = 0;
    trimestre = 0;
    monto = 0;

    codigo_pais[0] = '\0';
    nombre_pais[0] = '\0';
    operacion[0] = '\0';

    Columnas_Transferencias_personales Enumeracion;
    Columnas_Transferencias_personales *colm = &Enumeracion;

    PaisTrimestral paises[250];
    int cant_paises = 0;

    int anio_buscado;
    char linea[1000];

    rewind(fp_transferencias);

    printf("Ingrese el anio a consultar: ");
    scanf("%d", &anio_buscado);

    fgets(linea, sizeof(linea), fp_transferencias);
    Ubicar_Columnas_transferencia(linea, colm);

    /* Aca vamos a procesar las transferencias */
    while(fgets(linea, sizeof(linea), fp_transferencias))
    {
        Numero_columna = 0;
        token = linea;

        while(token != NULL)
        {
            separador = strchr(token, ';');

            if(separador != NULL)
            {
                *separador = '\0';
            }

            /*
               Aca vamos a identificar cada columna
               y guardar los datos correspondientes.
            */
            if(Numero_columna == colm->Columna_ANIO)
            {
                anio = atoi(token);
            }

            if(Numero_columna == colm->Columna_TRIMESTRE)
            {
                trimestre = atoi(token);
            }

            if(Numero_columna == colm->Columna_PAIS)
            {
                strcpy(codigo_pais, token);
            }

            if(Numero_columna == colm->Columna_PAIS_DESC)
            {
                strcpy(nombre_pais, token);
            }
            if(Numero_columna == colm->Columna_OPERACION)
            {
                strcpy(operacion, token);
            }

            if(Numero_columna == colm->Columna_MONTO)
            {
                monto = atof(token);
            }

            if(separador != NULL)
            {
                token = separador + 1;
            }
            else
            {
                token = NULL;
            }

            Numero_columna++;
        }
        if(anio == anio_buscado && trimestre >= 1 && trimestre <= 4)
        {
            // printf("Pais: %s | Trimestre: %d | Operacion: %s | Monto: %.2f\n", nombre_pais, trimestre + 1, operacion, monto);
            trimestre--;

            i = 0;

            while(i < cant_paises &&
                strcmp(paises[i].codigo, codigo_pais) != 0)
            {
                i++;
            }
            if(i == cant_paises)
            {
                if(cant_paises < 250)
                {
                    strcpy(paises[i].codigo, codigo_pais);
                    strcpy(paises[i].nombre, nombre_pais);

                    j = 0;

                    while(j < 4)
                    {
                        paises[i].creditos[j] = 0;
                        paises[i].debitos[j] = 0;
                        j++;
                    }

                    cant_paises++;
                }
            }
            if(i < cant_paises)
            {
                if(strcmp(operacion, "C") == 0)
                {
                    paises[i].creditos[trimestre] += monto;
                }
                else if(strcmp(operacion, "D") == 0)
                {
                    paises[i].debitos[trimestre] += monto;
                }
            }
        }
    }

    /// ORDENAR PAISES POR NOMBRE Y MOSTRAR RESULTADOS
    int posMenor;
    PaisTrimestral aux;

    i = 0;

    while(i < cant_paises - 1)
    {
        posMenor = i;
        j = i + 1;

        while(j < cant_paises)
        {
            if(strcmp(paises[j].nombre, paises[posMenor].nombre) < 0)
            {
                posMenor = j;
            }

            j++;
        }

        if(posMenor != i)
        {
            aux = paises[i];
            paises[i] = paises[posMenor];
            paises[posMenor] = aux;
        }

        i++;
    }

    printf("\n%-35s %12s %12s %12s %12s\n","PAIS", "T1", "T2", "T3", "T4");

    i = 0;

    while(i < cant_paises)
    {
        printf("%-35s", paises[i].nombre);

        j = 0;

        while(j < 4)
        {
            saldo = paises[i].creditos[j] - paises[i].debitos[j];

            printf(" %12.2f", saldo);

            j++;
        }

        printf("\n");

        i++;
    }
}

void Ubicar_Columnas_transferencia(char *linea, Columnas_Transferencias_personales *colm)
{
    char *token;
    int Numero_columna=0;

    token = strtok(linea, ";");//el primer token es la palabra antes del espeacio, linea quedaria id\0 whog wper
    while (token != NULL) {
        if(strcmp(token, "ANIO")==0){
            colm->Columna_ANIO = Numero_columna;
        }
        if(strcmp(token, "TRIMESTRE")==0){
            colm->Columna_TRIMESTRE = Numero_columna;
        }
        if(strcmp(token, "PAIS_COD")==0){
            colm->Columna_PAIS = Numero_columna;
        }
        if(strcmp(token, "PAIS_DESC")==0){
            colm->Columna_PAIS_DESC = Numero_columna;
        }
        if(strcmp(token, "OPERACION")==0){
            colm->Columna_OPERACION = Numero_columna;
        }
        if(strcmp(token, "MONTO\n")==0){
            colm->Columna_MONTO = Numero_columna;
        }
        token = strtok(NULL, ";");//aca va null para continuar donde se quedo el anterior string: id\0 ->whog wper
        Numero_columna++;
    }

}

void Ubicar_Columnas_pais(char *linea, Columnas_Paises_continentes *colmPais)
{
    char *token;
    int Numero_columna=0;

    token = strtok(linea, ";");//el primer token es la palabra antes del espeacio, linea quedaria id\0 whog wper
    while (token != NULL) {
        if(strcmp(token, "PAIS_COD")==0){
            colmPais->Columna_PAIS_COD = Numero_columna;
        }
        if(strcmp(token, "PAIS_DESC")==0){
            colmPais->Columna_PAIS_DESC = Numero_columna;
        }
        if(strcmp(token, "CONTINENTE\n")==0){
            colmPais->Columna_CONTINENTE = Numero_columna;
        }
        token = strtok(NULL, ";");//aca va null para continuar donde se quedo el anterior string: id\0 ->whog wper
        Numero_columna++;
    }

}


int atoiR(const char*cad)
{
    int num=0;
    while(*(cad)==' '|| *(cad)=='\t')
    {
        cad++;
    }
    while(*(cad)!='\0'&& (*(cad)<='9'&&*(cad)>='0'))
    {
        num= (num*10) + (*(cad)-'0');// es un -48 ya que 0=48 en ascii, y es la forma para normalizar
        cad++;
    }
    return num;
}


void ordenarSeleccion(Pais *vec, int ce)
{
    Pais *fin = vec + ce - 1;
    Pais *may;

    while(vec < fin)
    {
        may = buscarMayor(vec, fin);

        if(vec != may)
        {
            intercambiar(vec, may);
        }

        vec++;
    }
}

Pais *buscarMayor(Pais *vec, Pais *fin)
{
    Pais *may = vec;

    vec++;

    while(vec <= fin)
    {
        if(vec->total_credito > may->total_credito)
        {
            may = vec;
        }

        vec++;
    }

    return may;
}

void intercambiar(Pais *e1, Pais *e2)
{
    Pais aux = *e1;
    *e1 = *e2;
    *e2 = aux;
}


void mostrarVector(Pais *vec, int ce)
{
    int i;

    for(i = 0; i < ce; i++)
    {
        printf("%-8s | %-40s | %-9d | %-13.2f | %-12.2f\n",
               vec->codigo,
               vec->nombre,
               vec->registros,
               vec->total_credito,
               vec->total_debito);

        vec++;
    }
}
