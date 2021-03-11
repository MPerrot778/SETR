// Gestion des ressources et permissions
#include <sys/resource.h>

// Nécessaire pour pouvoir utiliser sched_setattr et le mode DEADLINE
#include <sched.h>
#include "schedsupp.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "allocateurMemoire.h"
#include "commMemoirePartagee.h"
#include "utils.h"
#include "jpgd.h"

//#include "jpgd.hpp"

// Définition de diverses structures pouvant vous être utiles pour la lecture d'un fichier ULV
#define HEADER_SIZE 4
const char header[] = "SETR";


struct videoInfos{
        uint32_t largeur;
        uint32_t hauteur;
        uint32_t canaux;
        uint32_t fps;
};

/******************************************************************************
* FORMAT DU FICHIER VIDEO
* Offset     Taille     Type      Description
* 0          4          char      Header (toujours "SETR" en ASCII)
* 4          4          uint32    Largeur des images du vidéo
* 8          4          uint32    Hauteur des images du vidéo
* 12         4          uint32    Nombre de canaux dans les images
* 16         4          uint32    Nombre d'images par seconde (FPS)
* 20         4          uint32    Taille (en octets) de la première image -> N
* 24         N          char      Contenu de la première image (row-first)
* 24+N       4          uint32    Taille (en octets) de la seconde image -> N2
* 24+N+4     N2         char      Contenu de la seconde image
* 24+N+N2    4          uint32    Taille (en octets) de la troisième image -> N2
* ...                             Toutes les images composant la vidéo, à la suite
*            4          uint32    0 (indique la fin du fichier)
******************************************************************************/

static int debug_flag;

