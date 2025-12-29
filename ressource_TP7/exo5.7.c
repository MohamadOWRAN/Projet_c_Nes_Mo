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

void compter(FILE *f, int *mots, int *lignes, int*car) {
    int c, danslemot = 0;
    *mots = *lignes = *car = 0;
    while((c=fgetc(f))!=EOF) (*car)++;
    if (c== '\n') (*lignes)++;
    if(c==' ' || c=='\t' || c=='\n') {
        danslemot = 0;
    } else if (!danslemot) {
        (*mots)++;
        danslemot = 1;
    }
}

int main(int argc, char * argv[]) {
    int opt_m = 0, opt_l = 0, opt_c = 0, opt_v = 0;
    int i = 1;

    if (i < argc && argv[i][0] == '-') {
        char *o = argv[i] + 1;
        while (*o) {
            switch (*o) {
                case 'm': opt_m = 1; break;
                case 'l': opt_l = 1; break;
                case 'c': opt_c = 1; break;
                case 'v': opt_v = 1; break;
                default: printf("Option invalide : -%c\n", *o);
                return 1;
            }
            o++;
        }
        i++;
    }

    if (!opt_m && !opt_l && !opt_c && !opt_v) opt_m = opt_l = opt_c = 1;
    if (opt_v) printf("my_wc : version de nesma de wc pas les toilettes lol");
    if(i>=argc) {
        int m, l ,c;
        compter(stdin, &m, &l, &c);
        if (opt_m) printf("Mots : %d\n",m);
        if (opt_l) printf("Lignes : %d\n",l);
        if (opt_c) printf("Caracteres : %d\n",c);

        return 0;
    }

    for(int i=0;i<argc;i++) {
        FILE *f = fopen(argv[i], "r");
        int m,l,c;
        compter(f,&m,&l,&c);
        fclose(f);

        printf("%s : ",argv[i]);
        if (opt_m) printf("%d mots ",m);
        if (opt_l) printf("%d lignes ",l);
        if (opt_c) printf("%d caractères ",c);
        printf("\n");
    }
    return 0;
}