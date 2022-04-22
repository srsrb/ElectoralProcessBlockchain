#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../Projet.h"

double temps_naive(long a, long m, long n){ // temps pour modpow naive
    clock_t ti = clock();
    modpow_naive(a, m, n);
    clock_t tf = clock();
    return (double)(tf-ti)/CLOCKS_PER_SEC;
}

double temps(long a, long m, long n){ // temps pour modpow
    clock_t ti = clock();
    modpow(a, m, n);
    clock_t tf = clock();
    return (double)(tf-ti)/CLOCKS_PER_SEC;
}

int main(){
    FILE* f = fopen("data/diff_modpow.txt", "w");
    if ( f == NULL ) { // vérifie que fopen se soit bien déroulé
        printf("Le fichier n'a pas pu être ouvert\n");
        exit( 0 );
    }

    for(long i = 5000; i < 10000; i += 500){
        fprintf(f, "%.8f %.8f %ld\n", temps(15, i, 5), temps_naive(15, i, 5), i);
    }

    fclose(f);

    return 0;
}