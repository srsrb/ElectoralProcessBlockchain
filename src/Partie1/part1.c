#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "../Projet.h"

// EXERCICE 1

// Implémentation par une méthode naîve
int is_prime_naive(long p){  // teste si les entiers de 3 a p-1 divise p
    if(p == 1){ return 0; }
    for(int i = 3; i < p; i++){
        if(p%i == 0){
            return 0;
        }
    }
    return 1;
}

// Exponentiotion modulaire rapide
long modpow_naive(long a, long m, long n){ // répéte m les opérations: (valeur courante*a)%n
    int res = 1;
    for(int i = 0; i < m; i++){
        res *= a;
        res %= n;
    }
    return res;
}

int modpow(long a, long m, long n){ // méthode avec élévations au carré

    int res = 0;
    if(m == 0){ return 1; }
    if(m == 1 || m == -1){ return a%n; }

    res = modpow(a, m/2, n);
    if(m%2 == 0){ res = res*res%n; }
    else{ res = a*res*res%n; }

    return res;
}

// Implémentation du test de Miller-Rabin
int witness(long a, long b, long d, long p){ // test si a est un témoin de Miller pour p, pour un entier a donné (fontion fournie)
    long x = modpow(a, d, p);
    if(x == 1){
        return 0;
    }
    for(long i = 0; i < b; i++){
        if(x == p-1){
            return 0;
        }
        x = modpow(x , 2, p);
    }
    return 1;
}

long rand_long(long low, long up){ // retourne un entier long généré aléatoirement en low et up inclus (fonction fournie)
    return rand()%(up-low+1)+low;
}

long is_prime_miller(long p, long k){ // renvoie 0 si p n'est pas premier (fonction fournie)
    if(p == 2){
        return 1;
    }
    if(!(p & 1) || p <= 1){ // on verifie que p est impair et different de 1
        return 0;
    }
    long b = 0;
    long d = p-1;
    while(!(d & 1)){ // tant que d n'est pas impair
        d = d/2;
        b = b+1;
    }
    // On genere k valeurs pour a, et on teste si c'est un temoin
    long a;
    int i;
    for(i = 0; i < k; i++){
        a = rand_long(2, p-1);
        if(witness(a, b, d, p)){
            return 0;
        }
    }
    return 1;
}

// Génération de nombres premiers
long random_prime_number(int low_size, int up_size, int k){ // génère un nombre premier d'une taille comprise entre low_size et up_size, effectue le test de Miller k fois
    long a;
    do{ // tant que le long aléatoire ne passe pas le test de Miller k fois on en génère un autre
        a = rand_long(pow(2, low_size), pow(2, up_size)-1);
    } while(!is_prime_miller(a, k));
    return a;
}

// EXERCICE 2

// Génération d'une clé publique et d'une clé secrète
long extended_gcd(long s, long t, long* u, long* v){ // version récursive de l'algorithme d'Euclide étendu (fonction fournie)
    if(t == 0){
        *u = 1;
        *v = 0;
        return s;
    }
    long uPrime, vPrime;
    long gcd = extended_gcd(t, s%t, &uPrime, &vPrime);
    *u = vPrime;
    *v = uPrime-(s/t)*vPrime;
    return gcd;
}

void generate_key_values(long p, long q, long* n, long* s, long* u){ // génère une paire (clé publique, clé secrète) à partir des nombres premiers p et q en suivant le protocole RSA
    long v;
    *n = p*q;
    long t = (p-1)*(q-1);
    do{
        *s = rand_long(0, t-1);
    } while((extended_gcd(*s, t, u, &v) != 1) || ((*s)*(*u)%t != 1));
}

// Chiffrement et déchiffrement de message
long* encrypt(char* chaine, long s, long n){ // convertit chaque caractère de chaine en int puis les chiffre
    int size = strlen(chaine);
    long* crypted = (long*)malloc(sizeof(long)*size);
    for(int i = 0; i < size; i++){
        crypted[i] = modpow((int)chaine[i], s, n);
    }
    return crypted;
}

char* decrypt(long* crypted, int size, long u, long n){ // déchiffre le tableau de long et renvoie la chaine obtenue
    char* chaine = (char*)malloc(sizeof(char)*(size+1));
    chaine[size] = '\0';
    for(int i = 0; i < size; i++){
        chaine[i] = modpow(crypted[i], u, n);
    }
    return chaine;
}


// Fonction de test
void print_long_vector(long* result, int size){ // affiche un tableau de long (fonction fournie)
    printf("Vector: [ ");
    for(int i = 0; i < size; i++){
        printf("%lx ", result[i]);
    }
    printf("]\n");
}