#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include "../Projet.h"

// EXERCICE 7

// Creation et suppression de blocs
Block* init_block(Key *k, CellProtected *votes, unsigned char *hash, unsigned char *previous_hash, int nonce){ // initialisation par copie
	Block* b=(Block*)malloc(sizeof(Block));
	b->author=k;
	b->votes=votes;
	b->hash=hash;
	b->previous_hash=previous_hash;
	b->nonce=nonce;
	return b;
}

void delete_block(Block *b){ // supprime et libère un block
    free(b->author);
	CellProtected* temp;
    while(b->votes){
        temp = b->votes->next;
        delete_cell_protected(b->votes);
        b->votes = temp;
    }
    free(b->hash);
    free(b->previous_hash);
	free(b);
}

// Lecture et écriture de blocs
void write_block(Block* b){
    FILE* f = fopen("data/block.txt", "w");
    if ( f == NULL ) { // vérifie que fopen se soit bien déroulé
        printf( "Le fichier n'a pas pu être ouvert\n");
        exit( 0 );
    }
    int i;
    // fprintf(f,"Author: (%lx,%lx)\nHash: %s\nPrevious_hash: %s\nNonce: %d\nListe cp:\n",b->author->s_u,b->author->n,b->hash,b->previous_hash,b->nonce);
    fprintf(f,"Author: (%lx,%lx)\nHash: ",b->author->s_u,b->author->n);
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++){
        fprintf(f, "%d#", b->hash[i]);
    }
    
    fprintf(f,"\nPrevious_hash: ");
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++){
        fprintf(f, "%d#", b->previous_hash[i]);
    }

    fprintf(f,"\nNonce: %d\nListe cp:\n",b->nonce);

    CellProtected* temp = b->votes;
    char* pr;
    while(temp){
        pr = protected_to_str(temp->data);
        fprintf(f,"%s\n",pr);
        temp = temp->next;
        free(pr);
    }
    fclose(f);
}

Block* read_block(char* txt){
    FILE* f = fopen(txt, "r");
    if ( f == NULL ) { // vérifie que fopen se soit bien déroulé
        printf( "Le fichier n'a pas pu être ouvert\n");
        exit( 0 );
    }

    int nonce;
    char key[50];
    char temp[256];
    char buffer[256];

    // fscanf(f,"Author: %s\nHash: %s\nPrevious_hash: %s\nNonce: %d\nListe cp:\n",key, hash, previous_hash, &nonce);
    fgets(buffer,256,f);
    sscanf(buffer,"Author: %s",key);

    fgets(buffer,256,f);
    sscanf(buffer,"Hash: %s",temp);
    unsigned char* hash = str_to_hash(temp);

    fgets(buffer,256,f);
    sscanf(buffer,"Previous_hash: %s",temp);
    unsigned char* previous_hash = str_to_hash(temp);

    fgets(buffer,256,f);
    sscanf(buffer,"Nonce: %d", &nonce);
    fgets(buffer,256,f);

    Key* author = str_to_key(key);

    CellProtected* votes = NULL;
    Protected* pr;
    char mess[50];
    char sgn[50];

    while(fgets(buffer,256,f)){
        sscanf(buffer, "%s %s %s", key, mess, sgn);
        sprintf(buffer,"%s %s %s", key, mess, sgn);
        pr = str_to_protected(buffer);
        ajouter_en_tete_cp(&votes, pr);
    }
    fclose(f);

    return init_block(author, votes, hash, previous_hash, nonce);
}

char* block_to_str(Block* b){
    char buffer[100];
    char nonce[50];
    char* final = (char*)malloc(sizeof(char)*5000);
    char* prtostr;
    char prev_hash[256];

    char* key = key_to_str(b->author);
    strcpy(final,key);
    strcat(final," ");

    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++){
        sprintf(prev_hash, "%d", b->previous_hash[i]);
        strcat(final, prev_hash);
    }

    CellProtected* temp = b->votes;
    while(temp){
        prtostr = protected_to_str(temp->data);
        sprintf(buffer," %s",prtostr);
        free(prtostr);
        strcat(final,buffer);
        temp = temp->next;
    }

    sprintf(nonce," %d",b->nonce);
    strcat(final,nonce);

    free(key);

    return final;
}

// Création de blocs valides
unsigned char* hash_SHA(const char* s){
    unsigned char* res = (unsigned char*)malloc(sizeof(unsigned char)*SHA256_DIGEST_LENGTH);
    unsigned char* str = SHA256((const unsigned char*)s, strlen(s), 0);
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++){
        res[i] = str[i];
    }
    return res;
}

unsigned char* str_to_hash(char* str){
    unsigned char* hash = (unsigned char*)malloc(sizeof(unsigned char)*256);
    char buffer[256];
    int pos = 0, num = 0;
    for(int i = 0; i < strlen(str); i++){
        if(str[i] != '#'){
            buffer[pos] = str[i];
            pos++;
        } else{
            if(pos){
                buffer[pos] = '\0';
                sscanf(buffer, "%d", hash+num);
                num++;
                pos = 0;
            }
        }
    }
    hash = (unsigned char*)realloc(hash, num*sizeof(unsigned char));
    return hash;
}

void compute_proof_of_work(Block *B, int d){

}