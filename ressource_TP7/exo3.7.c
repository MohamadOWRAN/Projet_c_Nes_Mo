#include <stdio.h>

int compteCaracteres(FILE *fichier) {
    int compteur = 0;
    while((fgetc(fichier))!= EOF) compteur ++;
    return compteur;
}

int compteLignes(FILE * fichier) {
    int c,compteur = 1;
    while((c=fgetc(fichier))!=EOF){
        if (c== '\n') compteur ++;
    }
    return compteur;
}

int main(int argc, char *argv[]) {
    for (int i = 1; i<argc;i++) {
        FILE *f = fopen(argv[i],"r");
        int car = compteCaracteres(f);
        rewind(f);
        int lignes = compteLignes(f);
        fclose(f);
        printf("%s : %d caractères, %d lignes\n",argv[i],car,lignes);
    }
    return 0;
}