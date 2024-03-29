#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../Projet.h"

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

HashCell* create_hashcell(Key* key){ // alloue et initialise une cellule d'une table de hachage contenant une key et une valeur initialisé à 0
    HashCell* HC = (HashCell*)malloc(sizeof(HashCell));
    HC->key = key;
    HC->val = 0;
    return HC;
}

int hash_function(Key* key, int size){ // retourne la position d'une élément dans la table de hachage
    return (key->s_u*key->n)%size;
}

int find_position(HashTable* t, Key* key){ // retourne la position de key dans la table, si elle n'y est pas, retourne la position où elle aurait dû être
    int pos = hash_function(key, t->size);
    while(t->tab[pos] != NULL){ // à chaque tour de boucle, si la case est vide, alors c'est ici que key doit être placée
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
    for(int i = 0; i < size; i++){ // initialisation des cases de tab à NULL
        HT->tab[i] = NULL;
    }
    int pos;
    while(keys){ // parcours de la liste keys
        pos = find_position(HT, keys->data);
        if(!HT->tab[pos]){
            HT->tab[pos] = create_hashcell(keys->data); // si la clef key->data n'est pas dans la table, alors on la place à l'indice retourné par find_postion // Faut-il dupliquer la clef ?
        }
        keys = keys->next;
    }
    return HT;
}

void delete_hashtable(HashTable* t){ // supprime et libère la table de hachage et les cellules de hachages (Ici on a pas duppliqué les clefs de la liste keys alors on ne les frees pas, on sera peut etre ammené à changer ça)
    for(int i = 0; i < t->size; i++){ // parcours la table de hachage t
        free(t->tab[i]); // on libère chaque cellule de hachage // Faut il aussi free les keys qui sont dans la liste CellKeys passée en paramètre de create_hashtable ?
    }
    free(t->tab); // on libère la table
    free(t); // on libère la structure
}

Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
    HashTable* Hc = create_hashtable(candidates, sizeC);
    HashTable* Hv = create_hashtable(voters, sizeV);
    verify_LCP(&decl);
    int posC, posV;
    Key* mess;
    CellProtected* temp = decl;
    while(temp){
        mess = str_to_key(temp->data->mess);
        posC = find_position(Hc, mess);
        posV = find_position(Hv, temp->data->pKey);
        if(Hc->tab[posC] && Hv->tab[posV] && Hv->tab[posV]->val == 0 && Hc->tab[posC]->key->s_u == mess->s_u && Hc->tab[posC]->key->n == mess->n){
            Hv->tab[posV]->val++;
            Hc->tab[posC]->val++;
        }
        free(mess);
        temp = temp->next;
    }
    int max = 0;
    int posmax = max;
    for(int i = 0; i < Hc->size; i++){
        if(Hc->tab[i] && Hc->tab[i]->val >= max){ //Ici pas >= mais > -> on verra puisque pas écrit
            max = Hc->tab[i]->val;
            posmax = i;
        }
        // else if(Hc->tab[i]->val == max){
        //     // On fait quoi ? C'est pas écrit mdrr
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