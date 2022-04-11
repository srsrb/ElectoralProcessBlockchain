#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include "../Projet.h"

int main(){
    Key* k=(Key*)malloc(sizeof(Key));
    init_key(k, 12345, 54321);

    CellProtected** cp = read_protected("data/declarations.txt");
    const char* str = "cgfb";
    unsigned char* prev = hash_SHA(str);

    const char* s = "Roseode";
    unsigned char* hash = SHA256(str, strlen(str), 0);

    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++){
        printf("%02x", prev[i]);
    }
    putchar('\n');

    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++){
        printf("%02x", hash[i]);
    }
    putchar('\n');

    int nonce = 3;

    Block *b = init_block(k, *cp, hash, prev, nonce);

    write_block(b);

    delete_block(b);
    free(cp);

    Block* b2 = read_block("data/block.txt");

    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++){
        printf("%02x", b2->previous_hash[i]);
    }
    putchar('\n');

    //char* btostr = block_to_str(b2);
    //printf("%s\n", btostr);

    delete_block(b2);
    //free(btostr);

    return 0;
}