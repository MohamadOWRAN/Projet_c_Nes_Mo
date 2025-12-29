#include <stdio.h>



int main(int argc, char *argv[]) {
    int n = 0;
    for (int i = 1; i<argc;i++) {
        if((argv[i][0])=='-') n += 1;
        else{
            FILE *f = fopen(argv[i],"r");
            if (f) {
                printf("Le fichier %s n'est pas lu\n",argv[i]);
                fclose(f);
            }                
        }
    }
    printf("%d %d\n", argc, n);
    return 0;
}

