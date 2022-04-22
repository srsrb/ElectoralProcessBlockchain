#!/bin/bash

V=valgrind

Opt=--leak-check=full

if [ $# -ne 1 ]
then
    echo "Vous devez donner en argument le numéro d'une des cinq parties, modpow, compute_proof ou main pour pouvoir l'exécuter."
    exit
fi

if [ $1 == "modpow" ]
then
    $V $Opt ./bin/diff_modpow.bin
    exit
fi

if [ $1 == "compute_proof" ]
then
    $V $Opt ./bin/time_compute_proof.bin
    exit
fi

if [ $1 == "main" ]
then
    $V $Opt ./bin/main.bin
    exit
fi

if [ $1 -lt 1 -o $1 -ge 6 ]
then
    echo "L'argument n'est pas valable, veuillez entrer une valeur entre 1 et 5, modpow, compute_proof ou main."
    exit
fi

case $1 in
        1)
            $V $Opt ./bin/main1.bin
            ;;
        2)
            $V $Opt ./bin/main2.bin
            ;;
        3)
            $V $Opt ./bin/main3.bin
            ;;
        4)
            $V $Opt ./bin/main4.bin
            ;;
        5)
            $V $Opt ./bin/main5.bin
            ;;
esac