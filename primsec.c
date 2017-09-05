/*
    Proyecto SO
    Santiago Lizarazo y Dorian Moreno
*/

// Algoritmo usado: Test de primalidad de Miller Rabin

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define ITERATION 5 // Constante para reducir la probabilidad de fallo en el test probabilistico en 1/(4^n)

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

// Main
int main(int argc, char *argv[]){
    // Verificacion de datos - inicio
    char h[]="-h";
    if(argc == 2)
    if(strcmp(argv[1],h) == 0){
        printf("Informacion: El programa guarda los N primeros numeros primos ordenados\nParametros: N(numero limite) y OutputFile(nombre del archivo de salida) respectivamente\n");
        exit(0);
    }

    if(argc<3 || argc >3){
        printf("invocacion erronea, faltan los argumentos.\n");
        exit(-1);
    }

    int j,i,s_limit,limit;
    limit = atoi(argv[1]);
    if(limit<2){
        printf("Limite invalido, debe ser un numero mayor a 1\n");
        exit(-1);
    }

    FILE *f = fopen(argv[2],"w");
    if(f==NULL){
        perror("Error: ");
        exit(-1);
    }
    // Verificacion de datos - final

    for(i=2;i<limit;i++){   // Imprimir en el archivo los primos
        if(Miller(i,ITERATION)) fprintf(f,"%d, ",i);
    }
    fclose(f);
    return 0;
}



