#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include "../Projet.h"

int main(){
    Key* k=(Key*)malloc(sizeof(Key));
    init_key(k,12345,54321);

    CellProtected** cp=read_protected("data/declarations.txt");
    unsigned char hash[256]="938YHS2902";
    unsigned char previous_hash[256]="3UUF47dsa77";

    int nonce = 3;

    Block *b=init_block(k,*cp,(unsigned char *)hash,(unsigned char *)previous_hash,nonce);

    write_block(b);

    delete_block(b);
    free(cp);

    Block* b2 = read_block("data/blocks.txt");

    char* btostr = block_to_str(b2);
    printf("%s\n",btostr);

    delete_block(b2);
    free(btostr);

    return 0;


}