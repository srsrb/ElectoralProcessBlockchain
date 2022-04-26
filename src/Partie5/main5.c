#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <openssl/sha.h>
#include "../Projet.h"

int main(){
    // Création et initialisation d'une key
    Key* k=(Key*)malloc(sizeof(Key));
    init_key(k, 12345, 54321);
    
    generate_random_data(5,1);

    // Création et initialisation d'une liste de cellules protected
    CellProtected** cp = read_protected("data/declarations.txt");

    // TEST HASH_SHA
    // Création de deux valeurs hachées avec la fonction hash_SHA
    char* str = "Hello World!";
    unsigned char* prev = hash_SHA(str);
    char* s = "Rosetta code";
    unsigned char* hash = hash_SHA(s);

    printf("Lecture des valeurs hachées avec la fonction hash_SHA\n");
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++){
        printf("%02x", prev[i]);
    }
    putchar('\n');

    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++){
        printf("%02x", hash[i]);
    }
    putchar('\n');

    // Initialisation d'un block
    Block *b = init_block(k, *cp, prev);
    b->hash = hash;

    // Lecture d'un block dans data/block.txt
    Block* b2 = read_block("data/Blockchain/Block1.txt");

    // Passage d'un block à une chaine de charactères
    printf("\nLecture d'une chaine de charactères représentant un block:\n");
    char* btostr = block_to_str(b2);
    printf("%s\n", btostr);

    free(btostr);

    // Création et initialisation d'une key
    Key* k2=(Key*)malloc(sizeof(Key));
    init_key(k2, 12345, 54321);
    
    generate_random_data(7,2);

    // Création et initialisation d'une liste de cellules protected
    CellProtected** cp2 = read_protected("data/declarations.txt");
    
    // Création d'une valeur hachée avec la fonction hash_SHA
    char* str2 = "Previous_Hash";
    unsigned char* prev2 = hash_SHA(str2);

    Block* b3 = init_block(k2, *cp2, prev2);
    compute_proof_of_work(b3, 1);
    char* btostr2 = block_to_str(b3);
    printf("\nHash après compute_proof_of_work(): %s\n\n", btostr2);
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++){
        printf("%02x", b3->hash[i]);
    }
    putchar('\n');

    // Test fraude
    if(verify_block(b3, 1)){
        printf("\nCe block est valide.\n");
        // write_block(b3);
    }
    else{
        printf("\nCe block n'est pas valide.\n");
    }

    putchar('\n');

    //TESTS ARBRES

    CellTree* ct = create_node(b3);
    CellTree* ct2 = create_node(b);
    CellTree* ct3 = create_node(b2);


    addchild(ct,ct2);
    addchild(ct2,ct3);

    print_tree(ct);

    if(highest_child(ct)){
        printf("\nHighest Child: Height: %d, Hash_block: ",highest_child(ct)->height);
        for(int i = 0; i < SHA256_DIGEST_LENGTH; i++){
            printf("%02x", highest_child(ct)->block->hash[i]);
        }
        putchar('\n');
    }

    printf("\nLast node: ");
    print_tree(last_node(ct));

    CellProtected* cp3;
    cp3 = fusion_tree(ct); 

    putchar('\n');
    printf("Fusion des listes chaînées de déclarations contenues dans les blocs de la plus longue chaîne:\n");
    print_list_protected(cp3);
    putchar('\n');


    delete_pr_in_block(b2);
    free(b2->author);

    delete_tree(ct);

    delete_list_protected_nodata(cp3);

    free(k);
    free(k2);
    delete_list_protected(cp);
    delete_list_protected(cp2);
    free(btostr2);


    // SIMULATION PROCESSUS DE VOTE

    Protected* p = str_to_protected("(b5d,1555) (69,9bb) #a9e#de9#272#388#272#47#47#127e#");

    submit_vote(p);

    free_protected(p);

    CellTree* tree = NULL;

    Key* author = (Key*)malloc(sizeof(Key));

    init_key(author, 1000, 265);

    create_block(&tree, author, 1);

    free(author);

    add_block(1, "data/Blockchain/Block1.txt"); // ICI FAUT CHANGER 

    Protected* p2 = str_to_protected("(17f9,1a4b) (821,daf) #16b9#84b#1550#d0#41b#14ae#3e6#189f#fb0#");

    submit_vote(p2);

    free_protected(p2);

    Key* author2 = (Key*)malloc(sizeof(Key));

    init_key(author2, 1, 2);

    create_block(&tree, author2, 1);

    free(author2);

    add_block(1, "name2.txt");

    print_tree(tree);
    putchar('\n');

    delete_pr_in_tree(tree);
    delete_tree(tree);

    // LECTURE DE L'ARBRE ET CALCUL DU GAGNANT
    printf("LECTURE DE L'ARBRE ET CALCUL DU GAGNANT\n\n");

    CellTree* tree2 = read_tree();

    print_tree(tree2);

    delete_author_in_tree(tree2);
    delete_pr_in_tree(tree2);
    delete_tree(tree2);

    return 0;
}