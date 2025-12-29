#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>

#define TAILLE_X 512
#define TAILLE_Y 512

#define TAILLE 40

typedef struct {
int x ;
int y ;
} Point;

typedef struct cellulePoint {
Point p ;
struct cellulePoint *suivant;
} CellulePoint, *ListePoint ;

CellulePoint* allouerCellule(Point pval){
    CellulePoint* cellule = malloc(sizeof(CellulePoint));
    cellule->p = pval;
    cellule->suivant = NULL;
    return cellule;
}

int insererEnTete(ListePoint *liste, Point val){
    if(liste == NULL) return 0;
    CellulePoint* cellule = allouerCellule(val);
    cellule->suivant = *liste;
    *liste = cellule;
    return 1;
}

Point creePoint(int x, int y){
    Point p;
    p.x = x;
    p.y = y;
    return p;
}

int entrerPolygone(ListePoint *lst){
    int x1, y1, x2, y2, unicode;
    x1 = -1, y1 = -1;
    MLV_Event ev;
    MLV_Keyboard_button sym;
	MLV_Keyboard_modifier mod;
    ev = MLV_wait_keyboard_or_mouse(&sym, &mod, &unicode, &x2, &y2);
    while(ev == MLV_MOUSE_BUTTON){
        insererEnTete(lst, creePoint(x2, y2));
        if(x1 != -1 && x2 != -1){
            MLV_draw_line(x1, y1, x2, y2, MLV_COLOR_ALICE_BLUE);
            MLV_update_window();
        }
        x1 = x2;
        y1 = y2;
        ev = MLV_wait_keyboard_or_mouse(&sym, &mod, &unicode, &x2, &y2);
    }
    return 1;
}
/*
void libererListe(ListePoint *liste){
    CellulePoint* c = *liste;
    CellulePoint* c2;
    int a = 0;
    while(c->suivant != NULL){
        printf("%d\n", a);
        a++;
        c2 = c->suivant;
        free(c);
        c = c2;
    }
    free(c);
}
*/
void afficherListe(ListePoint *liste){
    Point p;
    for(CellulePoint* c = *liste; c; c=c->suivant){
        p = c->p;
        printf("%d %d\n", p.x, p.y);
    }
}

void libererListe(ListePoint *liste){
    ListePoint c2, c = *liste;
    while (c != NULL){
        c2 = c->suivant;
        free(c);
        c = c2;
    }
    *liste = NULL;
}

void dessinePolygone(ListePoint *lst){
    CellulePoint* c = *lst;
    if(c){
        int i = 0;
        int lx[TAILLE];
        int ly[TAILLE];
        
        while(c != NULL && i < TAILLE){
            lx[i] = c->p.x;
            ly[i] = c->p.y;
            i++;
            printf("%d %d\n", c->p.x, c->p.y);
            c = c->suivant;
        }
        MLV_draw_filled_polygon(lx, ly, i, MLV_COLOR_ANTIQUE_WHITE);
        MLV_update_window();
    }
}

ListePoint extraireListeBasGauche(ListePoint* liste, Point p){
    int estPremier = 0;
    ListePoint l = malloc(sizeof(ListePoint));
    CellulePoint* c2;
    for(CellulePoint* c = *liste; c; c=c->suivant){
        if(c->p.x < p.x && c->p.y > p.y){
            printf("%d %d\n", p2.x, p2.y);
            if(estPremier){
                l->p = c->p;
                l->suivant = NULL;
                c2 = l;
            }
            else{
                c2->suivant = allouerCellule(creePoint(p2.x, p2.y));
                c2 = c2->suivant;
            }
        }
    }
    return l;
}  

ListePoint extraireListeHautGauche(ListePoint* liste, Point p){

    ListePoint result = NULL;
    ListePoint *fin = &result;

    CellulePoint *c = *liste;

    while (c != NULL) {

        if (c->p.x < p.x && c->p.y < p.y) {

            CellulePoint *nouveau = allouerCellule(c->p);
            if (!nouveau) {return result;}

            *fin = nouveau;
            fin = &((*fin)->suivant);
        }

        c = c->suivant;
    }

    return result;
}

/*
ListePoint extraireListeBasGauche(ListePoint* liste, Point p){
    int est_vide = 1;
    CellulePoint* c = *liste;
    ListePoint* lf = malloc(sizeof(ListePoint*));
    CellulePoint* cellule = lf;
    while(1){
        if(c->p.x < p.x &&  c->p.y > p.y){
            if(est_vide){ 
                cellule = allouerCellule(c->p);
            }
            else{
                insererEnTete(cellule, c->p);
            }
        }
        if(c->suivant != NULL) c = c->suivant;
        else break;
    }
    return lf;
}
*/



int main(void){
    ListePoint l = NULL;
    MLV_create_window( "", "", TAILLE_X, TAILLE_Y );

    entrerPolygone(&l);

    dessinePolygone(&l);
    MLV_wait_seconds(1);

    MLV_clear_window(MLV_COLOR_BLACK);
    MLV_update_window();
    
    Point p = creePoint(250, 250);
    ListePoint l2 = extraireListeBasGauche(&l, p);
    dessinePolygone(&l2);
    MLV_wait_seconds(3);

    MLV_free_window();
    return 0;
}
