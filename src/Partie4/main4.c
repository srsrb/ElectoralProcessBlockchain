#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include "../Projet.h"

int main(){
    CellProtected** liste4 = read_protected("data/declarations.txt");

    // TEST FRAUDES CELLPROTECTED
    printf("\nListe avec possiblement des fraudes (ici il n'y en aura pas car nous avons nous même généré ces déclarations):\n");
    print_list_protected(*liste4);

    verify_LCP(liste4);

    printf("\nListe sans fraude:\n");
    print_list_protected(*liste4);

    delete_list_protected(liste4);

    // CALCUL DU VAINQUEUR DE L'ELECTION
    printf("\nCALCUL DU VAINQUEUR DE L'ELECTION\n");
    CellKey** candidates = read_public_keys("data/candidates.txt");
    CellKey** voters = read_public_keys("data/keys.txt");
    CellProtected** decl = read_protected("data/declarations.txt");
    Key* winner = compute_winner(*decl, *candidates, *voters, NBC, NBV);
    printf("Winner = (%lx,%lx)\n", winner->s_u, winner->n);
    delete_list_keys(candidates);
    delete_list_keys(voters);
    delete_list_protected(decl);

    return 0;
}