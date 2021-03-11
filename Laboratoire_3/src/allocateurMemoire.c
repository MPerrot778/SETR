#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>
#include "allocateurMemoire.h"

static void* bloc[ALLOC_N_BIG];
static uint8_t blocLibre[ALLOC_N_BIG];

int prepareMemoire(size_t tailleImageEntree, size_t tailleImageSortie) {

    size_t taille_bloc;
    
    if(tailleImageEntree < tailleImageSortie) {
        taille_bloc = tailleImageSortie;
    } else {
        taille_bloc = tailleImageEntree;
    }

    for(int i = 0; i < ALLOC_N_BIG; i++) {
        bloc[i] = malloc(taille_bloc*TAILLE_MAX);
        if(bloc[i]==NULL){
            return -1;
        }
        blocLibre[i] = 1;
    }

    if (mlockall(MCL_CURRENT) != 0)
        ErrorExit("mlock");
    
    return taille_bloc;
}

void* tempsreel_malloc(size_t taille) {

    for(int i = 0; i < ALLOC_N_BIG; i++) {
        if(blocLibre[i] == 1) {
            blocLibre[i] = 0;
            return bloc[i];
        }
    }
    return NULL;    
}

void tempsreel_free(void* ptr) {

    for (int i = 0; i < ALLOC_N_BIG; i++) {
        if(bloc[i] == ptr) {
            blocLibre[i] = 1;
            return;
        }
    }
    ErrorExit("tempsreel_free");
}
