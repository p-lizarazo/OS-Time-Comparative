/*
    Proyecto SO
    Santiago Lizarazo y Dorian Moreno
*/

// Algoritmo: Criba de eratostenes
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int list [100001]={1,1};

int main(int argc, char *argv[]){
    if(argc<3 || argc >3){
        printf("No hay valores suficientes");
        exit(-1);
    }
    int j,i,s_limit,limit;
    limit = atoi(argv[1]);
    FILE *f = fopen(argv[2],"w");
    s_limit=(int)(sqrt(limit));
    for(i=2;i<=s_limit;i++){
        if(list[i]==0){
            for(j=i;i*j<=limit;j++){
                list[i*j]=1;
            }
        }
    }

    for(i=0;i<=limit;i++){
        if(!list[i]) fprintf(f,"%d, ",i);
    }

    return 0;
}



