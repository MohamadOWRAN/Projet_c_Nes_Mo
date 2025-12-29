#include <stdio.h>
#include <stdlib.h>

typedef struct cellulePoint {
    char *p;
    struct cellulePoint *suivant;
} CellulePoint, *ListePoint ;

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

int main(int argc, char * argv[]){

    return 1;
}