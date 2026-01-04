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

void initInfoMem(InfoMem *info) {
    if (!info) return;
    info->cumul_alloc = 0;
    info->cumul_desalloc = 0;
}

void* myMalloc(size_t size, InfoMem* InfoMem) {
    void* ptr = malloc(size);
    if (ptr && InfoMem) {
        InfoMem->cumul_alloc += size;
    }
    return ptr;
}

void* myRealloc(void* ptr, size_t new_size, InfoMem* InfoMem, size_t old_size) {
    void* new_ptr = realloc(ptr, new_size);
    if (!new_ptr) return NULL;

    if (InfoMem) {
        InfoMem->cumul_desalloc += old_size;
        InfoMem->cumul_alloc += new_size;
    }
    return new_ptr;
}

void myFree(void* ptr, InfoMem* InfoMem, size_t old_size) {
    if (!ptr) return;
    free(ptr);
    if (InfoMem) {
        InfoMem->cumul_desalloc += old_size;
    }
}



void initListeAdapt(ListeAdapt *l, int capacite_initiale, InfoMem *im) {
    l->valeur = myMalloc(capacite_initiale * sizeof(char*), im);
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

void ajouterListeAdapt(ListeAdapt *l, const char *mot, InfoMem *im) {

    if (l->nb_elem == l->capacite) {
        l->capacite *= 2;
        char **tmp = myRealloc(l->valeur, l->capacite * sizeof(char *), im, l->capacite * sizeof(char *)/2);
        if (!tmp) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
        l->valeur = tmp;
    }

    l->valeur[l->nb_elem] = myMalloc(strlen(mot) + 1, im);
    if (!l->valeur[l->nb_elem]) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    l->valeur[l->nb_elem] = myMalloc(strlen(mot) + 1, im);
    memcpy(l->valeur[l->nb_elem], mot, strlen(mot) + 1);
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

void libererListeAdapt(ListeAdapt *l, InfoMem *im) {
    if (!l || !l->valeur) return;  // sécurité

    // Libérer chaque mot
    for (int i = 0; i < l->nb_elem; i++) {
        myFree(l->valeur[i], im, strlen(l->valeur[i]) + 1);
        l->valeur[i] = NULL;
    }

    // Libérer le tableau de pointeurs
    myFree(l->valeur, im, l->capacite * sizeof(char*));
    l->valeur = NULL;

    // Réinitialiser les compteurs
    l->nb_elem = 0;
    l->capacite = 0;
}

CelluleVar* allouerCellule(char *pval, InfoMem *im){
    CelluleVar* cellule = myMalloc(sizeof(CelluleVar), im);

    if (cellule == NULL) return NULL;
    
    size_t taille = strlen(pval) + 1;
    cellule->valeur = myMalloc(taille, im);

    if (cellule->valeur == NULL){
        myFree(cellule, im, sizeof(CelluleVar));
        return NULL;
    }
    
    memcpy(cellule->valeur, pval, taille);  

    cellule->nb_occu = 1;
 
    cellule->suivant = NULL;
    return cellule;
}


void ajouterEnTete(ListeVar *liste, char *val, InfoMem *im) {
    CelluleVar *c = allouerCellule(val, im);
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


void libererListe(ListeVar *liste, InfoMem *im) {
    CelluleVar *tmp;
    while (*liste != NULL) {
        tmp = *liste;
        *liste = (*liste)->suivant;
        myFree(tmp->valeur, im, strlen(tmp->valeur));
        myFree(tmp, im, sizeof(CelluleVar));
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

void afficherInfoMem(InfoMem *im){
    printf("Bilan memoire :\n");
    printf("Cumul alloc : %zu\n", im->cumul_alloc);
    printf("Cumul desalloc : %zu\n", im->cumul_desalloc);
}

int main(int argc, char* argv[]) {

    InfoMem im;
    initInfoMem(&im);

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

        initListeAdapt(&l_uniq, 100, &im);
        initListeAdapt(&l_complet, 100, &im);


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
                                ajouterListeAdapt(&l_complet, mot, &im);
                                if (contientMotListeAdapt(&l_uniq, mot) == 0){
                                    ajouterListeAdapt(&l_uniq, mot, &im);
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
                        ajouterListeAdapt(&l_complet, mot, &im);
                        if (contientMotListeAdapt(&l_uniq, mot) == 0){
                            ajouterListeAdapt(&l_uniq, mot, &im);
                        }
                    }

                    fclose(f);
                    
                }
            }
        }
        int *lst_occu = myMalloc(l_uniq.nb_elem * sizeof(int), &im);
        for(int j = 0; j < l_uniq.nb_elem; j++){
            lst_occu[j] = compterMotListeAdapt(&l_complet, l_uniq.valeur[j]);
        }

        trierListeAdaptDecroissant(&l_uniq, lst_occu);

        printf("Nombre total de mots : %d\n", nb_mots);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Temps d'exécution : %f secondes\n", cpu_time_used);

        afficherListeAdapt(&l_uniq, lst_occu, 5);

        libererListeAdapt(&l_uniq, &im);
        libererListeAdapt(&l_complet, &im);
        myFree(lst_occu, &im, l_uniq.nb_elem * sizeof(int));
        
        afficherInfoMem(&im);

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
                                    ajouterEnTete(&l, mot, &im);
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
                            ajouterEnTete(&l, mot, &im);
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

        libererListe(&l, &im);

        afficherInfoMem(&im);
    }


    return 0;
}
