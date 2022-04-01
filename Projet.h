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

typedef struct hashcell{
    Key* key;
    int val;
} HashCell;

typedef struct hashtable{
    HashCell** tab;
    int size;
} HashTable;

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

void verify_LCP(CellProtected** liste);

HashCell* create_hashcell(Key* key); // 1 malloc

int hash_function(Key* key, int size);

int find_position(HashTable* t, Key* key);

HashTable* create_hashtable(CellKey* keys, int size);

void delete_hashtable(HashTable* t);

Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV);