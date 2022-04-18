#!/bin/bash

V=valgrind

Opt=--leak-check=full

if [ $# -ne 1 ]
then
    echo "Vous devez donner en argument le numéro de la partie (ou modpow / compute_proof) que vous voulez exécuter."
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
    echo "Il n'y a seulement que 5 parties exécutables. Donner en argument un chiffre entre 1 et 5, modpow ou compute_proof."
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