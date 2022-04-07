// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <time.h>
// #include <openssl/sha.h>
// #include "../Projet.h"

// // EXERCICE 7

// // Lecture et écriture de blocs
// void write_block(Block* b){
//     FILE* f = fopen("Partie4/blocks.txt","w");
//     if ( f == NULL ) { // vérifie que fopen s'est bien déroulé
//         printf( "Le fichier n'a pas pu être ouvert\n");
//         exit( 0 );
//     }
//     fprintf(f,"Block: (%lx,%lx) ",b->author->s_u,b->author->n,);
//     CellProtected* temp = b->votes;
//     while(temp){
//         fprintf(f,"(%lx,%lx) %s %s ", temp->data->pKey->s_u,temp->data->pKey->n, temp->data->mess, temp->data->sgn);
//         temp = temp->next;
//     }
//     fprintf(f,"%d %d %d\n",b->hash,b->previous_hash,b->nonce);
//     fclose(f);
// }

// void read_block(char* txt){
//     FILE* f = fopen(txt, "r");
//     if ( f == NULL ) { // vérifie que fopen s'est bien déroulé
//         printf( "Le fichier n'a pas pu être ouvert\n");
//         exit( 0 );
//     }
//     Key* author;
//     CellProtected* votes;
//     unsigned char hash[256];
//     unsigned char previous_hash[256];
//     int nonce;
//     fscanf;
//     init_block(author, votes, hash, previous_hash, nonce);
//     fclose(f);
// }

// // Création de blocs valides
// char* hash_SHA(char* s){
//     return SHA256(s, strlen(s), 0);
// }



// int main(){
//     char* s = "Rosetta code";
//     unsigned char* d = hash_SHA(s);
//     for(int i=0;i<SHA256_DIGEST_LENGTH;i++){
//         printf("%02x",d[i]);
//     }
//     putchar('\n');
//     return 0;
// }