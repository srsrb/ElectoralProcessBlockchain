#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Projet.h"

int main(){
    CellKey** liste = read_public_keys("Partie2/candidates.txt");
    HashTable* t = create_hashtable(*liste,100);
    for(int i=0;i<100;i++){
        if(t->tab[i]){
            printf("");
        }
    }
    delete_hashtable(t);
    delete_list_keys(liste);
    return 0;
}