/*
    Authors: Santiago Lizarazo & Dorian Moreno
    Program name: primHilos
*/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#define ITERATION 5 // Constante para reducir la probabilidad de fallo en el test probabilistico en 1/(4^n)
#define BUFFER 20 // Constante para determinar el tamanio del buffer en un string

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

/*
 * Calcula los primos en un rango y los guarda en un archivo temp para que luego
 * el padre los una
 */
void primes(long long lower,long long upper,long long consecutive){
    char p[BUFFER];
    sprintf(p,"%lld.txt",consecutive);
    long long i;
    FILE *f = fopen(p,"w");
    for(i=lower;i<upper;i++){
        if ( Miller( i, ITERATION))
            fprintf(f, "%lld ",i );
    }
    fclose(f);
}

//Main
int main(int argc,char *argv[])
{
    // Verificacion de datos - inicio
     char h[]="-h";
    if(argc == 2)
    if(strcmp(argv[1],h) == 0){
        printf("Informacion: El programa guarda los N primeros numeros primos ordenados\nParametros: N(numero limite), numProc(numero de procesos) y OutputFile(nombre del archivo de salida) respectivamente\n");
        exit(0);
    }

    if(argc!=4){
        printf("invocacion erronea, faltan los argumentos.\n");
        exit(-1);
    }
    pid_t pid;
    long long limit,nProcess,i,times,lower,upper;
    limit = atoi(argv[1]);
    if(limit<2){
        printf("Limite invalido, debe ser un numero mayor a 1\n");
        exit(-1);
    }
    nProcess = atoi(argv[2]);
     if(nProcess<1){
        printf("Num de procesos invalido, debe ser un numero mayor a 0\n");
        exit(-1);
    }

    FILE *f = fopen(argv[3],"w");
    if(f=NULL){
        perror("Error: ");
        exit(-1);
    }
    // Verificacion de datos - fin
    times=limit/nProcess;

    // Crea los n procesos y les otrga su rango
    for(i=0;i<nProcess;++i){
        lower=times*i;
        upper=times*(i+1);
        if(i==(nProcess-1)){
                upper=limit;
        }
        pid = fork();
        if(pid ==0 ){
            primes(lower,upper,i);
            exit(0);
        }

    }

    // Espera a todos los procesos a que terminen
    while(wait(NULL)!=-1){
        continue;
    }

    char p[BUFFER]; // Arreglo para guardar strings
    long long numTemp; //Guardar num primo del archivo

    // Leer archivo temporales y escribir en el final
    for(i=0;i<nProcess;++i){
        sprintf(p,"%lld.txt",i);
        FILE *ftemp = fopen(p,"r");
        while( fscanf(ftemp,"%lld",&numTemp) != EOF ) {
            fprintf(f, "%lld, ",numTemp );
        }
        fclose(ftemp);
        remove(p);
    }
    fclose(f);
    return 0;
}



