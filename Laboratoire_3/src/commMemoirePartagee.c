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
#include <stdlib.h>
#include "commMemoirePartagee.h"

int initMemoirePartageeLecteur(const char* identifiant, struct memPartage *zone){   
    

}

int initMemoirePartageeEcrivain(const char* identifiant, struct memPartage* zone, size_t taille, struct memPartageHeader* headerInfos){

    /* Create shared memory*/

    int shm_fd; // shared memory file descriptor
    void* shm_ptr;  // points to shared memory

    shm_fd = shm_open(identifiant, O_CREAT | O_RDWR | O_EXCL, 0666);

    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    printf("%d\n", shm_fd);
    if (ftruncate(identifiant, taille) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    shm_ptr = nmap(NULL, taille, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    printf("%d\n", shm_ptr);

    if (shm_ptr == MAP_FAILED) {
        perror("nmap");
        exit(EXIT_FAILURE);
    }

    zone->fd = shm_fd;
    zone->tailleDonnees = taille;

    /* Create Mutex in shared memory header */

    pthread_mutex_t shm_mutex = PTHREAD_MUTEX_INITIALIZER;
    if(pthread_mutex_init(&shm_mutex, NULL) != 0) {
        perror("mutex");
        exit(EXIT_FAILURE);
    }

    headerInfos->mutex = shm_mutex;
    zone->copieCompteur++; 

}

int attenteLecteur(struct memPartage *zone){

}

int attenteLecteurAsync(struct memPartage *zone){

}

int attenteEcrivain(struct memPartage *zone){
    
}