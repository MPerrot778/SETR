// Gestion des ressources et permissions
#include <sys/resource.h>

// Nécessaire pour pouvoir utiliser sched_setattr et le mode DEADLINE
#include <sched.h>
#include "schedsupp.h"
#include <ctype.h>
#include <stdio.h>

#include "allocateurMemoire.h"
#include "commMemoirePartagee.h"
#include "utils.h"

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

int setOrd(int type, const struct sched_param *param){
    return sched_setscheduler(getpid(), type, param);
}


int main(int argc, char* argv[]){
    
    // Écrivez le code de décodage et d'envoi sur la zone mémoire partagée ici!
    // N'oubliez pas que vous pouvez utiliser jpgd::decompress_jpeg_image_from_memory()
    // pour décoder une image JPEG contenue dans un buffer!
    // N'oubliez pas également que ce décodeur doit lire les fichiers ULV EN BOUCLE

    int aflag = 0;
    int bflag = 0;
    int sflag = 0;
    int dflag = 0;
    char *cvalue, *svalue, *dvalue = NULL;
    int index;
    int c;
    int i = 0;
    int ret;

    char schedType[4][10] = {"NORT", "RR", "FIFO", "DEADLINE"};
    int schedPolicy[4] = {0, 2, 1, 6};
    struct sched_param *p;
    
    // -s, -d, --debug
    while ((c = getopt (argc, argv, "as:d::")) != -1){
        switch (c){
            case 'a':
                aflag = 1;
            case 's':
                sflag = 1;
                svalue = optarg;
                while(i<4){
                    if(strcmp(schedType[i],svalue) == 0){
                        break;
                    }
                    i++;
                }
                if(i==4){
                    printf("Not valid scheduler policy\n");
                    exit(1);
                }
                ret = setOrd(schedPolicy[i],p);
                if(ret < 0){
                    printf("error no: %d\n",errno);
                    exit(1);
                }
                printf("scheduling done : %d\n",ret);
                break;

            case 'd':
                if(sflag){
                    printf("it worked\n");
                }
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
                abort ();
        }
    }
    for (index = optind; index < argc; index++)
        printf ("Non-option argument %s\n", argv[index]);

    return 0;
}
