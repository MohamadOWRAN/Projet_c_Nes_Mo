#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <time.h>

#define TAILLE 100
#define ALPHABET 26
#define FICHIER_MEM "bilan_memoire.txt"

typedef struct CelluleVar { 
    char* valeur;
    int nb_occu;
    struct CelluleVar *suivant;
} CelluleVar, *ListeVar ;

typedef struct ListeAdapt {
    char** valeur;
    int nb_elem;
    int capacite;
} ListeAdapt;

typedef struct {
    size_t cumul_alloc;   
    size_t cumul_desalloc;  
    size_t alloc_courant;   
    size_t max_alloc;       
    size_t nb_op;         
} InfoMem;

typedef struct Arbre {
    struct Arbre *lettres[ALPHABET];
    int nb_occu;                   
} Arbre;

typedef struct {
    char *mot;
    int nb_occu;
} MotOccu;

typedef struct {
    MotOccu *tab;
    int nb;
    int capacite;
} TableauMotOccu;

void* myMalloc(size_t size, InfoMem* im) {
    void* ptr = malloc(size);
    if (ptr && im) {
        im->cumul_alloc += size;
        im->alloc_courant += size;
        if (im->alloc_courant > im->max_alloc) {
            im->max_alloc = im->alloc_courant;
        }
        im->nb_op++;
    }
    return ptr;
}

void myFree(void* ptr, InfoMem* im, size_t old_size) {
    if (!ptr) return;
    free(ptr);
    if (im) {
        im->cumul_desalloc += old_size;
        im->alloc_courant -= old_size;
        im->nb_op++;
    }
}

void* myRealloc(void* ptr, size_t new_size, InfoMem* im, size_t old_size) {
    if (!ptr) return myMalloc(new_size, im);
    if (new_size == 0) {                    
        myFree(ptr, im, old_size);
        return NULL;
    }

    void* new_ptr = realloc(ptr, new_size);
    if (!new_ptr) return NULL;

    if (im) {

        if (new_ptr != ptr) {
            im->cumul_desalloc += old_size;
            im->alloc_courant -= old_size;
        }

        im->cumul_alloc += new_size;
        im->alloc_courant += new_size;

        if (im->alloc_courant > im->max_alloc) {
            im->max_alloc = im->alloc_courant;
        }

        im->nb_op++;
    }

    return new_ptr;
}



void enregistrerInfoMem(InfoMem *im, const char *nom_fichier, double temps) {
    FILE *f = fopen(nom_fichier, "w");
    if (!f) {
        perror("Erreur ouverture fichier memoire");
        return;
    }

    fprintf(f, "Bilan memoire :\n");
    fprintf(f, "Cumul alloc       : %zu octets\n", im->cumul_alloc);
    fprintf(f, "Cumul desalloc    : %zu octets\n", im->cumul_desalloc);
    fprintf(f, "Allocation max    : %zu octets\n", im->max_alloc);
    fprintf(f, "Nb operations mem : %zu\n", im->nb_op);
    fprintf(f, "Temps execution   : %f secondes\n", temps);

    fclose(f);
}

void initTableauMotOccu(TableauMotOccu *t, int capacite, InfoMem *im) {
    t->tab = myMalloc(capacite * sizeof(MotOccu), im);
    if (!t->tab) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    t->nb = 0;
    t->capacite = capacite;
}

void parcoursArbre(
    Arbre *a,
    char *buffer,
    int profondeur,
    TableauMotOccu *res,
    InfoMem *im
) {
    if (!a) return;

    if (a->nb_occu > 0) {

        if (res->nb == res->capacite) {
            int ancienne_cap = res->capacite;
            res->capacite *= 2;
            MotOccu *tmp = myRealloc(
                res->tab,
                res->capacite * sizeof(MotOccu),
                im,
                ancienne_cap * sizeof(MotOccu)
            );
            if (!tmp) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
            res->tab = tmp;
        }

        buffer[profondeur] = '\0';
        res->tab[res->nb].mot = myMalloc(strlen(buffer) + 1, im);
        strcpy(res->tab[res->nb].mot, buffer);
        res->tab[res->nb].nb_occu = a->nb_occu;
        res->nb++;
    }

    for (int i = 0; i < ALPHABET; i++) {
        if (a->lettres[i]) {
            buffer[profondeur] = 'a' + i;
            parcoursArbre(a->lettres[i], buffer, profondeur + 1, res, im);
        }
    }
}

void libererArbre(Arbre *a, InfoMem *im) {
    if (!a) return;

    for (int i = 0; i < ALPHABET; i++) {
        libererArbre(a->lettres[i], im);
    }

    myFree(a, im, sizeof(Arbre));
}

void initInfoMem(InfoMem *info) {
    if (!info) return;
    info->cumul_alloc = 0;
    info->cumul_desalloc = 0;
    info->alloc_courant = 0;
    info->max_alloc = 0;
    info->nb_op = 0;
}




