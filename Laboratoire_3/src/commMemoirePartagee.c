#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sched.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include "commMemoirePartagee.h"

int initMemoirePartageeLecteur(const char* identifiant, struct memPartage *zone){
    int shm_fd; // shared memory file descriptor
    void* shm_ptr;  // points to shared memory

    shm_fd = shm_open(identifiant, O_CREAT | O_RDONLY | O_EXCL);

    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    printf("%d\n", shm_fd);
    if (ftruncate(identifiant, sizeof(memPartage)) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    shm_ptr = nmap(NULL, sizeof(memPartage), PROT_READ, MAP_SHARED, shm_fd, 0);
    printf("%d\n" shm_ptr);

    if (shm_ptr == MAP_FAILED) {
        perror("nmap");
        exit(EXIT_FAILURE);
    }

}

int initMemoirePartageeEcrivain(const char* identifiant, struct memPartage *zone, size_t taille, struct memPartageHeader* headerInfos){

}

int attenteLecteur(struct memPartage *zone){

}

int attenteLecteurAsync(struct memPartage *zone){

}

int attenteEcrivain(struct memPartage *zone){
    
}