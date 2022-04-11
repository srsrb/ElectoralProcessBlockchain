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
    unsigned char* prev = SHA256(str, strlen(str), 0);

    const char* s = "Roseode";
    unsigned char* hash = SHA256(s, strlen(s), 0);

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

    // delete_block(b);
    // free(prev);
    // free(hash);
    // free(cp);

    // Block* b2 = read_block("data/block.txt");

    char* btostr = block_to_str(b);
    printf("%s\n", btostr);

    delete_block(b);
    free(cp);
    free(prev);
    free(hash);
    free(btostr);

    return 0;
}