Arbre* creerArbre(InfoMem *im) {
    Arbre *a = myMalloc(sizeof(Arbre), im);
    if (!a) return NULL;

    for (int i = 0; i < ALPHABET; i++)
        a->lettres[i] = NULL;

    a->nb_occu = 0;
    return a;
}

void insererMotArbre(Arbre *racine, const char *mot, InfoMem *im) {
    Arbre *courant = racine;

    for (int i = 0; mot[i] != '\0'; i++) {
        if (!isalpha((unsigned char)mot[i])) continue;

        int index = mot[i] - 'a';

        if (courant->lettres[index] == NULL) {
            courant->lettres[index] = creerArbre(im);
        }
        courant = courant->lettres[index];
    }

    courant->nb_occu++;
}

int comparerMotOccuDecroissant(const void *a, const void *b) {
    const MotOccu *m1 = (const MotOccu *)a;
    const MotOccu *m2 = (const MotOccu *)b;

    return m2->nb_occu - m1->nb_occu;
}

void afficherNPlusPresents(MotOccu *resultats, int nb_resultats, int n) {
    if (!resultats || nb_resultats == 0) {
        printf("Aucun mot à afficher.\n");
        return;
    }

    if (n > nb_resultats)
        n = nb_resultats;

    qsort(resultats, nb_resultats, sizeof(MotOccu),
          comparerMotOccuDecroissant);

    printf("Affichage des %d mots les plus fréquents :\n", n);
    for (int i = 0; i < n; i++) {
        printf("%s : %d\n", resultats[i].mot, resultats[i].nb_occu);
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
            int tmp = lst_occu[i];
            lst_occu[i] = lst_occu[max];
            lst_occu[max] = tmp;

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
    if (!l || !l->valeur) return;

    for (int i = 0; i < l->nb_elem; i++) {
        myFree(l->valeur[i], im, strlen(l->valeur[i]) + 1);
        l->valeur[i] = NULL;
    }

    myFree(l->valeur, im, l->capacite * sizeof(char*));
    l->valeur = NULL;

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
    const char *sep = " \n\r\t.,;:!?\'\"()[]{}«»—–-…“”‘’";
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
        myFree(tmp->valeur, im, strlen(tmp->valeur) + 1);
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
    printf("Cumul alloc       : %zu octets\n", im->cumul_alloc);
    printf("Cumul desalloc    : %zu octets\n", im->cumul_desalloc);
    printf("Allocation max    : %zu octets\n", im->max_alloc);
    printf("Nb operations mem : %zu\n", im->nb_op);
}


void ecrireListeAdaptDansFichier(ListeAdapt *l, int *lst_occu, const char *nom_fichier) {
    if (!l || !lst_occu) return;

    FILE *f = fopen(nom_fichier, "w");
    if (!f) {
        perror("Erreur ouverture fichier");
        return;
    }

    for (int i = 0; i < l->nb_elem; i++) {
        fprintf(f, "%s %d\n", l->valeur[i], lst_occu[i]);
    }

    fclose(f);
}


void ecrireListeVarDansFichier(ListeVar l, const char *nom_fichier) {
    if (!l) return;

    FILE *f = fopen(nom_fichier, "w");
    if (!f) {
        perror("Erreur ouverture fichier");
        return;
    }

    for (ListeVar tmp = l; tmp != NULL; tmp = tmp->suivant) {
        fprintf(f, "%s %d\n", tmp->valeur, tmp->nb_occu);
    }

    fclose(f);
}

void ecrireTableauMotOccuDansFichier(TableauMotOccu *res, const char *nom_fichier) {
    if (!res || !res->tab) return;

    FILE *f = fopen(nom_fichier, "w");
    if (!f) {
        perror("Erreur ouverture fichier");
        return;
    }

    for (int i = 0; i < res->nb; i++) {
        fprintf(f, "%s %d\n", res->tab[i].mot, res->tab[i].nb_occu);
    }

    fclose(f);
}

int mot_valide(const char *mot) {
    if (!mot) return 0;
    for (int i = 0; mot[i]; i++) {
        if (isalpha((unsigned char)mot[i]))
            return 1;
    }
    return 0;
}

int main(int argc, char* argv[]) {

    InfoMem im;
    initInfoMem(&im);

    clock_t start, end;
    double cpu_time_used;
    start = clock();

    int nb_mots = 0;

    int algo = 0;
    int num_file = 1; 

    if (argc < 2) {
        printf("Usage: %s fichier.txt\n", argv[0]);
        return 1;
    }
 
    if(strcmp(argv[1],"-2") == 0){

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
                            if (dans_mot && i > 0) {
                                mot[i] = '\0';

                                if (mot_valide(mot)) {
                                    nb_mots++;
                                    ajouterListeAdapt(&l_complet, mot, &im);
                                    if (!contientMotListeAdapt(&l_uniq, mot)) {
                                        ajouterListeAdapt(&l_uniq, mot, &im);
                                    }
                                }

                                i = 0;
                                dans_mot = 0;
                            }
                        
                        } 
                        
                        else {
                            if (i < TAILLE - 1) {
                                mot[i++] = (char)tolower((unsigned char)c);
                            }
                            dans_mot = 1;
                        }
                    
                    }

                    if (dans_mot && i > 0) {
                        mot[i] = '\0';
                        if (mot_valide(mot)) {
                            nb_mots++;
                            ajouterListeAdapt(&l_complet, mot, &im);
                            if (!contientMotListeAdapt(&l_uniq, mot))
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

        ecrireListeAdaptDansFichier(&l_uniq, lst_occu, "resultats.txt");

        libererListeAdapt(&l_uniq, &im);
        libererListeAdapt(&l_complet, &im);
        myFree(lst_occu, &im, l_uniq.nb_elem * sizeof(int));
        
        afficherInfoMem(&im);

        algo = 2;

        char nom_fichier[100];
        sprintf(nom_fichier, "bilan_memoire_algo%d_fichier%d.txt", algo, num_file);
        enregistrerInfoMem(&im, nom_fichier, cpu_time_used);

    }



    else if (strcmp(argv[1], "-3") == 0) {

        Arbre *racine = creerArbre(&im);

        for (int num_file = 1; num_file < argc; num_file++) {
            if (num_file == 1 && argv[num_file][0] == '-') continue;

            FILE *f = fopen(argv[num_file], "r");
            if (!f) continue;

            char mot[TAILLE];
            int i = 0, c, dans_mot = 0;

            while ((c = fgetc(f)) != EOF) {
                if (est_separateur(c)) {
                    if (dans_mot && i > 0) {
                        mot[i] = '\0';

                        if (mot_valide(mot)) {
                            insererMotArbre(racine, mot, &im);
                            nb_mots++;
                        }

                        i = 0;
                        dans_mot = 0;
                    }
                } else {
                    if (i < TAILLE - 1)
                        mot[i++] = tolower(c);
                    dans_mot = 1;
                }
            }

            if (dans_mot && i > 0) {
                mot[i] = '\0';
                if (mot_valide(mot)) {
                    insererMotArbre(racine, mot, &im);
                    nb_mots++;
                }
            }

            fclose(f);
        }

        TableauMotOccu res;
        initTableauMotOccu(&res, 128, &im);

        char buffer[TAILLE];
        parcoursArbre(racine, buffer, 0, &res, &im);

        printf("Nombre total de mots : %d\n", nb_mots);

        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Temps d'exécution : %f secondes\n", cpu_time_used);

        afficherNPlusPresents(res.tab, res.nb, 5);

        ecrireTableauMotOccuDansFichier(&res, "resultats.txt");

        for (int i = 0; i < res.nb; i++) {
        myFree(res.tab[i].mot, &im, strlen(res.tab[i].mot) + 1);
        }
        myFree(res.tab, &im, res.capacite * sizeof(MotOccu));

        libererArbre(racine, &im);

        afficherInfoMem(&im);

        algo = 3;

        char nom_fichier[100];
        sprintf(nom_fichier, "bilan_memoire_algo%d_fichier%d.txt", algo, num_file);
        enregistrerInfoMem(&im, nom_fichier, cpu_time_used);

        
    }

    
    else{
        ListeVar l = NULL;

        for(int num_file = 1; num_file < argc; num_file++){
            if(argv[num_file][0] == '-') continue;
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
                            if (dans_mot && i > 0) {
                                mot[i] = '\0';

                                if (mot_valide(mot)) {
                                    nb_mots++;
                                    CelluleVar* cellule = chercherMot(l, mot);
                                    if (!cellule) ajouterEnTete(&l, mot, &im);
                                    else cellule->nb_occu++;
                                }

                                i = 0;
                                dans_mot = 0;
                            }
                        
                        } 
                        
                        else {
                            if (i < TAILLE - 1) {
                                mot[i++] = (char)tolower((unsigned char)c);
                            }
                            dans_mot = 1;
                        }
                    
                    }

                    if (dans_mot && i > 0) {
                        mot[i] = '\0';
                        if (mot_valide(mot)) {
                            nb_mots++;
                            CelluleVar* cellule = chercherMot(l, mot);
                            if (!cellule) ajouterEnTete(&l, mot, &im);
                            else cellule->nb_occu++;
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

        ecrireListeVarDansFichier(l, "resultats.txt");

        libererListe(&l, &im);

        afficherInfoMem(&im);

        algo = 1;
        
        char nom_fichier[100];
        sprintf(nom_fichier, "bilan_memoire_algo%d_fichier%d.txt", algo, num_file);
        enregistrerInfoMem(&im, nom_fichier, cpu_time_used);
    }


    return 0;
}
