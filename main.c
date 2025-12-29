#include <stdio.h>

int est_lettre(int c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s fichier.txt\n", argv[0]);
        return 1;
    }

    FILE* f = fopen(argv[1], "r");
    if (f == NULL) {
        printf("Erreur dans l'ouverture du fichier\n");
        return 1;
    }

    int c;
    int dans_mot = 0;
    int nb_mots = 0;

    while ((c = fgetc(f)) != EOF) {
        if (est_lettre(c)) {
            if (!dans_mot) {
                nb_mots++;
                dans_mot = 1;
            }
        } 
        else {
            dans_mot = 0;
        }
    }

    fclose(f);

    printf("Nombre total de mots : %d\n", nb_mots);
    return 0;
}
