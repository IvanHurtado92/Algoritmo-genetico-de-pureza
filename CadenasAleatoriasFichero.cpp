#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <string>

#define TAMBITS 10
#define POBLACION 10

float probCruza = .5;
int numevaluaciones = 0;
int generacion = 1;
int numGeneraciones = 250;
int probMutacion = .5;
int generacionesCorridas[1000];

using std::endl;
using std::ifstream;
using std::ios;
using std::ofstream;
using std::string;

struct individuo
{
    int cromosoma[TAMBITS];
    int cantidadCeros = 0;
};

void ImprimirPoblacion(struct individuo *poblacionActual, int tamanio);
void Inicializar_Poblacion(struct individuo *poblacionActual);
void LlenarCantidadCeros(struct individuo *poblacionActual, int tamanio);
void Cruzamiento(struct individuo *poblacionActual);
void SeleccionarMejor(struct individuo *poblacionActual);
void OrdenarPoblacion(struct individuo *poblacionActual, int tamanio);
bool CondicionDeParo(struct individuo *poblacionActual, int cantidadPerfectos);
void Mutacion(struct individuo *poblacionactual, int tamanio, int tamanioBits);
void inicializarFichero(string nombreFichero);
void imprimirPoblacionFichero(string nombreFichero, struct individuo *poblacionActual, int generacion);
void imprimirEjecuciones(string nombreFichero, int corrida);
void imprimirEstadisticas(string nombreFichero, int generacionesEjecucion[], int tam);

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    struct individuo poblacionActual[POBLACION];
    string nombreFichero = "ResumenEjecuciones.txt";
    string nombreFicheroEstadistico = "PromedioEjecuciones.txt";

    inicializarFichero(nombreFichero);
    inicializarFichero(nombreFicheroEstadistico);
    for (int l = 0; l < 1000; l++)
    {
        imprimirEjecuciones(nombreFichero, l + 1);
        generacion = 1;
        Inicializar_Poblacion(poblacionActual);
        imprimirPoblacionFichero(nombreFichero, poblacionActual, generacion);
        // imprimo la población inicial de la primer ejecución
        if (l == 0)
            imprimirPoblacionFichero(nombreFicheroEstadistico, poblacionActual, generacion);
        do
        {
            if (generacion != 1)
            {
                Cruzamiento(poblacionActual);
                OrdenarPoblacion(poblacionActual, POBLACION);
                Mutacion(poblacionActual, POBLACION, TAMBITS);
            }
            generacion++;
        } while (!CondicionDeParo(poblacionActual, TAMBITS) && generacion <= numGeneraciones);
        imprimirPoblacionFichero(nombreFichero, poblacionActual, generacion);
        printf("Finalicé la corrida: %d", l + 1);
        generacionesCorridas[l] = generacion;
    }
    imprimirPoblacionFichero(nombreFicheroEstadistico, poblacionActual, generacion);
    double promedio = 0;
    for (int i = 0; i < 1000; i++)
        promedio += generacionesCorridas[i];
    promedio /= 1000;

    ofstream f;
    f.open(nombreFicheroEstadistico, ios::app);
    f << "El promedio de generaciones para encontrar una poblacion perfecta es: " << promedio<<endl;
    f.close();

    imprimirEstadisticas(nombreFicheroEstadistico, generacionesCorridas, 1000);

    return 0;
}

void ImprimirPoblacion(struct individuo *poblacionActual, int tamanio)
{
    for (int i = 0; i < tamanio; i++)
    {
        printf("Individuo %d: ", i + 1);
        printf("{");
        for (int j = 0; j < TAMBITS; j++)
        {
            printf("%d ", poblacionActual[i].cromosoma[j]);
        }
        printf("}");
        printf("\n");
    }
}

void Inicializar_Poblacion(struct individuo *poblacionActual)
{
    int i, j;

    for (i = 0; i < POBLACION; i++)
    {
        for (j = 0; j < TAMBITS; j++)
        {
            if ((rand() % 100) + 1 <= 50)
            {
                poblacionActual[i].cromosoma[j] = 1;
            }
            else
            {
                poblacionActual[i].cromosoma[j] = 0;
            }
        }
    }
    LlenarCantidadCeros(poblacionActual, POBLACION);
}

void LlenarCantidadCeros(struct individuo *poblacionActual, int tamanio)
{
    int i, j;
    int ceros; /*------VARIABLE CONTABLE DE LOS CEROS-----*/

    for (i = 0; i < tamanio; i++)
    {
        ceros = 0;
        for (j = 0; j < TAMBITS; j++)
        {
            if (poblacionActual[i].cromosoma[j] == 0)
                ceros++;
        }
        poblacionActual[i].cantidadCeros = ceros;
        numevaluaciones++;
    }
}

void Cruzamiento(struct individuo *poblacionActual)
{
    // generar una nueva poblaciOn por cruzamiento
    int puntoCruza;
    int i, j;
    int const cantidadDePadres = 6;
    int padres[cantidadDePadres];
    bool esIgual = false;
    individuo poblacionOrdenada[cantidadDePadres * 2];
    individuo temporal;
    j = 0;
    do
    {
        for (int i = 0; i < cantidadDePadres; i++)
        {
            do
            {
                // GENERO 6 NÚMEROS ALEATORIOS
                esIgual = false;
                padres[i] = rand() % POBLACION;
                for (int l = 0; l < i; l++)
                {
                    if (padres[i] == padres[l])
                    {
                        esIgual = true;
                        break;
                    }
                }
            } while (esIgual);
            poblacionOrdenada[i] = poblacionActual[padres[i]];
            poblacionOrdenada[i + cantidadDePadres] = poblacionActual[padres[i]];
            // FIN DE GENERAR 6 NÚMEROS ALEATORIOS
        }
        // invierto los padres (población temporal) con los hijos (población actual)
        if ((double)((rand() % 100) + 1) / 100 < probCruza)
        {
            // aquí creo a los hijos
            puntoCruza = rand() % (TAMBITS - 1) + 2;
            for (int k = 0; k < cantidadDePadres; k += 2)
            {
                for (i = puntoCruza; i < TAMBITS; i++)
                {
                    // Combino en pares la copia del primer padre (par de padres que se aparean) con el segundo padre
                    poblacionOrdenada[k + cantidadDePadres].cromosoma[i] = poblacionActual[padres[k + 1]].cromosoma[i];
                    poblacionOrdenada[k + cantidadDePadres + 1].cromosoma[i] = poblacionActual[padres[k]].cromosoma[i];
                }
            }
            LlenarCantidadCeros(poblacionOrdenada, cantidadDePadres * 2);
            // Ordeno y asigno a los mejores seis hijos o padres
            OrdenarPoblacion(poblacionOrdenada, cantidadDePadres * 2);
            for (int k = 0; k < cantidadDePadres; k++)
            {
                poblacionActual[padres[k]] = poblacionOrdenada[k];
            }
        }
        j += cantidadDePadres;
    } while (j < (POBLACION / cantidadDePadres - 1));
    LlenarCantidadCeros(poblacionActual, POBLACION);
} // fin de Cruzamiento

void CopiarPoblaciones(struct individuo *poblacionActual, struct individuo *poblacionTemporal)
{
    for (int i = 0; i < POBLACION; i++)
    {
        poblacionTemporal[i] = poblacionActual[i];
    }
}

void SeleccionarMejor(struct individuo *poblacionActual)
{
    struct individuo mejor = poblacionActual[0];
    struct individuo tmp;
    int tindice;

    for (int i = 0; i < POBLACION; i++)
    {
        if (mejor.cantidadCeros > poblacionActual[i].cantidadCeros)
        {
            mejor = poblacionActual[i];
            tindice = i;
        }
    }
    tmp = poblacionActual[0];
    poblacionActual[0] = mejor;
    poblacionActual[tindice] = tmp;

    mejor = poblacionActual[1];
    for (int i = 1; i < POBLACION; i++)
    {
        if (mejor.cantidadCeros > poblacionActual[i].cantidadCeros)
        {
            mejor = poblacionActual[i];
            tindice = i;
        }
    }
}

