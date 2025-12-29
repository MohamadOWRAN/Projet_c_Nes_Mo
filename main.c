#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

#define TAILLE 100



typedef struct cellulePoint {
    char *p;
    int nb_occu;
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

    cellule->nb_occu = 1;

    cellule->suivant = NULL;
    return cellule;
}


void ajouterEnTete(ListeChar *liste, char *val) {
    CellulePoint *c = allouerCellule(val);
    if (c == NULL) return;

    c->suivant = *liste;
    *liste = c;
}

int len(ListeChar liste) {
    int compteur = 0;
    while (liste != NULL) {
        compteur++;
        liste = liste->suivant;
    }
    return compteur;
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

int compterOccurrences(ListeChar liste, const char *mot) {
    int compteur = 0;
    while (liste != NULL) {
        if (chercherMot(liste, mot) != NULL) {  // cherche dans la sous-liste
            compteur++;
        }
        liste = liste->suivant;
    }
    return compteur;
}

int est_lettre(int c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        return 1;
    }
    return 0;
}

int est_separateur(int c) {
    const char *sep = " \n.,;:!?\'ʼ’";
    return strchr(sep, c) != NULL;
}


void afficherListe(ListeChar liste) {
    if (liste == NULL) {
        printf("La liste est vide.\n");
        return;
    }

    printf("Liste des mots avec occurrences :\n");
    for (ListeChar tmp = liste; tmp != NULL; tmp = tmp->suivant) {
        printf("%s : %d\n", tmp->p, tmp->nb_occu);
    }
}


void afficherNPremiers(ListeChar liste, int n) {
    if (liste == NULL) {
        printf("La liste est vide.\n");
        return;
    }

    printf("Affichage des %d premiers termes :\n", n);
    int compteur = 0;
    for (ListeChar tmp = liste; tmp != NULL && compteur < n; tmp = tmp->suivant) {
        printf("%s : %d\n", tmp->p, tmp->nb_occu);
        compteur++;
    }
}


void libererListe(ListeChar *liste) {
    CellulePoint *tmp;
    while (*liste != NULL) {
        tmp = *liste;
        *liste = (*liste)->suivant;
        free(tmp->p);
        free(tmp);
    }
}


void trierListeDecroissante(ListeChar *liste) {
    if (*liste == NULL) return;

    for (CellulePoint *i = *liste; i != NULL; i = i->suivant) {
        CellulePoint *max = i;
        for (CellulePoint *j = i->suivant; j != NULL; j = j->suivant) {
            if (j->nb_occu > max->nb_occu) {
                max = j;
            }
        }
        // échanger p et nb_occu
        if (max != i) {
            char *tmp_p = i->p;
            int tmp_nb = i->nb_occu;

            i->p = max->p;
            i->nb_occu = max->nb_occu;

            max->p = tmp_p;
            max->nb_occu = tmp_nb;
        }
    }
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

    ListeChar l = NULL;


    char mot[TAILLE];
    int i = 0;

    int c;
    int dans_mot = 0;
    int nb_mots = 0;

    while ((c = fgetc(f)) != EOF) {
        if (est_separateur(c)) {
            if (dans_mot) {
                mot[i] = '\0';   // fin du mot
                //printf("Mot lu : %s\n", mot); // ou sauvegarde
                i = 0;
                dans_mot = 0;
                nb_mots++;
                CellulePoint* cellule = chercherMot(l, mot);
                if (cellule == NULL){
                    ajouterEnTete(&l, mot);
                }
                else{
                    cellule->nb_occu++;
                }
            }
            
        }

        else {
            if (i < TAILLE - 1) {
                mot[i++] = (char)tolower((unsigned char)c);
            }
            dans_mot = 1;
        }
        
    }
    /* Cas où le fichier se termine par une lettre */
    if (dans_mot) {
        mot[i] = '\0';
        //printf("Mot lu : %s\n", mot);
        nb_mots++;
        CellulePoint* cellule = chercherMot(l, mot);
        if (cellule == NULL){
            ajouterEnTete(&l, mot);
        }
        else{
            cellule->nb_occu++;
        }
    }

    fclose(f);

    printf("Nombre total de mots : %d\n", nb_mots);

    trierListeDecroissante(&l);

    afficherNPremiers(l, 5);
    return 0;
}