int main(int argc, char* argv[]){
    
    // Écrivez le code de décodage et d'envoi sur la zone mémoire partagée ici!
    // N'oubliez pas que vous pouvez utiliser jpgd::decompress_jpeg_image_from_memory()
    // pour décoder une image JPEG contenue dans un buffer!
    // N'oubliez pas également que ce décodeur doit lire les fichiers ULV EN BOUCLE
 
    // getopt variable
    int c;
    char *svalue;
    char *dvalue;
    char *inFile;
    char *outMem;
    int deadline_flag = 0;
    struct sched_param p;
    struct sched_attr attr;

    // getopt section
    p.sched_priority = 90;
    static struct option long_options[] = {
        {"debug", no_argument, &debug_flag, 1},
        {"set_scheduler", required_argument, 0, 's'},
        {0, 0, 0, 0}
    };
    int option_index = 0;
    while ((c = getopt_long(argc, argv, "s:d:", long_options, &option_index)) != -1){
        switch(c){
            case 0:
                /* If this option set a flag, do nothing else now. */
                if (long_options[option_index].flag != 0)
                    break;
                printf ("option %s", long_options[option_index].name);
                if (optarg)
                    printf (" with arg %s", optarg);
                printf ("\n");
                break;

            case 's':
                svalue = optarg;
                if(strcmp(svalue,"NORT\0")==0){
                    printf("NORT scheduler [Default]\n");
                }
                else if (strcmp(svalue,"RR\0")==0)
                {
                    if(sched_setscheduler(getpid(),SCHED_RR,&p)<0){
                        perror("failed to initialize scheduler\n");
                        exit(EXIT_FAILURE);
                    }
                    printf("RR scheduler initialized\n");
                }
                else if (strcmp(svalue,"FIFO\0")==0)
                {
                    if(sched_setscheduler(getpid(),SCHED_FIFO,&p)<0){
                        perror("failed to initialize scheduler\n");
                        exit(EXIT_FAILURE);
                    }
                    printf("FIFO scheduler initialized\n");
                }
                else if (strcmp(svalue,"DEADLINE\0")==0)
                {   
                    deadline_flag = 1;
                    attr.size = sizeof(attr);
                    attr.sched_policy = SCHED_DEADLINE;
                    attr.sched_flags = 0;
                    attr.sched_priority = 90;
                }     
                break;

            case 'd':
                if(!deadline_flag){
                    printf("DEADLINE scheduler need to be set first");
                    exit(EXIT_FAILURE);
                }
                // Splits into tokens
                dvalue = strtok((char*)optarg,",");

                //Runtime
                printf("sched_runtime: %s ns\n",dvalue);
                attr.sched_runtime = (uint32_t)atoi(dvalue);
                dvalue = strtok(NULL,",");

                printf("sched_deadline: %s ns\n",dvalue);
                attr.sched_deadline = (uint32_t)atoi(dvalue);
                dvalue = strtok(NULL,",");

                printf("sched_period: %s ns\n",dvalue);
                attr.sched_period = (uint32_t)atoi(dvalue);
                dvalue = strtok(NULL,",");

                break;

            case '?':
                if (optopt == 'c')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                fprintf (stderr,
                        "Unknown option character `\\x%x'.\n",
                        optopt);
                return 1;

            default:
                fprintf(stderr,"%s [-s SCHED_TYPE] [-d SCHED_ATTR] input_flux output_flux\n",argv[0]);
                abort ();
        }
    }

    if(deadline_flag){
       if(sched_setattr(getpid(),&attr,0)){
           perror("sched_setattr()");
           exit(EXIT_FAILURE);
       } 
    }

    if(debug_flag){
        // ADD routine for debug
        printf("Debug mode activated\n");
        inFile = "240p/02_Sintel.ulv";
        outMem = "/mem1";
    }    
    else {
        if(argc == 1){
            fprintf(stderr,"%s [-s SCHED_TYPE] [-d SCHED_ATTR] input_flux output_flux\n",argv[0]);
            exit(EXIT_FAILURE);        
        }
        printf("%d\n",optind);
        inFile = (char*)argv[optind];
        outMem = (char*)argv[optind+1];    
    }

    // algorithm variable
    if (mlockall(MCL_CURRENT | MCL_FUTURE ))
    {
        perror("mlockall failed:");
    }
    int   offset   = 0;
    int   frameSize = 0;
    int frameCount = 0;

    uint32_t compressed_image_size;
    uint32_t decompressed_image_size = 0;
    int actual_comp = 0;

    struct stat inFile_stat_info;
    struct videoInfos vidInfos;
    struct memPartage memPartage;
    struct memPartageHeader memPartageHeader;

    memPartageHeader.frameReader = 0;
    memPartageHeader.frameWriter = 0;

    // start of the algorithm
    int fd = open(inFile, O_RDONLY);
    if(fd < 0){
        printf("failed to open file\n");
        exit(EXIT_FAILURE);
    }
    

    fstat(fd, &inFile_stat_info);
    char* inFile_mem = (char*)mmap(NULL, inFile_stat_info.st_size, PROT_READ, MAP_POPULATE | MAP_PRIVATE, fd, 0);

    // Validate header
    if(strncmp(header, inFile_mem, HEADER_SIZE) <0){
        printf("Invalid Header\n");
        exit(EXIT_FAILURE);
    }
    offset += 4;

    // Parsing video infos (l,h,c,fps) into memPartagerHeader
    memcpy(&vidInfos.largeur, inFile_mem + offset,4);
    memPartageHeader.largeur = vidInfos.largeur;
    offset += 4;
    memcpy(&vidInfos.hauteur, inFile_mem + offset,4);
    memPartageHeader.hauteur = vidInfos.hauteur;
    offset += 4;
    memcpy(&vidInfos.canaux, inFile_mem + offset, 4);
    memPartageHeader.canaux = vidInfos.canaux;
    offset += 4;
    memcpy(&vidInfos.fps, inFile_mem + offset, 4);
    memPartageHeader.fps = vidInfos.fps;
    offset += 4;
    printf("video largeur: %d \nvideo hauteur: %d \nvideo nbr cannaux: %d \nvideo fps: %d \n", vidInfos.largeur, vidInfos.hauteur, vidInfos.canaux, vidInfos.fps);

    // compute frameSize (l*h*c) and prepare memory pool
    frameSize = vidInfos.largeur*vidInfos.hauteur*vidInfos.canaux;
    
    
    if(prepareMemoire(frameSize,frameSize)<0){
        printf("Failed to init memory pool\n");
        exit(EXIT_FAILURE);
    } 

    if(initMemoirePartageeEcrivain(outMem, &memPartage, frameSize, &memPartageHeader)){
        printf("Failed to init shared memory\n");
        exit(EXIT_FAILURE);
    }


    memPartage.header->frameWriter = 0;

    // Decoding loop
    while (1) {

        // looping to begining of the file, if end reached
        if (offset >= inFile_stat_info.st_size - 4) {
            offset = 20;
        }

        memcpy(&compressed_image_size, inFile_mem + offset, 4);
        offset += 4;

        unsigned char *frame = (unsigned char *)tempsreel_malloc(compressed_image_size);
        //decode jpg img

        frame = jpgd::decompress_jpeg_image_from_memory((const unsigned char*) (inFile_mem + offset), compressed_image_size, &vidInfos.largeur, &vidInfos.hauteur, &actual_comp, memPartage.canaux);
        
        memPartage.header->largeur = vidInfos.largeur;
        memPartage.header->hauteur = vidInfos.hauteur;
        memPartage.header->canaux = actual_comp;

        decompressed_image_size = memPartage.header->largeur*memPartage.header->hauteur*actual_comp;


        memcpy(memPartage.data, frame, decompressed_image_size);

        //liberation du mutex et mise a jour de notre index prive
        offset += compressed_image_size;
        memPartage.copieCompteur = memPartage.header->frameReader;
        pthread_mutex_unlock(&memPartage.header->mutex);

        attenteEcrivain(&memPartage); //on attend apres le lecteur
        memPartage.header->frameWriter++;
        tempsreel_free(frame);

    }

    return 0;
}
