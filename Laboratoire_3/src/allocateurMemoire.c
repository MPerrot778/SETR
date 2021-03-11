#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>
#include "allocateurMemoire.h"

#define TAILLE_MAX  5

static void* bloc[TAILLE_MAX];
static uint8_t blocLibre[TAILLE_MAX];

int prepareMemoire(size_t tailleImageEntree, size_t tailleImageSortie) {

    size_t taille_bloc;
    
    if(tailleImageEntree < tailleImageSortie) {
        taille_bloc = tailleImageSortie;
    } else {
        taille_bloc = tailleImageEntree;
    }

    for(int i = 0; i < TAILLE_MAX; i++) {
        bloc[i] = malloc(taille_bloc);
        if(bloc[i]==NULL){
            return -1;
        }
        blocLibre[i] = 1;
    }
    return 0;
}

void* tempsreel_malloc(size_t taille) {

    for(int i = 0; i < TAILLE_MAX -1; i++) {
        if(blocLibre[i] == 1) {
            blocLibre[i] = 0;
            return bloc[i];
        }
    }
    return NULL;    
}

void tempsreel_free(void* ptr) {

    for (int i = 0; i < TAILLE_MAX-1; i++) {
        if(bloc[i] == ptr) {
            blocLibre[i] = 1;
            return;
        }
    }
}
