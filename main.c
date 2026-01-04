#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <time.h>

#define TAILLE 100



typedef struct CelluleVar { //struct pour l'algo 1
    char* valeur;
    int nb_occu;
    struct CelluleVar *suivant;
} CelluleVar, *ListeVar ;

typedef struct ListeAdapt { //struct pour l'algo 2
    char** valeur;
    int nb_elem;
    int capacite;
} ListeAdapt;

typedef struct {
size_t cumul_alloc; // champ obligatoire : cumul de l’espace mémoire alloué
size_t cumul_desalloc; // champ obligatoire : cumul de l’espace mémoire désalloué
 // d’autres champs qui sembleraient utiles
} InfoMem;

void* myMalloc(size_t size, InfoMem* infoMem){

}

void* myRealloc(void* ptr, size_t new_size, InfoMem* infoMem, size_t old_size){

}
void myFree(void* ptr, InfoMem* infoMem, size_t old_size){

}


void initListeAdapt(ListeAdapt *l, int capacite_initiale) {
    l->valeur = malloc(capacite_initiale * sizeof(char*));
    if (l->valeur == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    l->nb_elem = 0;
    l->capacite = capacite_initiale;
}

int contientMotListeAdapt(ListeAdapt *l, const char *mot) {
    for (int i = 0; i < l->nb_elem; i++) {
        if (strcmp(l->valeur[i], mot) == 0)
            return 1;
    }
    return 0;
}

int compterMotListeAdapt(ListeAdapt *l, const char *mot) {
    int nb_occu = 0;
    for (int i = 0; i < l->nb_elem; i++) {
        if (strcmp(l->valeur[i], mot) == 0)
            nb_occu++;
    }
    return nb_occu;
}

void ajouterListeAdapt(ListeAdapt *l, const char *mot) {

    if (l->nb_elem == l->capacite) {
        l->capacite *= 2;
        char **tmp = realloc(l->valeur, l->capacite * sizeof(char *));
        if (!tmp) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
        l->valeur = tmp;
    }

    l->valeur[l->nb_elem] = malloc(strlen(mot) + 1);
    if (!l->valeur[l->nb_elem]) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    strcpy(l->valeur[l->nb_elem], mot);
    l->nb_elem++;
}


void trierListeAdaptDecroissant(ListeAdapt *l, int *lst_occu) {
    for (int i = 0; i < l->nb_elem - 1; i++) {
        int max = i;
        for (int j = i + 1; j < l->nb_elem; j++) {
            if (lst_occu[j] > lst_occu[max]) {
                max = j;
            }
        }

        if (max != i) {
            // échange occurrences
            int tmp = lst_occu[i];
            lst_occu[i] = lst_occu[max];
            lst_occu[max] = tmp;

            // échange mots
            char *tmp_mot = l->valeur[i];
            l->valeur[i] = l->valeur[max];
            l->valeur[max] = tmp_mot;
        }
    }
}


void afficherListeAdapt(ListeAdapt *l, int* lst_occu, int n){
    if (l == NULL) {
        printf("La liste est vide.\n");
        return;
    }
    if(n > l->nb_elem) n = l->nb_elem;
    printf("Liste des mots avec occurrences :\n");
    for(int i = 0; i < n; i++){
        printf("%s : %d\n", l->valeur[i], lst_occu[i]);
    }
}

void libererListeAdapt(ListeAdapt *l) {
    if (!l || !l->valeur) return;  // sécurité

    // Libérer chaque mot
    for (int i = 0; i < l->nb_elem; i++) {
        free(l->valeur[i]);
        l->valeur[i] = NULL;
    }

    // Libérer le tableau de pointeurs
    free(l->valeur);
    l->valeur = NULL;

    // Réinitialiser les compteurs
    l->nb_elem = 0;
    l->capacite = 0;
}

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

    if (argc < 2) {
        printf("Usage: %s fichier.txt\n", argv[0]);
        return 1;
    }

    if(strcmp(argv[1],"-2") == 0){
        printf("Pas cool\n");

        ListeAdapt l_uniq;
        ListeAdapt l_complet;

        initListeAdapt(&l_uniq, 100);
        initListeAdapt(&l_complet, 100);


        for(int num_file = 1; num_file < argc; num_file++){
            if(num_file == 1 && argv[num_file][0] == '-'){ }
            else{
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
                                ajouterListeAdapt(&l_complet, mot);
                                if (contientMotListeAdapt(&l_uniq, mot) == 0){
                                    ajouterListeAdapt(&l_uniq, mot);
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
                        ajouterListeAdapt(&l_complet, mot);
                        if (contientMotListeAdapt(&l_uniq, mot) == 0){
                            ajouterListeAdapt(&l_uniq, mot);
                        }
                    }

                    fclose(f);
                    
                }
            }
        }
        int lst_occu[l_uniq.nb_elem]; //algo2
        for(int j = 0; j < l_uniq.nb_elem; j++){
            lst_occu[j] = compterMotListeAdapt(&l_complet, l_uniq.valeur[j]);
        }

        trierListeAdaptDecroissant(&l_uniq, lst_occu);

        printf("Nombre total de mots : %d\n", nb_mots);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Temps d'exécution : %f secondes\n", cpu_time_used);

        afficherListeAdapt(&l_uniq, lst_occu, 5);

        libererListeAdapt(&l_uniq);
        libererListeAdapt(&l_complet);

        


    }



    else if(strcmp(argv[1],"-3") == 0){
        //algo3
    }

    
    else{
        //algo 1 default
        ListeVar l = NULL;

        for(int num_file = 1; num_file < argc; num_file++){
            if(num_file == 1 && argv[num_file][0] == '-'){ }
            else{
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
