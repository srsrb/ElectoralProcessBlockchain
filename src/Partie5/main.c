#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <openssl/sha.h>
#include "../Projet.h"

int main(){
    srand(time(NULL));

    generate_random_data(NBV, NBC);

    CellProtected** lcp = read_protected("data/declarations.txt");
    CellKey** lcand = read_public_keys("data/candidates.txt");
    CellKey** lcitoy = read_public_keys("data/keys.txt");

    CellTree* tree = NULL;

    int i = 0, j = 1;

    CellProtected* temp = *lcp;
    char strkey[25];
    char* str;
    Key* author;
    char name[20];

    while(temp){
        submit_vote(temp->data);
        i++;
        if(i == 10){
            str = protected_to_str(temp->data);
            sscanf(str, "%s", strkey);
            author = str_to_key(strkey);
            
            create_block(&tree, author, 1);

            sprintf(name, "Block%d.txt", j);
            add_block(1, name);

            free(str);
    
            i = 0;
            j++;
        }
        if(!temp->next && i != 0){
            str = protected_to_str(temp->data);
            sscanf(str, "%s", strkey);
            author = str_to_key(strkey);
        }
        temp = temp->next;
    }
    if(i != 0){
        create_block(&tree, author, 1);

        sprintf(name, "Block%d.txt", j);
        add_block(1, name);

        free(str);
    }

    CellTree* final_tree = read_tree();

    printf("Arbre final:\n");
    print_tree(final_tree);

    Key* w = compute_winner_BT(final_tree, *lcand, *lcitoy, NBC, NBV);

    char* winner = key_to_str(w);
    printf("\nGagnant de l'élection: %s\n", winner);

    free(winner);
    delete_list_protected(lcp);
    delete_list_keys(lcitoy);
    delete_list_keys(lcand);
    delete_pr_in_tree(tree);
    delete_author_in_tree(tree);
    delete_tree(tree);
    delete_pr_in_tree(final_tree);
    delete_author_in_tree(final_tree);
    delete_tree(final_tree);

    return 0;
}