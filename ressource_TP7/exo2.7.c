#include <stdio.h>


int compteCaracteres(FILE *fichier) {
    int compteur = 0;
    while((fgetc(fichier))!= EOF) compteur ++;
    return compteur;
}

int main(int argc, char *argv[]) {
    for (int i = 1; i<argc;i++) {
        FILE *f = fopen(argv[i],"r");
        int car = compteCaracteres(f);
        fclose(f);
        printf("%s : %d caractères\n",argv[i],car);
    }
    return 0;
}


