#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


//1.3
long modpow_naive(long a, long m, long n){
    long res = 1;
    for(int i=0; i<m; i++){
        res *= a;
        res = res%n;
    }
    return res;
}

//1.4
int modpow(long a, long m, long n){
    long res;
    if(m == 0){
        return 1;
    }
    if(m == 1){
        return a%n;
    }
    if(m%2 == 0){
        res = modpow(a, m/2, n);
        res = (res*res)%n;
    }
    else{
        res = modpow(a, (long)(m/2), n);
        res = (a*res*res)%n;
    }
    return res;
}

double temps_naive(long a, long m, long n){ // temps pour modpow naive
    clock_t ti = clock();
    modpow_naive(a, m, n);
    clock_t tf = clock();
    return (double)(tf-ti)/CLOCKS_PER_SEC;
}

double temps(long a, long m, long n){ // temps pour modpow
    clock_t ti = clock();
    modpow(a, m, n);
    clock_t tf = clock();
    return (double)(tf-ti)/CLOCKS_PER_SEC;
}

int main(){

    FILE* f = fopen("1-5.txt", "w");

    for(long i = 5000; i < 10000; i += 500){
        fprintf(f,"%.8f %.8f %ld\n", temps(15, i, 5), temps_naive(15, i, 5), i);
    }

    return 0;
}