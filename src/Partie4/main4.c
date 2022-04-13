#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include "../Projet.h"

int main(){
    // Création et initialisation d'une key
    Key* k=(Key*)malloc(sizeof(Key));
    init_key(k, 12345, 54321);
    
    // Création et initialisation d'une protected
    CellProtected** cp = read_protected("data/declarations.txt");

    // TEST HASH_SHA
    // Création de deux valeurs hachées avec la fonction hash_SHA
    char* str = "cgfb";
    unsigned char* prev = hash_SHA(str);
    char* s = "Roseode";
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

    int nonce = 0;

    // Initialisation d'un block
    Block *b = init_block(k, *cp, prev, nonce);
    b->hash = hash;

    // Ecriture d'un block dans data/block.txt
    write_block(b);

    // Librération de la mémoire du block
    delete_block(b);
    free(cp);

    // Lecture d'un block dans data/block.txt
    Block* b2 = read_block("data/block.txt");

    // Passage d'un block à une chaine de charactères
    printf("\nLecture d'une chaine de charactères représentant un block:\n");
    char* btostr = block_to_str(b2);
    printf("%s\n", btostr);

    // Librération de la mémoire du block
    delete_block(b2);
    free(btostr);

    // Création et initialisation d'une key
    Key* k2=(Key*)malloc(sizeof(Key));
    init_key(k2, 12345, 54321);
    
    // Création et initialisation d'une protected
    CellProtected** cp2 = read_protected("data/declarations.txt");
    
    // Création de deux valeurs hachées avec la fonction hash_SHA
    char* str2 = "cgfb";
    unsigned char* prev2 = hash_SHA(str2);

    Block* b3 = init_block(k2, *cp2, prev2, 0);
    compute_proof_of_work(b3, 1);
    char* btostr2 = block_to_str(b3);
    printf("Hash après compute_proof_of_work(): %s\n\n", btostr2);
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++){
        printf("%02x", b3->hash[i]);
    }
    putchar('\n');

    // Test fraude
    if(verify_block(b3, 1)){
        printf("\nCe block est valide.\n");
        write_block(b3);
    }
    else{
        printf("\nCe block n'est pas valide.\n");
    }

    delete_block(b3);
    free(cp2);
    free(btostr2);

    return 0;
}