#include <MLV/MLV_all.h>
#include <stdio.h>


#define WIDTH 640
#define HEIGHT 480
#define TAILLE 10

typedef struct {
    int nb_som;
    int sx[TAILLE];
    int sy[TAILLE];
} Polygone ;

void dessinePolygone(Polygone p){
    if(p.nb_som >= 2){
        MLV_draw_filled_polygon(p.sx, p.sy, p.nb_som, MLV_rgba(255, 140, 210, 255));
        MLV_update_window();
    }
}

void saisirPolygone(Polygone *p){
    int x, y, unicode, n = 0;
    MLV_Event ev;
    MLV_Keyboard_button sym;
	MLV_Keyboard_modifier mod;
    ev = MLV_wait_keyboard_or_mouse(&sym, &mod, &unicode, &x, &y);
    while(ev == MLV_MOUSE_BUTTON && n < TAILLE){
        p->sx[n] = x;
        p->sy[n] = y;
        n += 1;
        p->nb_som = n;
        dessinePolygone(*p);
        
        if(n < TAILLE) ev = MLV_wait_keyboard_or_mouse(&sym, &mod, &unicode, &x, &y);
    }
    p->nb_som = n;
}

int effaceSommet(Polygone *p, int i){
    if(i > p->nb_som || p->nb_som <= 3) return 0;
    p->nb_som -= 1;
    int k = i-1;
    while(k < p->nb_som){
        p->sx[k] = p->sx[k + 1];
        p->sy[k] = p->sy[k + 1];
        k += 1; 
    }
    return 1;
}

int main(void){
    Polygone P;

    MLV_create_window(" ", " ", WIDTH, HEIGHT);

    MLV_clear_window(MLV_COLOR_WHITE);
    MLV_update_window();
    
    saisirPolygone(&P);

    dessinePolygone(P);

    int a = (rand()%P.nb_som) + 1;

    MLV_wait_seconds(1);

    while(effaceSommet(&P, a)){
        MLV_clear_window(MLV_COLOR_WHITE);
        dessinePolygone(P);
        MLV_wait_seconds(1);
        a = (rand()%P.nb_som) + 1;
    }

    MLV_wait_seconds(1);

	MLV_free_window();

    return 0;
}

