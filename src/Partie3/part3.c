#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../Projet.h"

// EXERCICE 5

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
    if (f == NULL){ // vérifie que fopen se soit bien déroulé
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
    while(fgets(buffer, 100, f)){
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
        printf("(%lx,%lx)\n", temp->data->s_u, temp->data->n);
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
CellProtected* create_cell_protected(Protected* pr){ // alloue et initialise une cellule de liste chaînée contenant une protected
    CellProtected* cp = (CellProtected*)malloc(sizeof(CellProtected));
    cp->data = pr;
    cp->next = NULL;
    return cp;
}

void ajouter_en_tete_cp(CellProtected** cp, Protected* pr){ // ajoute une protected en tête de liste
    CellProtected* cp2 = create_cell_protected(pr);
    cp2->next = *cp;
    *cp = cp2;
}

CellProtected** read_protected(char* txt){ // retourne une liste chaînée contenant toutes les protected du fichier txt
    FILE* f = fopen(txt, "r");
    if ( f == NULL ) { // vérifie que fopen se soit bien déroulé
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
    while(fgets(buffer, 400, f)){
        sscanf(buffer, "Declaration: %s %s %s", key, mess, sgn);
        sprintf(buffer,"%s %s %s", key, mess, sgn);
        pr = str_to_protected(buffer);
        ajouter_en_tete_cp(liste, pr);
    }
    
    fclose(f);
    return liste;
}

void print_list_protected(CellProtected* LCP){ // affiche une liste chaînée de CellProtected
    char* str;
    CellProtected* temp = LCP;
    while(temp){
        str = protected_to_str(temp->data);
        printf("%s\n", str);
        free(str);
        temp = temp->next;
    }
}

void delete_cell_protected(CellProtected* c){ // supprime et libère une cellule d'une liste chaînée de CellPey
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