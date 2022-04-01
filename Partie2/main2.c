#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../Projet.h"

int main(){    
    // Testing Init Keys
    Key* pKey = (Key*)malloc(sizeof(Key));
    Key* sKey = (Key*)malloc(sizeof(Key));
    init_pair_keys(pKey, sKey, 3, 7);
    printf("pKey: %lx, %lx\n", pKey->s_u, pKey->n);
    printf("sKey: %lx, %lx\n", sKey->s_u, sKey->n);

    // Testing Key Serialization
    char* chaine = key_to_str(pKey);
    printf("key_to_str: %s\n", chaine);
    Key* k = str_to_key(chaine);
    printf("str_to_key: (%lx,%lx)\n", k->s_u, k->n);

    // Testing signature
    // Candidate keys:
    Key* pKeyC = (Key*)malloc(sizeof(Key));
    Key* sKeyC = (Key*)malloc(sizeof(Key));
    init_pair_keys(pKeyC, sKeyC, 3, 7);
    // Declaration:
    char* mess = key_to_str(pKeyC);
    char *ktostr = key_to_str(pKey);
    printf("%s vote pour %s\n", ktostr, mess);
    Signature* sgn = sign(mess, sKey);
    printf("Signature: ");
    print_long_vector(sgn->T, sgn->size);
    char *str = signature_to_str(sgn);
    printf("signature_to_str: %s\n", str);
    free(sgn->T);
    free(sgn);
    sgn = str_to_signature(str);
    printf("str_to_signature: ");
    print_long_vector(sgn->T, sgn->size);

    // Testing protected:
    Protected* pr = init_protected(pKey, mess, sgn);
    // Verification:
    if(verify(pr)){
        printf("Signature valide\n");
    } else{
        printf("Signature non valide\n");
    }
    free(chaine);
    chaine = protected_to_str(pr);
    printf("protected_to_str: %s\n", chaine);
    free(pr);
    pr = str_to_protected(chaine);
    char *stostr = signature_to_str(pr->sgn);
    char *ktostr2 = key_to_str(pr->pKey);
    printf("str_to_protected: %s %s %s\n", ktostr2, pr->mess, stostr);

    free(ktostr2);
    free(stostr);
    free_protected(pr);
    free(mess);
    free(str);
    free(ktostr);
    free(pKeyC);
    free(sKeyC);
    free_signature(sgn);
    free(chaine);
    free(pKey);
    free(sKey);
    free(k);

    return 0;
}