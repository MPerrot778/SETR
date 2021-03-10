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

    while ((zone->fd = shm_open(identifiant,O_RDWR,0666)) < 0);
    struct stat s;
    do {
        fstat(zone->fd,&s);
    } while(s.st_size == 0);

    void* shm_header = mmap(NULL, s.st_size, PROT_WRITE | PROT_READ, MAP_SHARED, zone->fd, 0);

    struct memPartageHeader* header = (struct memPartageHeader*)shm_header;

    zone->data = (((unsigned char*)shm_header) + sizeof(struct memPartageHeader));
    zone->header = header;
    zone->tailleDonnees = s.st_size - sizeof(struct memPartageHeader);
    zone->copieCompteur = 0;

    while(zone->header->frameWriter == 0); 
    
    return 0;
}

int initMemoirePartageeEcrivain(const char* identifiant, struct memPartage* zone, size_t taille, struct memPartageHeader* headerInfos){

    /* Create shared memory*/
    int shm_fd; // shared memory file descriptor

    shm_fd = shm_open(identifiant, O_CREAT | O_RDWR , 0666);

    if (shm_fd == -1)
        ErrorExit("shm_open - ecrivain");

    // printf("%d\n", shm_fd);

    if (ftruncate(shm_fd, taille + sizeof(struct memPartageHeader)) == -1)
        ErrorExit("ftruncate");

    void* shm_header = mmap(NULL, taille, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
    //printf("%d\n", shm_ptr);

    if (shm_header == MAP_FAILED) 
        ErrorExit("mmap");

    headerInfos = (struct memPartageHeader*)shm_header;
    
    /* Create Mutex in shared memory header */

    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);

    pthread_mutex_init(&headerInfos->mutex, &mutex_attr);

    headerInfos->frameReader = 0;
    headerInfos->frameWriter = 1;

    zone->fd = shm_fd;
    zone->header = headerInfos;
    zone->tailleDonnees = taille;
    zone->copieCompteur++;

    return 0; // return ou exit(EXIT_SUCCESS) - same thing?
    
}

int attenteLecteur(struct memPartage *zone){

    while(zone->header->frameWriter == zone->header->frameReader);
    pthread_mutex_t mutex = zone->header->mutex;
    pthread_mutex_lock(&mutex); // acquisition du mutex
    return 0;
}

/* returns 0 si mutex est acquit, 1 sinon */
int attenteLecteurAsync(struct memPartage *zone){

    if(zone->header->frameWriter == zone->header->frameReader)
        return 1;

    pthread_mutex_t mutex = zone->header->mutex;
    if (pthread_mutex_trylock(&mutex) == 0)    // acquisiton non bloquant du mutex
        return 0;
    
    return 1;
}

int attenteEcrivain(struct memPartage *zone){

    while(zone->copieCompteur == zone->header->frameReader);
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