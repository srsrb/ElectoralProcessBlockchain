#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Projet.h"
#define NBV 100
#define NBC 5

int main(){
    srand(time(NULL));
    generate_random_data(NBV,NBC);
    CellKey** candidates = read_public_keys("Partie2/candidates.txt");
    CellKey** voters = read_public_keys("Partie2/keys.txt");
    CellProtected** decl = read_protected("Partie2/declarations.txt");
    Key* winner = compute_winner(*decl,*candidates,*voters,NBC,NBV);
    printf("Winner = (%lx,%lx)\n",winner->s_u,winner->n);
    delete_list_keys(candidates);
    delete_list_keys(voters);
    delete_list_protected(decl);
}

// int main(){
//     CellProtected** decl = read_protected("Partie2/declarations.txt");
//     verify_LCP(decl);
//     delete_list_protected(decl);
// }