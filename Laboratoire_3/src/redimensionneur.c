// Gestion des ressources et permissions
#include <sys/resource.h>

// Nécessaire pour pouvoir utiliser sched_setattr et le mode DEADLINE
#include <sched.h>
#include "schedsupp.h"

#include "allocateurMemoire.h"
#include "commMemoirePartagee.h"
#include "utils.h"


int main(int argc, char* argv[]){
    
    // Écrivez le code permettant de redimensionner une image (en utilisant les fonctions précodées
    // dans utils.c, celles commençant par "resize"). Votre code doit lire une image depuis une zone 
    // mémoire partagée et envoyer le résultat sur une autre zone mémoire partagée.
    // N'oubliez pas de respecter la syntaxe de la ligne de commande présentée dans l'énoncé.

    int c;
    char *wvalue;
    char *hvalue;
    char *mvalue;
    char *svalue;
    char *dvalue;
    char *inFile;
    char *outMem;
    int deadline_flag, out_height, out_width, mode = 0;
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
    while ((c = getopt_long(argc, argv, "s:d:w:h:m", long_options, &option_index)) != -1){
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

            case 'w':
                wvalue = optarg;
                out_width = atoi(wvalue);
                break;
            case 'h':
                hvalue = optarg;
                out_height = atoi(hvalue);
                break;
            case 'm':
                mvalue = optarg;
                mode = atoi(mvalue);                
                break;
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

    char* memInPath = argv[optc];
    char* memOutPath = argv[optc+1];

    int compteur_lecteur = 0;
    int compteur_ecrivain = 0;

    struct memPartage memIn,memOut;
    if (initMemoirePartageeLecteur(memInPath, &memIn))
        ErrorExit("initMemoireLecteur - redim");

    pthread_mutex_lock(&memIn.header->mutex);

    size_t size = out_width * out_height * memIn.header->canaux;

    struct memPartageHeader header;
    header.frameWriter = 0;
    header.frameReader = 0;
    header.hauteur = out_height;
    header.largeur = out_width;
    header.fps = memIn.header->fps;
    header.canaux = memIn.header->canaux;

    if (initMemoirePartageeEcrivain(memOutPath,&memOut,size,&header))
        ErrorExit("initMemoireEcrivain - redim");
    
    pthread_mutex_lock(&memOut.header->mutex);

    int in_height = memIn.header->hauteur;
    int in_width = memIn.header->largeur;
    int canal = memIn.header->canaux;

    ResizeGrid grid;
    if (mode == 0) {
        grid = resizeNearestNeighborInit(out_height,out_width,in_height,in_width);
    } else if (mode == 1) {
        grid = resizeBilinearInit(out_height,out_width,in_height,in_width);        
    } else {
        ErrorExit("mode: 0 ou 1");
    }

    while(1) {
        if (mode == 0) {
            resizeNearestNeighbor(memIn.data,in_height,in_width,memOut.data,out_height,out_width,grid,canal);
        } else {
            resizeBilinear(memIn.data,in_height,in_width,memOut.data,out_height,out_width,grid,canal);
        }

        compteur_ecrivain = memIn.header->frameWriter;
        memIn.header->frameReader++;
        pthread_mutext_unlock(&memIn.header->mutex);

        compteur_lecteur = memOut.header->frameReader;
        memOut.header->frameWriter++;
        pthread_mutex_unlock(&memOut.header->mutex);

        attenteEcrivain(&memIn);
        attenteLecteur(&memOut);
    }

    return 0;
}