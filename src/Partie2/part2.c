#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../Projet.h"

// EXERCICE 3

// Manipulation de clés
void init_key(Key* key, long val, long n){ // initialise un clé key déjà allouée
    key->s_u = val;
    key->n = n;
}

void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size){ // initialise une paire de clés déjà allouées
    long p, q;
    p = random_prime_number(low_size, up_size, 256);
    do{ // tant que les nombres premiers ne sont pas différents on en génère un autre
        q = random_prime_number(low_size, up_size, 256);
    } while(p == q);
    long n, s, u;
    generate_key_values(p, q, &n, &s, &u); // génère la valeur d'une paire de clés
    init_key(pKey, s, n);
    init_key(sKey, u, n);
}

char* key_to_str(Key* key){ // passe un type key en chaîne de caractères
    char* s = (char*)malloc(sizeof(char)*256);
    sprintf(s,"(%lx,%lx)", key->s_u, key->n);
    return s;
}

Key *str_to_key(char *str){ // passe une chaîne de caractères en type key
    Key *key = (Key*)malloc(sizeof(Key));
    long val1, val2;
    sscanf(str,"(%lx,%lx)",&val1,&val2);
    init_key(key,val1,val2);
    return key;
}

// Signature
Signature* init_signature(long* content, int size){ // alloue et initialise une signature
    Signature* s = (Signature*)malloc(sizeof(Signature)*size);
    s->size = size;
    s->T = content;
    return s;
}

Signature* sign(char* mess, Key* sKey){ // crée une signature à partir du message mess(déclaration de vote) et de la clé secrète de l'émetteur
    Signature *s = init_signature(encrypt(mess, sKey->s_u, sKey->n), strlen(mess));
    return s;
}

char* signature_to_str(Signature* sgn){ // passe un signature en chaîne de caractères (fonction fournie)
    char* result = (char*)malloc(10*sgn->size*sizeof(char));
    result[0] = '#';
    int pos = 1;
    char buffer[156];
    for(int i = 0; i < sgn->size; i++){
        sprintf(buffer, "%lx", sgn->T[i]);
        for(int j = 0; j < strlen(buffer); j++){
            result[pos] = buffer[j];
            pos++;
        }
        result[pos] = '#';
        pos++;
    }
    result[pos] = '\0';
    result = (char*)realloc(result, (pos+1)*sizeof(char));
    return result;
}

Signature* str_to_signature(char* str){ // passe une chaîne de caractères en type signature (fonction fournie)
    int len = strlen(str);
    long* content = (long*)malloc(sizeof(long)*len);
    int num = 0;
    char buffer[256];
    int pos = 0;
    for(int i = 0; i < len; i++){
        if(str[i] != '#'){
            buffer[pos] = str[i];
            pos++;
        } else{
            if(pos){
                buffer[pos] = '\0';
                sscanf(buffer, "%lx", &(content[num]));
                num++;
                pos = 0;
            }
        }
    }
    content = (long*)realloc(content, num*sizeof(long));
    return init_signature(content, num);
}

// Déclarations signées
Protected *init_protected(Key *pKey, char *mess, Signature *sgn){ // alloue et initialise un protected
    Protected *prt = (Protected*)malloc(sizeof(Protected));
    prt->pKey = pKey;
    prt->mess = mess;
    prt->sgn = sgn;
    return prt;
}

int verify(Protected *pr){ // vérifie une déclaration en décryptant le tableau de la signature et le comparant au mess du protected
    char *mess = decrypt(pr->sgn->T,pr->sgn->size,pr->pKey->s_u,pr->pKey->n);
    if(strcmp(mess,pr->mess) == 0){
        free(mess);
        return 1;
    }
    free(mess);
    return 0;
}

char *protected_to_str(Protected *pr){ // passe du type protected a une chaîne de caractères
    char *res = (char *)malloc(sizeof(char)*256);
    char *key = key_to_str(pr->pKey);
    char *sgn = signature_to_str(pr->sgn);
    sprintf(res,"%s %s %s", key, pr->mess, sgn);
    free(key);
    free(sgn);
    return res;
}

