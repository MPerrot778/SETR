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
    
    /* Open shared memory */
    int shm_fd = -1;

    while (shm_fd < 0 || sizeof(zone) == 0) {
        shm_fd = shm_open(identifiant, O_RDONLY, 0);
    }

    zone = mmap(NULL, sizeof(zone), PROT_READ, MAP_SHARED, shm_fd, 0); 

    while (zone->copieCompteur == 0); // boucle tant que compteur est zero

    pthread_mutex_t mutex = zone->header->mutex;
    pthread_mutex_lock(&mutex); // attente sur le mutex
    
    return 0;
}

int initMemoirePartageeEcrivain(const char* identifiant, struct memPartage* zone, size_t taille, struct memPartageHeader* headerInfos){

    /* Create shared memory*/

    int shm_fd; // shared memory file descriptor
    //void* shm_ptr;  // points to shared memory

    shm_fd = shm_open(identifiant, O_CREAT | O_RDWR | O_EXCL, 0666);

    if (shm_fd == -1)
        ErrorExit("shm_open - ecrivain");

    // printf("%d\n", shm_fd);

    if (ftruncate(shm_fd, sizeof(*zone)) == -1)
        ErrorExit("ftruncate");

    zone = mmap(NULL, sizeof(zone), PROT_WRITE, MAP_SHARED, shm_fd, 0);
    //printf("%d\n", shm_ptr);

    if (zone == MAP_FAILED) 
        ErrorExit("mmap");

    /* Create Mutex in shared memory header */

    pthread_mutex_t shm_mutex = PTHREAD_MUTEX_INITIALIZER;
    if(pthread_mutex_init(&shm_mutex, NULL) != 0) 
        ErrorExit("mutex");

    pthread_mutex_lock(&shm_mutex); // lock mutex

    headerInfos->mutex = shm_mutex;

    zone->fd = shm_fd;
    zone->header = headerInfos; // memcpy?
    zone->tailleDonnees = taille;
    zone->copieCompteur++;

    return 0; // return ou exit(EXIT_SUCCESS) - same thing?
    
}

int attenteLecteur(struct memPartage *zone){
    pthread_mutex_t mutex = zone->header->mutex;
    pthread_mutex_lock(&mutex); // acquisition du mutex
    zone->copieCompteur++;  // incremente compteur_lecteur
    return 0;
}

/* returns 0 si mutex est acquit, -1 sinon */
int attenteLecteurAsync(struct memPartage *zone){
    pthread_mutex_t mutex = zone->header->mutex;
    if (pthread_mutex_trylock(&mutex) == 0) {   // acquisiton non bloquant du mutex
        zone->copieCompteur++; // incremente compteur_lecteur
        return 0;
    } 
    return -1;
}

int attenteEcrivain(struct memPartage *zone){
    pthread_mutex_t mutex = zone->header->mutex;
    pthread_mutex_lock(&mutex); // acquisition du mutex
    zone->copieCompteur++; // incremente compteur_ecrivain
}

int main(int argc, char* argv[]){
    struct memPartage *testmem = malloc(sizeof(*testmem));
    struct memPartageHeader *testmemheader = malloc(sizeof(*testmemheader));
    int c;
    char *id;

    while((c = getopt (argc, argv, "a:")) != -1){
        switch (c){
            case 'a':
                id = optarg;
                printf("Shared mem name: %s\n",id);
                break;
            default:
                abort();
        }
    }  

    if(initMemoirePartageeEcrivain(id, testmem, (size_t)32,testmemheader) == 0){
        printf("mem correctly initialized\n");
    }
    else{
        exit(1);
    }
    return 0;
}