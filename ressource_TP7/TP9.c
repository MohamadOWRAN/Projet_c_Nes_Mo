#include <stdio.h>
#include <stdlib.h>


#define TAILLE 100


typedef struct 
{
    int deg;
    float* coef;
} Polynome ;


void affichePolynome(Polynome p){
    int coef_n_u = 0;
    printf("P(x) = ");
    for(int i = 0; i <= p.deg; i++){

        if(coef_n_u == 0 && p.coef[i] != 0){
            coef_n_u = 1;
            if(i == 0){
                if(p.coef[i] > 0) printf("%.2f ", p.coef[i]);
                if(p.coef[i] < 0) printf("- %.2f ", -p.coef[i]);
            }
            else if(i == 1){
                if(p.coef[i] > 0) printf("%.2f*x ", p.coef[i]);
                if(p.coef[i] < 0) printf("- %.2f*x ", -p.coef[i]);
            }
            else{
                if(p.coef[i] > 0) printf("%.2f*x^%d ", p.coef[i], i);
                if(p.coef[i] < 0) printf("- %.2f*x^%d ", -p.coef[i], i);
            }
        }
        
        else if(p.coef[i] != 0){
            if(i == 0){
                if(p.coef[i] > 0) printf("+ %.2f ", p.coef[i]);
                if(p.coef[i] < 0) printf("- %.2f ", -p.coef[i]);
            }
            else if(i == 1){
                if(p.coef[i] > 0) printf("+ %.2f*x ", p.coef[i]);
                if(p.coef[i] < 0) printf("- %.2f*x ", -p.coef[i]);
            }
            else{
                if(p.coef[i] > 0) printf("+ %.2f*x^%d ", p.coef[i], i);
                if(p.coef[i] < 0) printf("- %.2f*x^%d ", -p.coef[i], i);
            }
        }
    }
    printf("\n");
    
}


int lirePolynome(Polynome *p){
    printf("Entrez le degree du polynome : ");
    scanf("%d", &(p->deg));
    int i = p->deg;
    p->coef = malloc(sizeof(float) * (p->deg+1));
    for(int j = 0; j <= p->deg; j++){
        p->coef[j] = 0;
    }
    printf("Entrez le coefficient du terme de degre %d : ", i);
    scanf("%f", &(p->coef[i]));
    while(i >= 0){
        printf("Entrez le degre suivi du coefficient (terminer avec un degre negatif) : ");
        scanf("%d", &i);
        if(i >= 0 && i <= p->deg){
            printf("Entrez le coefficient du terme de degre %d : ", i);
            scanf("%f", &(p->coef[i]));
        }
    }
    return 1;
}

int somme(Polynome p, Polynome q, Polynome *res){
    if(p.deg > q.deg) res->deg = p.deg;
    else res->deg = q.deg;
    res->coef = malloc(sizeof(float) * (res->deg+1));
    for(int i = 0; i <= res->deg; i++){
        if(p.deg > q.deg){
            if(i > q.deg) res->coef[i] = p.coef[i];
            else res->coef[i] = p.coef[i] + q.coef[i];
        }
        else{
            if(i > p.deg) res->coef[i] = q.coef[i];
            else res->coef[i] = p.coef[i] + q.coef[i];
        }
    }
    return 1;
}


int produit(Polynome p, Polynome q, Polynome *res){
    res->deg = p.deg + q.deg;
    res->coef = malloc(sizeof(float) * (res->deg+1));
    for(int i = 0; i <= res->deg; i++) res->coef[i] = 0;
    for(int j = 0; j <= p.deg; j++){
        for(int k = 0; k <= q.deg; k++){
            res->coef[j+k] += p.coef[j] * q.coef[k];
        }
    }
    return 1;
}


int main(void){
    Polynome Tp[26];
    for(int i = 0; i<26; i++){
        Tp[i].deg = 0;
        Tp[i].coef = malloc(sizeof(float) * 1);
        Tp[i].coef[0] = 0;
    }
    char c;
    int a1, a2, a3;
    int b = 4;

    while(b !=0){
        printf("Commandes :\n");
        printf("0. Stop\n");
        printf("1. Cree polynome\n");
        printf("2. Somme polynome\n");
        printf("3. Produit polynome\n");
        printf("4. Afficher polynome\n");
        scanf("%d", &b);
        if(b == 1){
            printf("Lettre de votre Polynome: \n");
            scanf(" %c", &c);
            if( 'A'<= c && c <= 'Z') a1 = c - 'A';
            if( 'a'<= c && c <= 'z') a1 = c - 'a';
            lirePolynome(&(Tp[a1]));
            affichePolynome(Tp[a1]);
        }
        if(b == 2){
            printf("Lettre de votre Polynome 1: ");
            scanf(" %c", &c);
            if( 'A'<= c && c <= 'Z') a1 = c - 'A';
            if( 'a'<= c && c <= 'z') a1 = c - 'a';
            printf("Lettre de votre Polynome 2: ");
            scanf(" %c", &c);
            if( 'A'<= c && c <= 'Z') a2 = c - 'A';
            if( 'a'<= c && c <= 'z') a2 = c - 'a';
            printf("Lettre de votre Polynome 3: ");
            scanf(" %c", &c);
            if( 'A'<= c && c <= 'Z') a3 = c - 'A';
            if( 'a'<= c && c <= 'z') a3 = c - 'a';
            somme(Tp[a1], Tp[a2], &(Tp[a3]));
            affichePolynome(Tp[a3]);
        }
        if(b == 3){
            printf("Lettre de votre Polynome 1: ");
            scanf(" %c", &c);
            if( 'A'<= c && c <= 'Z') a1 = c - 'A';
            if( 'a'<= c && c <= 'z') a1 = c - 'a';
            printf("Lettre de votre Polynome 2: ");
            scanf(" %c", &c);
            if( 'A'<= c && c <= 'Z') a2 = c - 'A';
            if( 'a'<= c && c <= 'z') a2 = c - 'a';
            printf("Lettre de votre Polynome 3: ");
            scanf(" %c", &c);
            if( 'A'<= c && c <= 'Z') a3 = c - 'A';
            if( 'a'<= c && c <= 'z') a3 = c - 'a';
            produit(Tp[a1], Tp[a2], &(Tp[a3]));
            affichePolynome(Tp[a3]);
        }
        if(b == 4){
            printf("Lettre de votre Polynome: ");
            scanf("%c", &c);
            if( 'A'<= c && c <= 'Z') a1 = c - 'A';
            if( 'a'<= c && c <= 'z') a1 = c - 'a';
            affichePolynome(Tp[a1]);
        }
    }
    return 0;
}