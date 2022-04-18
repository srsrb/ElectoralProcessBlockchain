#ifndef PROJET_H
#define PROJET_H

// PARAMETRES

#define NBV 1000
#define NBC 5

// PARTIE 1

int is_prime_naive(long p);

long modpow_naive(long a, long m, long n);

int modpow(long a, long m, long n);

int witness(long a, long b, long d, long p);

long rand_long(long low, long up);

long is_prime_miller(long p, long k);

long random_prime_number(int low_size, int up_size, int k);

long extended_gcd(long s, long t, long* u, long* v);

void generate_key_values(long p, long q, long* n, long* s, long* u);

long* encrypt(char* chaine, long s, long n); // 1 malloc

char* decrypt(long* crypted, int size, long u, long n); // 1 malloc

void print_long_vector(long* result, int size);

// PARTIE 2

typedef struct key{
    long s_u;
    long n;
} Key;

typedef struct signature{
    unsigned int size;
    long *T;
} Signature;

typedef struct protected{
    Key *pKey;
    char *mess;
    Signature *sgn;
} Protected;

void init_key(Key* key, long val, long n);

void init_pair_keys(Key *pKey, Key *sKey, long low_size, long up_size);

char *key_to_str(Key *key); // 1 malloc

Key *str_to_key(char *str); // 1 malloc

Signature* init_signature(long* content, int size); // 1 malloc

Signature* sign(char* mess, Key* sKey); // 1 malloc

char* signature_to_str(Signature* sgn); // 1 malloc

Signature* str_to_signature(char* str); // 1 malloc

Protected *init_protected(Key *pKey, char *mess, Signature *sgn); // 1 malloc

int verify(Protected *pr);

char *protected_to_str(Protected *pr); // 1 malloc

Protected *str_to_protected(char *str); // 4 malloc

void free_signature(Signature* sgn);

void free_protected(Protected* pr);

void generate_random_data(int nv, int nc);

// PARTIE 3

typedef struct cellKey{
    Key* data;
    struct cellKey* next;
} CellKey;

typedef struct cellProtected{
    Protected* data;
    struct cellProtected* next;
} CellProtected;

CellKey* create_cell_key(Key* key); // 1 malloc

void ajouter_en_tete_ck(CellKey** ck, Key*); // 1 malloc

CellKey** read_public_keys(char* txt); // malloc liste cellkey

void print_list_keys(CellKey* LCK);

void delete_cell_key(CellKey* c);

void delete_list_keys(CellKey** lc);

CellProtected* create_cell_protected(Protected* pr); // 1 malloc

void ajouter_en_tete_cp(CellProtected** cp, Protected* pr); // 1 malloc

CellProtected** read_protected(char* txt); // malloc liste cellprotected

void print_list_protected(CellProtected* LCP);

void delete_cell_protected(CellProtected* c);

void delete_list_protected(CellProtected** lp);

void delete_list_protected_nodata(CellProtected* lc);

// PARTIE 4

typedef struct hashcell{
    Key* key;
    int val;
} HashCell;

typedef struct hashtable{
    HashCell** tab;
    int size;
} HashTable;

void verify_LCP(CellProtected** liste);

HashCell* create_hashcell(Key* key); // 1 malloc

int hash_function(Key* key, int size);

int find_position(HashTable* t, Key* key);

HashTable* create_hashtable(CellKey* keys, int size);

void delete_hashtable(HashTable* t);

Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV);

// PARTIE 5

typedef struct block{
    Key* author;
    CellProtected* votes;
    unsigned char* hash;
    unsigned char* previous_hash;
    int nonce;
} Block;

typedef struct block_tree_cell {
    Block * block ;
    struct block_tree_cell * father ;
    struct block_tree_cell * firstChild ;
    struct block_tree_cell * nextBro ;
    int height ;
} CellTree ;

Block* init_block(Key *k, CellProtected *votes, unsigned char *previous_hash);

void delete_block(Block *b);

void delete_pr_in_block(Block* b);

void write_block(Block* b, char* nomfichier);

Block* read_block(char* txt);

char* block_to_str(Block* b);

unsigned char* hash_SHA(char* s);

unsigned char* str_to_hash(char* str);

void compute_proof_of_work(Block *B, int d);

int verify_block(Block* b, int d);

CellTree* create_node(Block* b);

int update_height(CellTree* father, CellTree* child);

void update_height_all(CellTree* father,CellTree* child);

void addchild(CellTree* father, CellTree* child);

void print_tree(CellTree* ct);

void delete_node(CellTree* node);

void delete_pr_in_node(CellTree* node);

void delete_tree(CellTree* ct);

void delete_pr_in_tree(CellTree* ct);

void delete_author_in_tree(CellTree* ct);

CellTree* highest_child(CellTree* cell);

CellTree* last_node(CellTree* tree);

CellProtected* fusion_lcp(CellProtected* lcp1, CellProtected* lcp2);

CellProtected* fusion_tree(CellTree* tree);

void submit_vote(Protected* p);

void create_block(CellTree** tree, Key* author, int d);

void add_block(int d, char* name);

CellTree* read_tree();

Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC, int sizeV);

#endif