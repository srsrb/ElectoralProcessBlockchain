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
    int i=0;
    while(parcours){
        if(verify(parcours->data)){ // si la signature est valide on continue
            avant = parcours;
            parcours = parcours->next;
            continue;
        }
        if(avant){ // si la cellule n'est pas valide et n'est pas en tête de liste
            i++;
            avant->next = parcours->next;
            delete_cell_protected(parcours);
            parcours = avant->next;
        } else{ // la cellule n'est pas valide et est en tête de liste
            i++;
            temp = parcours->next;
            delete_cell_protected(parcours);
            parcours = temp;
            *liste = parcours;
        }
    }
    printf("%d\n",i);
}

HashCell* create_hashcell(Key* key){ // alloue et initialise une cellule d'une table de hachage contenant une key et une valeur initialisé à 0
    HashCell* HC = (HashCell*)malloc(sizeof(HashCell));
    HC->key = key;
    HC->val = 0;
    return HC;
}

int hash_function(Key* key, int size){ // fonction de hachage
    return (key->s_u*key->n)%size;
}

int find_position(HashTable* t, Key* key){ // Retourne l'indice de la key (si elle n'est pas dedans retourne l'indice ou elle dervra etre) dans le tableau de hachage t
    int pos = hash_function(key,t->size);
    while(t->tab[pos] != NULL){ // à chaque tour de boucle, si la case est vide, alors c'est ici que key doit etre placée
        if(t->tab[pos]->key->s_u != key->s_u || t->tab[pos]->key->n != key->n){ // si la case n'est pas vide mais est déjà occupée par une autre clef, alors on passe à la case suivante
            if(pos == t->size-1){ // si on est à la fin de la table, on revient au début pour trouver une place à key
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

HashTable* create_hashtable(CellKey* keys, int size){  // alloue et initialise une table de hachage via une liste de clefs <= à sa taille
    HashTable* HT = (HashTable*)malloc(sizeof(HashTable));
    HT->size = size;
    HT->tab = (HashCell**)malloc(sizeof(HashCell*)*size);
    for(int i=0;i<size;i++){ // Il faut initiliser toutes les cases de la table à NULL
        HT->tab[i] = NULL;
    }
    int pos;
    while(keys){ // On parcourt la liste keys 
        pos = find_position(HT, keys->data);
        if(!HT->tab[pos]){
            HT->tab[pos] = create_hashcell(keys->data); // Si la clef key->data n'est pas dans la table, alors on la place à l'indice retourné par find_postion // Faut-il dupliquer la clef ?
        }
        keys = keys->next;
    }
    return HT;
}

void delete_hashtable(HashTable* t){ // Supprime et libère la table de hachage et les cellules de hachages (Ici on a pas duppliqué les clefs de la liste keys alors on ne les frees pas, on sera peut etre ammené à changer ça)
    for(int i=0;i<t->size;i++){ // On parcourt la table de hachage t
        free(t->tab[i]); // On libère chaque cellule de hachage // Faut il aussi free les keys qui sont dans la liste CellKeys passée en paramètre de create_hashtable ?
    }
    free(t->tab); // On libère la table
    free(t); // On libère la structure
}

Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
    HashTable* Hc = create_hashtable(candidates,sizeC);
    HashTable* Hv = create_hashtable(voters,sizeV);
    verify_LCP(&decl);
    int posC,posV;
    Key* mess;
    CellProtected* temp = decl;
    while(temp){
        mess = str_to_key(temp->data->mess);
        posC = find_position(Hc,mess);
        posV = find_position(Hv,temp->data->pKey);
        if(Hv->tab[posV]->val == 0 && Hc->tab[posC]->key->s_u == mess->s_u && Hc->tab[posC]->key->n == mess->n){
            Hv->tab[posV]->val++;
            Hc->tab[posC]->val++;
        }
        free(mess);
        temp = temp->next;
    }
    int max = 0;
    int posmax = max;
    for(int i=0;i<Hc->size;i++){
        if(Hc->tab[i] && Hc->tab[i]->val >= max){ //Ici pas >= mais > -> on verra puisque pas écrit
            max = Hc->tab[i]->val;
            posmax = i;
        }
        // else if(Hc->tab[i]->val == max){
        //     //On fait quoi ? C'est pas écrit mdrr
        // }
    }

    CellKey* temp2 = candidates;
    while(temp2){
        if(temp2->data->n == Hc->tab[posmax]->key->n && temp2->data->s_u == Hc->tab[posmax]->key->s_u){
            break;
        }
        temp2 = temp2->next;
    }

    delete_hashtable(Hc);
    delete_hashtable(Hv);

    return temp2->data;
}