#include <stdio.h>

int compteCaracteres(FILE *fichier) {
    int compteur = 0;
    while((fgetc(fichier))!= EOF) compteur ++;
    return compteur;
}

int compteLignes(FILE * fichier) {
    int c,compteur = 0;
    while((c=fgetc(fichier))!=EOF){
        if (c== '\n') compteur ++;
    }
    return compteur;
}

int compteMots(FILE * fichier) {
    int c,compteur = 0, danslemot = 0;
    while((c=fgetc(fichier))!= EOF) {
        if (c == ' ' || c=='\t' || c=='\n') {
            danslemot=0;
        } else {
            if(!danslemot) {
                compteur ++;
                danslemot = 1;
            }
        }
    }
    return compteur;
}

int main(int argc, char *argv[]) {
    for (int i = 1; i<argc;i++) {
        FILE *f = fopen(argv[i],"r");
        int car = compteCaracteres(f);
        rewind(f);
        int lignes = compteLignes(f);
        rewind(f);
        int mots = compteMots(f);
        fclose(f);

        printf("%s : %d caractères, %d lignes, %d mots\n",argv[i],car,lignes,mots);
    }
    return 0;
}