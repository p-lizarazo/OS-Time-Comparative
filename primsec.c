/*
    Proyecto SO
    Santiago Lizarazo y Dorian Moreno
*/

// Algoritmo usado: Criba de eratostenes

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
int list [100001]={1,1}; // Lista de booleanos para definir si un num es primo o no

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

    s_limit=(int)(sqrt(limit)); // Limite hasta la raiz del limite ya que no debe verificarse mas Ej( con 49 va hasta el 7)
    for(i=2;i<=s_limit;i++){    // Agarrar el primer numero, considerado primo y tachar el resto hasta s_limit
        if(list[i]==0){
            for(j=i;i*j<=limit;j++){
                list[i*j]=1;
            }
        }
    }

    for(i=0;i<limit;i++){   // Imprimir en el archivo los primos
        if(!list[i]) fprintf(f,"%d, ",i);
    }
    fclose(f);
    return 0;
}



