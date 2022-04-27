#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include "../Projet.h"

double temps_compute_proof_of_work(int d){ // temps pour compute_proof_of_work
    clock_t ti = clock();

    generate_random_data(NBV,NBC);

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
            
            create_block(&tree, author, d);

            sprintf(name, "Block%d.txt", j);
            add_block(d, name);

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

    CellTree* final_tree = read_tree();

    printf("\nArbre final avec %d zero(s) sur leur Hash:\n", d);
    print_tree(final_tree);

    delete_list_protected(lcp);
    delete_list_keys(lcitoy);
    delete_list_keys(lcand);
    delete_pr_in_tree(tree);
    delete_author_in_tree(tree);
    delete_tree(tree);
    delete_pr_in_tree(final_tree);
    delete_author_in_tree(final_tree);
    delete_tree(final_tree);

    clock_t tf = clock();
    return (double)(tf-ti)/CLOCKS_PER_SEC;
}

int main(){

    FILE* f = fopen("data/time_compute_proof.txt", "w");
    if ( f == NULL ) { // vérifie que fopen se soit bien déroulé
        printf("Le fichier n'a pas pu être ouvert\n");
        exit( 0 );
    }
    
    srand(time(NULL));

    for(int i = 1; i <= 5; i++){
        fprintf(f, "%.8f %d\n", temps_compute_proof_of_work(i), i);
    }

    fclose(f);

    return 0;
}