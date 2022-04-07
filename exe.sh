#!/bin/bash

V=valgrind

if [ $# -ne 1 ]
then
    echo "Vous devez donner en argument le numéro de la partie que vous voulez compiler."
    exit
fi

if [ $1 -lt 1 -o $1 -ge 5 ]
then
    echo "Il n'y a seulement que 4 parties exécutables. Entrer un chiffre entre 1 et 4."
    exit
fi

case $1 in
        1)
            $V ./bin/main1.bin
            ;;
        2)
            $V ./bin/main2.bin
            ;;
        3)
            $V ./bin/main3.bin
            ;;
        4)
            $V ./bin/main4.bin
            ;;
esac