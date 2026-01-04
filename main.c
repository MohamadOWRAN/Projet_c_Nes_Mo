#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <time.h>

#define TAILLE 100



typedef struct CelluleVar {
    char *valeur;
    int nb_occu;
    struct CelluleVar *suivant;
} CelluleVar, *ListeVar ;


CelluleVar* allouerCellule(char *pval){
    CelluleVar* cellule = malloc(sizeof(CelluleVar));

    if (cellule == NULL) return NULL;
    
    cellule->valeur = strdup(pval);

    if (cellule->valeur == NULL) {
        free(cellule);
        return NULL;
    }
    
    cellule->nb_occu = 1;
 
    cellule->suivant = NULL;
    return cellule;
}


void ajouterEnTete(ListeVar *liste, char *val) {
    CelluleVar *c = allouerCellule(val);
    if (c == NULL) return;

    c->suivant = *liste;
    *liste = c;
}

int len(ListeVar liste) {
    int compteur = 0;
    while (liste != NULL) {
        compteur++;
        liste = liste->suivant;
    }
    return compteur;
}


CelluleVar* chercherMot(ListeVar liste, const char *mot) {
    while (liste != NULL) {
        if (strcmp(liste->valeur, mot) == 0) {
   
            return liste;
        }
        liste = liste->suivant;
    }
    return NULL;
}

int est_lettre(int c) {
    return (isalpha((unsigned char)c));
}

int est_separateur(int c) {
    const char *sep = " \n.,;:!?\'ʼ’";
    return strchr(sep, c) != NULL;
}


void afficherListe(ListeVar liste) {
    if (liste == NULL) {
        printf("La liste est vide.\n");
        return;
    }

    printf("Liste des mots avec occurrences :\n");
    for (ListeVar tmp = liste; tmp != NULL; tmp = tmp->suivant) {
        printf("%s : %d\n", tmp->valeur, tmp->nb_occu);
    }
}


void afficherNPremiers(ListeVar liste, int n) {
    if (liste == NULL) {
        printf("La liste est vide.\n");
        return;
    }

    printf("Affichage des %d premiers termes :\n", n);
    int compteur = 0;
    for (ListeVar tmp = liste; tmp != NULL && compteur < n; tmp = tmp->suivant) {
        printf("%s : %d\n", tmp->valeur, tmp->nb_occu);
        compteur++;
    }
}


void libererListe(ListeVar *liste) {
    CelluleVar *tmp;
    while (*liste != NULL) {
        tmp = *liste;
        *liste = (*liste)->suivant;
        free(tmp->valeur);
        free(tmp);
    }
}


void trierListeDecroissante(ListeVar *liste) {
    if (*liste == NULL) return;

    for (CelluleVar *i = *liste; i != NULL; i = i->suivant) {
        CelluleVar *max = i;
        for (CelluleVar *j = i->suivant; j != NULL; j = j->suivant) {
            if (j->nb_occu > max->nb_occu) {
                max = j;
            }
        }
        // échanger valeur et nb_occu
        if (max != i) {
            char *tmp_p = i->valeur;
            int tmp_nb = i->nb_occu;

            i->valeur = max->valeur;
            i->nb_occu = max->nb_occu;

            max->valeur = tmp_p;
            max->nb_occu = tmp_nb;
        }
    }
}


int main(int argc, char* argv[]) {

    
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    int nb_mots = 0;
    ListeVar l = NULL;

    if (argc < 2) {
        printf("Usage: %s fichier.txt\n", argv[0]);
        return 1;
    }

    printf("%s\n", argv[1]);

    if(strcmp(argv[1],"-2") == 0){
        printf("Pas cool\n");
        //algo2
    }



    else if(strcmp(argv[1],"-3") == 0){
        //algo3
    }

    
    else{
        //algo 1 default
        for(int num_file = 1; num_file < argc; num_file++){
            FILE* f = fopen(argv[num_file], "r");
            if (f == NULL) {
                printf("Erreur dans l'ouverture du fichier\n");
            }
            else{
                


                char mot[TAILLE];
                int i = 0;

                int c;
                int dans_mot = 0;
                

                while ((c = fgetc(f)) != EOF) {
                    if (est_separateur(c)) {
                        if (dans_mot) {
                            mot[i] = '\0';   // fin du mot
                            //printf("Mot lu : %s\n", mot); // ou sauvegarde
                            i = 0;
                            dans_mot = 0;
                            nb_mots++;
                            CelluleVar* cellule = chercherMot(l, mot);
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

                if (dans_mot) {
                    mot[i] = '\0';
                    //printf("Mot lu : %s\n", mot);     
                    nb_mots++;
                    CelluleVar* cellule = chercherMot(l, mot);
                    if (cellule == NULL){
                        ajouterEnTete(&l, mot);
                    } 
                    else{
                        cellule->nb_occu++;
                    }
                }

                fclose(f);
                
            }
        }
    
        trierListeDecroissante(&l);

        printf("Nombre total de mots : %d\n", nb_mots);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Temps d'exécution : %f secondes\n", cpu_time_used);

        afficherNPremiers(l, 5);

        libererListe(&l);

    }


    return 0;
}
