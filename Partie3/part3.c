#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../Projet.h"

// Liste chaînée de clés
CellKey* create_cell_key(Key* key){ // alloue et initialise une cellule de liste chaînée contenant une key
    CellKey* ck = (CellKey*)malloc(sizeof(CellKey));
    ck->data = key;
    ck->next = NULL;
    return ck;
}

void ajouter_en_tete_ck(CellKey** ck, Key* key){ // ajoute une clé en tête de liste
    CellKey* ck2 = create_cell_key(key);
    ck2->next = *ck;
    *ck = ck2;
}

CellKey** read_public_keys(char* txt){ // retourne une liste chaînée contenant toutes les clés publiques du fichier txt
    FILE* f = fopen(txt, "r");
    if (f == NULL){ // vérifie que fopen s'est bien déroulé
        printf("Le fichier n'a pas pu être ouvert\n");
        exit(0);
    }
    // Alloue la liste
    CellKey** liste = (CellKey**)malloc(sizeof(CellKey*));
    *liste = NULL;
    // Ajoute les clés du fichier à la liste
    Key* key;
    char buffer[100];
    char str[50];
    while(fgets(buffer,100,f)){
        sscanf(buffer, "Publique: %s", str);
        key = str_to_key(str);
        ajouter_en_tete_ck(liste, key);
    }
    
    fclose(f);
    return liste;
}

void print_list_keys(CellKey* LCK){ // affiche une liste chaînée de Cellkey
    CellKey* temp = LCK;
    while(temp){
        printf("%lx %lx\n", temp->data->s_u, temp->data->n);
        temp = temp->next;
    }
}

void delete_cell_key(CellKey* c){ // supprime et libère une cellule d'une liste chaîne de CellKey
    free(c->data);
    free(c);
}

void delete_list_keys(CellKey** lc){ // supprime et libère la liste lc
    CellKey* temp;
    while(*lc){
        temp = (*lc)->next;
        delete_cell_key(*lc);
        *lc = temp;
    }
    free(lc);
}

// Liste chaînée de déclarations signées
CellProtected* create_cell_protected(Protected* pr){ // alloue et initialise une cellule de liste chaînée contenant un protected
    CellProtected* cp = (CellProtected*)malloc(sizeof(CellProtected));
    cp->data = pr;
    cp->next = NULL;
    return cp;
}

void ajouter_en_tete_cp(CellProtected** cp, Protected* pr){ // ajoute un protected en tête de liste
    CellProtected* cp2 = create_cell_protected(pr);
    cp2->next = *cp;
    *cp = cp2;
}

CellProtected** read_protected(char* txt){ // retourne une liste chaînée contenant toutes protected du fichier txt
    FILE* f = fopen(txt, "r");
    if ( f == NULL ) { // vérifie que fopen s'est bien déroulé
        printf( "Le fichier n'a pas pu être ouvert\n");
        exit( 0 );
    }
    // Alloue la liste
    CellProtected** liste = (CellProtected**)malloc(sizeof(CellProtected*));
    *liste = NULL;
    // Ajoute les protected du fichier à la liste
    Protected* pr;
    char buffer[400];
    char key[50];
    char mess[50];
    char sgn[50];
    while(fgets(buffer,100,f)){
        sscanf(buffer, "Declaration: %s %s %s", key, mess, sgn);
        sprintf(buffer,"%s %s %s", key, mess, sgn);
        pr = str_to_protected(buffer);
        ajouter_en_tete_cp(liste, pr);
    }
    
    fclose(f);
    return liste;
}

void print_list_protected(CellProtected* LCP){ // affiche une liste chaîne de CellProtected
    char* str;
    CellProtected* temp = LCP;
    while(temp){
        str = protected_to_str(temp->data);
        printf("%s\n", str);
        free(str);
        temp = temp->next;
    }
}

void delete_cell_protected(CellProtected* c){ // supprime et libère une cellule d'une liste chaîne de CellPey
    free_protected(c->data);
    free(c);
}

void delete_list_protected(CellProtected** lc){ // supprime et libère la liste lc
    CellProtected* temp;
    while(*lc){
        temp = (*lc)->next;
        delete_cell_protected(*lc);
        *lc = temp;
    }
    free(lc);
}

// EXERCICE 6

void verify_LCP(CellProtected** liste){ // supprime de la liste liste les déclarations dont la signature n'est pas valide
    CellProtected* parcours = *liste;
    CellProtected* avant = NULL;
    CellProtected* temp = NULL;
    while(parcours){
        if(verify(parcours->data)){ // si la signature est valide on continue
            avant = parcours;
            parcours = parcours->next;
            continue;
        }
        if(avant){ // si la cellule n'est pas valide et n'est pas en tête de liste
            avant->next = parcours->next;
            delete_cell_protected(parcours);
            parcours = avant->next;
        } else{ // la cellule n'est pas valide et est en tête de liste
            temp = parcours->next;
            delete_cell_protected(parcours);
            parcours = temp;
            *liste = parcours;
        }
    }
}

HashCell* create_hashcell(Key* key){
    HashCell* HC = (HashCell*)malloc(sizeof(HashCell));
    HC->key = key;
    HC->val = 0;
    return HC;
}

int hash_function(Key* key, int size){
    return (key->s_u*key->n)%size;
}

int find_position(HashTable* t, Key* key){
    int pos = hash_function(key,t->size);
    while(t->tab[pos] != NULL){
        if(t->tab[pos]->key != key){
            if(pos == t->size-1){
                pos = 0;
            }
            else{
                pos++;
            }
        }
        else{
            return pos;
        }
    }
    return pos;
}

HashTable* create_hashtable(CellKey* keys, int size){
    HashTable* HT = (HashTable*)malloc(sizeof(HashTable));
    HT->size = size;
    HT->tab = (HashCell**)malloc(sizeof(HashCell*)*size);
    for(int i=0;i<size;i++){
        HT->tab[i] = NULL;
    }
    int pos;
    while(keys){
        pos = find_position(HT, keys->data);
        if(!HT->tab[pos]){
            HT->tab[pos] = create_hashcell(keys->data); // Faut-il dupliquer ????
        }
        keys = keys->next;
    }
    return HT;
}

void delete_hashtable(HashTable* t){
    for(int i=0;i<t->size;i++){
        free(t->tab[i]);
    }
    free(t->tab);
    free(t);
}

Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){}