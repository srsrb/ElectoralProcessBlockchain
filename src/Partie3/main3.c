#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../Projet.h"

int main(){
    srand(time(NULL));

    // TEST FCTS CELLKEY
    printf("TEST AJOUT KEY DANS UNE LISTE DE KEY\n");
    CellKey** liste = (CellKey**)malloc(sizeof(CellKey*));
    Key* key = (Key*)malloc(sizeof(Key));
    init_key(key, 10, 5);
    Key* key2 = (Key*)malloc(sizeof(Key));
    init_key(key2, 2, 3);
    *liste = create_cell_key(key);
    ajouter_en_tete_ck(liste, key2);
    CellKey* temp;
    while(*liste){
        printf("%ld %ld\n", (*liste)->data->s_u, (*liste)->data->n);
        temp = (*liste)->next;
        free(*liste);
        *liste = temp;
    }
    free(key);
    free(key2);
    free(liste);

    // GENERATION DE RANDOM KEYS ET DECLARATIONS DANS LE DOSSIER DATA
    generate_random_data(10,2);

    // TEST READ CELLKEYS
    printf("\nTEST READ CELLKEYS\n");
    CellKey** liste2 = read_public_keys("data/keys.txt");
    print_list_keys(*liste2);
    delete_list_keys(liste2);

    // TEST FCTS CELLPROTECTED
    printf("\nTEST AJOUT DANS CELLPROTECTED\n");
    // Initialisation de la liste de cp
    CellProtected** liste3 = (CellProtected**)malloc(sizeof(CellProtected*));
    *liste3 = NULL;
    // Création de 2 keys
    Key* pkey1 = (Key*)malloc(sizeof(Key));
    Key* skey1 = (Key*)malloc(sizeof(Key));
    init_pair_keys(pkey1, skey1, 3, 7);
    // Création d'une signature et d'un protected dans une cell
    //char* mess1 = (char*)malloc(sizeof(char)*256); ?????
    char* mess1 = key_to_str(pkey1);
    Signature* sgn1 = sign(mess1, skey1);
    Protected* pr1 = init_protected(pkey1, mess1, sgn1);
    // Ajout dans liste
    ajouter_en_tete_cp(liste3, pr1);
    // Création de 2 nouvelles keys
    Key* pkey2 = (Key*)malloc(sizeof(Key));
    Key* skey2 = (Key*)malloc(sizeof(Key));
    init_pair_keys(pkey2, skey2, 3, 7);
    // Création d'une signature et d'un protected dans une cell
    char* mess2 = key_to_str(pkey2);
    Signature* sgn2 = sign(mess2, skey2);
    Protected* pr2 = init_protected(pkey2, mess2, sgn2);
    // Ajout dans liste
    ajouter_en_tete_cp(liste3, pr2);
    // Affichage liste
    print_list_protected(*liste3);

    free(skey1);
    free(skey2);
    delete_list_protected(liste3);

    // TEST READ CELLPROTECTED
    printf("\nTEST READ CELLPROTECTED\n");
    CellProtected** liste4 = read_protected("data/declarations.txt");
    print_list_protected(*liste4);

    delete_list_protected(liste4);

    return 0;
}
