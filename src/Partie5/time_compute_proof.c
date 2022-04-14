#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include "../Projet.h"

double temps_compute_proof_of_work(Block* b, int d){ // temps pour compute_proof_of_work
    clock_t ti = clock();
    compute_proof_of_work(b, d);
    clock_t tf = clock();
    return (double)(tf-ti)/CLOCKS_PER_SEC;
}

int main(){

    FILE* f = fopen("data/time_compute_proof.txt", "w");
    if ( f == NULL ) { // vérifie que fopen se soit bien déroulé
        printf("Le fichier n'a pas pu être ouvert\n");
        exit( 0 );
    }
    
    // Création et initialisation d'une key
    Key* k=(Key*)malloc(sizeof(Key));
    init_key(k, 12345, 54321);
    
    // Création et initialisation d'une protected
    CellProtected** cp = read_protected("data/declarations.txt");
    
    char* str = "a";
    unsigned char* prev = hash_SHA(str);

    Block* b = init_block(k, *cp, prev, 0);

    for(int i = 1; i < 6; i++){
        fprintf(f, "%.8f %d\n", temps_compute_proof_of_work(b , i), i);
        if(i != 5){
            free(b->hash);
        }
    }

    free(k);
    delete_block(b);
    delete_list_protected(cp);

    fclose(f);

    return 0;
}