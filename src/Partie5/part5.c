#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include <math.h>
#include "../Projet.h"

// EXERCICE 7

// Creation et suppression de blocs
Block* init_block(Key *k, CellProtected *votes, unsigned char *previous_hash, int nonce){ // initialisation par copie
	Block* b=(Block*)malloc(sizeof(Block));
	b->author=k;

    CellProtected* tempv = votes;
    b->votes = NULL;
    while(tempv){
        ajouter_en_tete_cp(&b->votes,tempv->data);
        tempv = tempv->next;
    }
	
    b->hash=NULL;
	b->previous_hash=previous_hash;
	b->nonce=nonce;
	return b;
}

void delete_block(Block *b){ // supprime et libère un block
    delete_list_protected_nodata(b->votes);
    if(b->hash){ free(b->hash); }
    free(b->previous_hash);
	free(b);
}

void delete_pr_in_block(Block* b){
    CellProtected* temp = b->votes;
    while(temp){
        free_protected(temp->data);
        temp = temp->next;
    }
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

    Block* b = init_block(author, votes, previous_hash, nonce);
    b->hash = hash;

    CellProtected* tempv;
    while(votes){
        tempv = votes->next;
        free(votes);
        votes = tempv;
    }
    
    return b;
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
        sprintf(prev_hash, "%02x", b->previous_hash[i]);
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
    final = (char*)realloc(final, (strlen(final)+1)*sizeof(char));

    return final;
}

// Création de blocs valides
unsigned char* hash_SHA(char* s){
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
                sscanf(buffer, "%hhd", hash+num);
                num++;
                pos = 0;
            }
        }
    }
    hash = (unsigned char*)realloc(hash, num*sizeof(unsigned char));
    return hash;
}

void compute_proof_of_work(Block *B, int d){
    int i;
    char* btostr;
    unsigned char* hash;
    char temp[10] = "";
    char htostr[5000] = "";
    char test[256] = "";  
    for(i = 0; i < d ; i++){
        strcat(test, "0");
    }

    while(1){
        strcpy(htostr,"");
        btostr = block_to_str(B);
        hash = hash_SHA(btostr);
        for(i = 0; i < d; i++){
            sprintf(temp, "%02x", hash[i]);
            strcat(htostr, temp);
        }
        strncpy(temp,"",10);
        strncpy(temp, htostr, d);
        // printf("\n%s %s\n", temp, test);
        if(strcmp(temp, test)){
            free(btostr);
            free(hash);
            B->nonce++;
        }
        else{ break; }
    }
    
    free(btostr);
    B->hash = hash;

    return;
}

int verify_block(Block* b, int d){
    int i;
    char temp[10] = "";
    char htostr[5000] = "";
    char* btostr;
    unsigned char* hash;

    btostr = block_to_str(b);
    hash = hash_SHA(btostr);

    char hash_block[256] = "";
    char hash_to_verif[256] = "";

    // Hash to Str:
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++){
        sprintf(temp, "%02x", b->hash[i]);
        strcat(hash_block, temp);
    }

    // Hash to Str:
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++){
        sprintf(temp, "%02x", hash[i]);
        strcat(hash_to_verif, temp);
    }

    if(strcmp(hash_block,hash_to_verif)){
        free(btostr);
        free(hash);
        return 0;
    }

    char test[256] = "";  
    for(i = 0; i < d ; i++){
        strcat(test, "0");
    }
    
    for(i = 0; i < d; i++){
        sprintf(temp, "%02x", hash[i]);
        strcat(htostr, temp);
    }
    
    free(btostr);
    free(hash);
    strncpy(temp,"",10);
    strncpy(temp, htostr, d);
    
    return (!strcmp(temp,test));
}

CellTree* create_node(Block* b){
    CellTree* ct = (CellTree*)malloc(sizeof(CellTree));
    ct->block = b;
    ct->father = NULL;
    ct->firstChild = NULL;
    ct->nextBro = NULL;
    ct->height = 0;
    return ct;
}

int update_height(CellTree* father, CellTree* child){
    int h = father->height;
    father->height = (int)fmax(h,child->height+1);

    return !(h == father->height);
}

void update_height_all(CellTree* father,CellTree* child){
    CellTree* tempfather1 = father;
    CellTree* tempchild1 = child;
    while(tempfather1){
        update_height(tempfather1,tempchild1);
        tempchild1 = tempfather1;
        tempfather1 = tempfather1->father;
    }
}

void addchild(CellTree* father, CellTree* child){
    if(!father->firstChild){
        father->firstChild = child;
        child->father = father;
        update_height_all(father,child);
        return;
    }
    
    CellTree* temp = father->firstChild;
    while(temp->nextBro){
        temp = temp->nextBro;
    }
    temp->nextBro = child;
    child->father = father;
    update_height_all(father,child);
}

void print_tree(CellTree* ct){
    if(ct->nextBro && !ct->firstChild){
        print_tree(ct->nextBro);
    }
    if(ct->firstChild && !ct->nextBro){
        print_tree(ct->firstChild);
    }
    if(ct->firstChild && ct->nextBro){
        print_tree(ct->nextBro);
        print_tree(ct->firstChild);
    }
    printf("Height: %d, Hash_block: ",ct->height);
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++){
        printf("%02x", ct->block->hash[i]);
    }
    putchar('\n');
}

void delete_node(CellTree* node){
    delete_block(node->block);
    free(node);
}

void delete_tree(CellTree* ct){
    if(ct->nextBro && !ct->firstChild){
        delete_tree(ct->nextBro);
    }
    if(ct->firstChild && !ct->nextBro){
        delete_tree(ct->firstChild);
    }
    if(ct->firstChild && ct->nextBro){
        delete_tree(ct->nextBro);
        delete_tree(ct->firstChild);
    }
    delete_node(ct);
}

CellTree* highest_child(CellTree* cell){
    int highest = 0;

    CellTree* temp = cell->firstChild;
    CellTree* highest_child = NULL;
    while(temp){
        if(temp->height >= highest){
            highest_child = temp;
            highest = temp->height;
        }
        temp = temp->nextBro;
    }

    return highest_child;
}

CellTree* last_node(CellTree* tree){
    CellTree* temp = tree;
    while(highest_child(temp)){
        temp = highest_child(temp);
    }
    return temp;
}

CellProtected* fusion_lcp(CellProtected* lcp1, CellProtected* lcp2){
    CellProtected* res = NULL;

    CellProtected* temp = lcp1;
    while(temp){
        ajouter_en_tete_cp(&res, temp->data);
        temp = temp->next;
    }

    temp = lcp2;
    while(temp){
        ajouter_en_tete_cp(&res, temp->data);
        temp = temp->next;
    }
    return res;
}

CellProtected* fusion_tree(CellTree* tree){
    CellProtected* fathervotes = tree->block->votes;
    CellProtected* res;
    CellTree* temp = tree;
    while(highest_child(temp)){
        temp = highest_child(temp);
        res = fusion_lcp(fathervotes, temp->block->votes);
        if(fathervotes != tree->block->votes){delete_list_protected_nodata(fathervotes);}
        fathervotes = res;
    }
    return res;
}