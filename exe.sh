#!/bin/bash

V=valgrind

Opt=--leak-check=full

if [ $# -ne 1 ]
then
    echo "Vous devez donner en argument le numéro de la partie que vous voulez compiler."
    exit
fi

if [ $1 -lt 1 -o $1 -ge 7 ]
then
    echo "Il n'y a seulement que 4 parties exécutables. Entrer un chiffre entre 1 et 6."
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
            $V $Opt ./bin/diff_modpow.bin
            ;;
        6)
            $V $Opt ./bin/time_compute_proof.bin
            ;;
esac