#include <dirent.h>
#include <string.h>
#include <stdio.h>

int main(){
    DIR* rep = opendir("./data/Blockchain");
    if(rep != NULL){
        struct dirent* dir;
        while((dir = readdir(rep))){
            if(strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0){
                printf("Chemin du fichier : ./data/Blockchain/%s taille: %d\n", dir->d_name, dir->d_reclen);
            }
        }
        closedir(rep);
    }
}