Protected *str_to_protected(char *str){ // passe une chaîne de caractères en type protected
    char key[256];
    char *mess = (char*)malloc(sizeof(char)*256);
    char sgnT[256];
    sscanf(str,"%s %s %s",key,mess,sgnT);
    Key *pKey = str_to_key(key);
    Signature *sgn = str_to_signature(sgnT);
    Protected *pr = init_protected(pKey,mess,sgn);
    return pr;
}

// EXERCICE 4

void generate_random_data(int nv, int nc){ // génère keys.txt, candidates.txt, declarations.txt avec nv citoyens et nc candidats
    // Vérifie que les paramètres sont valables
    if(nv == 0 || nc == 0 || nc > nv) {
        printf("nv et nc doivent être non nuls et nc doit être inférieur ou égal à nv");
        return;
    }
    FILE* keys = fopen("data/keys.txt", "w");
    FILE* candidates = fopen("data/candidates.txt", "w");
    FILE* declarations = fopen("data/declarations.txt", "w");
    
    char* mess;
    char* prtostr;
    Signature* sgn;
    Protected* pr;

    Key** tabpKeys = (Key**)malloc(sizeof(Key*)*nv);
    Key** tabsKeys = (Key**)malloc(sizeof(Key*)*nv);
    Key** tabCandidates = (Key**)malloc(sizeof(Key*)*nc);


    int random, true, j = 0;
    // Génère nv paires de clés représentant des citoyens
    for(int i = 0; i < nv; i++){
        tabpKeys[i] = (Key*)malloc(sizeof(Key));
        tabsKeys[i] = (Key*)malloc(sizeof(Key));
        init_pair_keys(tabpKeys[i], tabsKeys[i], 3, 7);
        fprintf(keys, "Publique: (%lx,%lx) Secrète: (%lx,%lx)\n", tabpKeys[i]->s_u, tabpKeys[i]->n, tabsKeys[i]->s_u, tabsKeys[i]->n);
    }
    // Sélectionne nc clés publiques pour définier nc candidats
    while(j < nc){ // tant qu'on a pas nc candidats
        true = 1;
        random = rand()%nv; // on génère un indice au hasard dans notre tableau de citoyens
        for(int k = 0; k < j; k++){ // on boucle dans notre tableau de candidat
            if(tabCandidates[k]->s_u == tabpKeys[random]->s_u && tabCandidates[k]->n == tabpKeys[random]->n){ // compare le citoyen aux candidats
                true = 0; // la condition passe à false si le citoyen est déjà candidat
                break;
            }  
        }
        if(true){ // le candidat est ajouté seuleument s'il ne l'était pas déjà
            fprintf(candidates, "Publique: (%lx,%lx)\n", tabpKeys[random]->s_u, tabpKeys[random]->n);
            tabCandidates[j] = (Key*)malloc(sizeof(Key));
            init_key(tabCandidates[j], tabpKeys[random]->s_u, tabpKeys[random]->n);
            j++; // on incrémente seulement quand on a ajouté un candidat
        }
    }
    // Génère des déclarations aléatoires
    for(int l = 0; l < nv; l++){ // pour chaque citoyens
        random = rand()%nc;
        mess = key_to_str(tabCandidates[random]);
        sgn = sign(mess, tabsKeys[l]);
        pr = init_protected(tabpKeys[l], mess, sgn);
        prtostr = protected_to_str(pr);
        fprintf(declarations, "Declaration: %s\n", prtostr);
        free(prtostr);
        free(tabsKeys[l]);
        free_protected(pr);
    }

    fclose(keys);
    fclose(candidates);
    fclose(declarations);

    free(tabpKeys);
    free(tabsKeys);
    
    for(int a = 0; a < nc; a++){
        free(tabCandidates[a]);
    }
    free(tabCandidates);
}

// FREE

void free_signature(Signature* sgn){ // libère une signature
    free(sgn->T);
    free(sgn);
}

void free_protected(Protected* pr){ // libère un protected
    free(pr->mess);
    free(pr->pKey);
    free_signature(pr->sgn);
    free(pr);
}