#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "../Projet.h"

int main(){
    srand(time(NULL));

    // Generation de cle:
    long p, q;
    p = random_prime_number(3, 7, 5000);
    do{
        q = random_prime_number(3, 7, 5000);
    } while(p == q);
    long n, s, u;
    generate_key_values(p, q, &n, &s, &u);
    // Pour avoir des positives
    if(u < 0){
        long t = (p-1)*(q-1);
        u = u*t; // on aura toujours s*u mod t = 1
    }

    // Affichage des clés en hexadecimal
    printf("cle publique = (%ld, %ld) \n", s, n);
    printf("cle privee = (%ld, %ld) \n", u, n);

    // Chiffrement
    char message[1000] = "Hello World!";
    int len = strlen(message);
    long* crypted = encrypt(message, s, n);

    printf("Initial message: %s \n", message);
    printf("Encoded representation: \n");
    print_long_vector(crypted, len);

    // Dechiffrement
    char* decoded = decrypt(crypted, len, u, n);
    printf("Decoded: %s\n", decoded);

    free(crypted);
    free(decoded);

    return 0;
}