int Generar6Aleatorios(int rango)
{
    int numeros[6];
    bool esIgual = false;
    for (int i = 0; i < 6; i++)
    {
        do
        {
            numeros[i] = rand() % rango;
            for (int j = 0; j < i; j++)
            {
                if (numeros[i] == numeros[j])
                {
                    esIgual = true;
                    break;
                }
            }
        } while (esIgual);
        esIgual = false;
    }
}

void OrdenarPoblacion(struct individuo *poblacionActual, int tamanio)
{
    individuo temporal;
    for (int i = 0; i < tamanio; i++)
    {
        for (int k = i; k < tamanio; k++)
        {
            if (poblacionActual[i].cantidadCeros > poblacionActual[k].cantidadCeros)
            {
                temporal = poblacionActual[i];
                poblacionActual[i] = poblacionActual[k];
                poblacionActual[k] = temporal;
            }
        }
    }
}

bool CondicionDeParo(struct individuo *poblacionActual, int cantidadPerfectos)
{
    int individuosPerfectos = 0;
    for (int i = 0; i < POBLACION; i++)
    {
        if (poblacionActual[i].cantidadCeros == 0)
        {
            individuosPerfectos++;
        }
    }

    if (individuosPerfectos >= cantidadPerfectos)
    {
        return true;
    }
    return false;
}

void Mutacion(struct individuo *poblacionactual, int tamanio, int tamanioBits)
{
    if (CondicionDeParo(poblacionactual, tamanioBits - 2))
        return;
    int const pobAMutar = 2;
    int individuos[pobAMutar];
    bool EsIgual = false;
    int bitsAMutar = 0;
    int bitPos[4];
    // determino la cantidad de bits a mutar
    bitsAMutar = rand() % 4 + 1;
    for (int i = 0; i < pobAMutar; i++)
    {
        do
        {
            EsIgual = false;
            individuos[i] = rand() % 5 + 5;
            for (int k = 0; k < i; k++)
            {
                if (individuos[k] == individuos[i])
                    EsIgual = true;
            }
        } while (EsIgual || poblacionactual[individuos[i]].cantidadCeros == 0);
        // saco los individuos random que van a mutar
        for (int j = 0; j < bitsAMutar; j++)
        {
            do
            {
                EsIgual = false;
                bitPos[j] = rand() % tamanioBits;
                for (int k = 0; k < j; k++)
                {
                    if (bitPos[k] == bitPos[j])
                        EsIgual = true;
                }
            } while (EsIgual);
            if (poblacionactual[individuos[i]].cromosoma[bitPos[j]] == 0)
                poblacionactual[individuos[i]].cromosoma[bitPos[j]] = 1;
            else
                poblacionactual[individuos[i]].cromosoma[bitPos[j]] = 0;
        }
    }
    LlenarCantidadCeros(poblacionactual, tamanio);
}

void imprimirPoblacionFichero(string nombreFichero, struct individuo *poblacion, int generacion)
{
    ofstream f;
    f.open(nombreFichero, ios::app);
    if (generacion == 1)
    {
        f << "Población Inicial:"
          << "\t\t  Conteo de ceros \n";
    }
    else
    {
        f << "Generacion: " << generacion << "\t\t  Conteo de ceros \n";
    }
    for (int i = 0; i < 10; i++)
    {
        f << endl;
        for (int j = 0; j < 10; j++)
        {
            f << "\t" << poblacion[i].cromosoma[j];
        }
        f << "\t\t" << poblacion[i].cantidadCeros;
    }
    f << endl;
    f.close();
}

void inicializarFichero(string nombreFichero)
{
    ofstream fichero;
    fichero.open(nombreFichero, ios::out);
    fichero << "";
    fichero.close();
}

void imprimirEjecuciones(string nombreFichero, int corrida)
{
    ofstream f;
    f.open(nombreFichero, ios::app);
    f << "Ejecucion: " << corrida << endl;
    f.close();
}
void imprimirEstadisticas(string nombreFichero, int generacionesEjecucion[], int tam)
{
    ofstream f;
    f.open(nombreFichero, ios::app);
    f << "Ejecucion\t\tGeneraciones" << endl;
    for (int i = 0; i < tam; i++)
    {
        f << "  " << i + 1 << "\t\t  " << generacionesEjecucion[i] << endl;
    }
    f.close();
}