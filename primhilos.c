/*
    Proyecto SO
    Santiago Lizarazo y Dorian Moreno
*/

// Algoritmo usado: Test de primalidad de Miller Rabin


#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// Constante para reducir la probabilidad de fallo en el test probabilistico: en 1/(4^n)
#define ITERATION 5

// Arreglo para utilizar memoria compartida para los primos
bool list [100001]={false,false,true};

// estructura para determinar rangos
struct range{
    long long lower;
    long long upper;

};


/*
 * calcula (a * b) % c tomando en cuenta que a*b puede salirse de los limites
 * En particular en este programa se usa para calcular a^2 % mod para verificar test de primalidad
 */

long long mulmod(long long a, long long b, long long mod)
{
    long long x = 0,y = a % mod;
    while (b > 0)
    {
        if (b % 2 == 1)
        {
            x = (x + y) % mod;
        }
        y = (y * 2) % mod;
        b /= 2;
    }
    return x % mod;
}

/*
 * Exponenciacion modular (mas rapida que una exponenciacion normal)
 */
long long modulo(long long base, long long exponent, long long mod)
{
    long long x = 1;
    long long y = base;
    while (exponent > 0)
    {
        if (exponent % 2 == 1)
            x = (x * y) % mod;
        y = (y * y) % mod;
        exponent = exponent / 2;
    }
    return x % mod;
}

/*
 * Test de primalidad Miller-Rabin , la iteración significa la probabilidad de fallo
 */
int Miller(long long p,int iteration)
{

    int i;
    long long s;
    if (p < 2)
    {
        return 0;
    }
    if (p != 2 && p % 2==0)
    {
        return 0;
    }
     s = p - 1;
    while (s % 2 == 0)
    {
        s /= 2;
    }
    for (i = 0; i < iteration; i++)
    {
        long long a = rand() % (p - 1) + 1, temp = s;
        long long mod = modulo(a, temp, p);
        while (temp != p - 1 && mod != 1 && mod != p - 1)
        {
            mod = mulmod(mod, mod, p);
            temp *= 2;
        }
        if (mod != p - 1 && temp % 2 == 0)
        {
            return 0;
        }
    }
    return 1;
}

// Se encarga de buscar los primos en un rango dado
void* primes(void* params){
    struct range *r = params;
    long long i;
    for(i=(r->lower);i<(r->upper);i++){
        if ( Miller( i, ITERATION))
            list[i]=true;
    }
    return NULL;
}




//Main
int main(int argc,char *argv[])
{
    // Verificacion de datos - inicio
    char h[]="-h";
    if(argc == 2)
    if(strcmp(argv[1],h) == 0){
        printf("Informacion: El programa guarda los N primeros numeros primos ordenados\nParametros: N(numero limite), numHilos(numero de hilos) y OutputFile(nombre del archivo de salida) respectivamente\n");
        exit(0);
    }

    if(argc!=4){
        printf("invocacion erronea, faltan los argumentos.\n");
        exit(-1);
    }

    long long limit,nThreads,i,times;
    limit = atoi(argv[1]);
    if(limit<2){
        printf("Limite invalido, debe ser un numero mayor a 1\n");
        exit(-1);
    }
    nThreads = atoi(argv[2]);
    if(nThreads<1){
        printf("Num hilos invalido, debe ser un numero mayor a 0\n");
        exit(-1);
    }

    pthread_t pthreads[nThreads];
    FILE *f = fopen(argv[3],"w");
    if(f=NULL){
        perror("Error: ");
        exit(-1);
    }
    // Verificacion de datos - final
    times=limit/nThreads;


    // Crea los nHilos y les otorga su rango para cacular primos
    for(i=0;i<nThreads;++i){
        struct range* param=malloc(sizeof(struct range));
        param->lower=times*i;
        param->upper=times*(i+1);
        if(i==(nThreads-1)){
            param->upper=limit;
        };
        pthread_create(&pthreads[i], NULL, primes, param);
    }
    // Espera por todos los hilos
    for(i = 0; i < nThreads; i++) {
     pthread_join( pthreads[i], NULL);
    }
    // Guarda todos los primos en el archivo
    for(i=2;i<=limit;i++){
        if ( list[i] )
            fprintf(f,"%lld, ", i);
    }

    fclose(f);
    return 0;
}



