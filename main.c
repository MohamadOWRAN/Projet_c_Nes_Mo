#include <stdio.h>

#define TAILLE 100



typedef struct cellulePoint {
    char *p;
    struct cellulePoint *suivant;
} CellulePoint, *ListeChar ;


CellulePoint* allouerCellule(char *pval){
    CellulePoint* cellule = malloc(sizeof(CellulePoint));

    if (cellule == NULL) return NULL;
    
    cellule->p = strdup(pval);

    if (cellule->p == NULL) {
        free(cellule);
        return NULL;
    }
    
    cellule->suivant = NULL;
    return cellule;
}


void ajouterEnTete(ListeChar *liste, char *val) {
    CellulePoint *c = allouerCellule(val);
    if (c == NULL) return;

    c->suivant = *liste;
    *liste = c;
}


CellulePoint* chercherMot(ListeChar liste, const char *mot) {
    while (liste != NULL) {
        if (strcmp(liste->p, mot) == 0) {
            return liste;
        }
        liste = liste->suivant;
    }
    return NULL;
}

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

    ListeChar l_unique = NULL;
    ListeChar l_tout = NULL;


    char mot[TAILLE];
    int i = 0;

    int c;
    int dans_mot = 0;
    int nb_mots = 0;

    while ((c = fgetc(f)) != EOF) {
        if (est_lettre(c)) {

            if (i < TAILLE_MAX - 1) {
                mot[i++] = (char)c;
            }
            dans_mot = 1;
        }

        else {
            if (dans_mot) {
                mot[i] = '\0';   // fin du mot
                printf("Mot lu : %s\n", mot); // ou sauvegarde
                i = 0;
                dans_mot = 0;
                nb_mots++;
                ajouterEnTete(&l_tout, mot);
                CellulePoint *c = chercherMot(l_unique, mot);
                if (c != NULL){
                    ajouterEnTete(&l_unique, mot);
                }
            }
        }
        
        /* Cas où le fichier se termine par une lettre */
        if (dans_mot) {
            mot[i] = '\0';
            printf("Mot lu : %s\n", mot);
            nb_mots++;
            ajouterEnTete(&l_tout, mot);
            CellulePoint *c = chercherMot(l_unique, mot);
            if (c != NULL){
                ajouterEnTete(&l_unique, mot);
            }
        }
    }

    fclose(f);

    printf("Nombre total de mots : %d\n", nb_mots);
    return 0;
}
