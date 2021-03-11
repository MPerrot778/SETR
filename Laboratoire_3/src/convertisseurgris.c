// Gestion des ressources et permissions
#include <sys/resource.h>

// Nécessaire pour pouvoir utiliser sched_setattr et le mode DEADLINE
#include <sched.h>
#include "schedsupp.h"

#include "allocateurMemoire.h"
#include "commMemoirePartagee.h"
#include "utils.h"


int main(int argc, char* argv[]){
    
    // Écrivez le code permettant de convertir une image en niveaux de gris, en utilisant la
    // fonction convertToGray de utils.c. Votre code doit lire une image depuis une zone mémoire 
    // partagée et envoyer le résultat sur une autre zone mémoire partagée.
    // N'oubliez pas de respecter la syntaxe de la ligne de commande présentée dans l'énoncé.

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

    struct memPartage lecteur;
    struct memPartage ecrivain;

    if (initMemoirePartageeLecteur(argv[optind], &lecteur) != 0)
        ErrorExit("initMemoireLectuer - gris");

    prepareMemoire(lecteur.tailleDonnees,0);

    struct memPartageHeader header_ecrivain;

    header_ecrivain.header->largeur = lecteur.header->largeur;
    header_ecrivain.header->hauteur = lecteur.header->largeur;
    header_ecrivain.header->canaux = 1;
    header_ecrivain.fps = lecteur.header->fps;
    size_t tailleDonnees = header_ecrivain.header->largeur * header_ecrivain.hauteur->hauteur;

    if (initMemoirePartageeEcrivain(argv[optind+1], &ecrivain, tailleDonnees, &header_ecrivain) != 0)
        ErrorExit("initMemoireEcrivain - gris");

    int in_height = lecteur.header->hauteur;
    int in_width = lecteur.header->largeur;

    while(1) {

        attenteLecteur(&lecteur);
        lecteur.header->frameReader++;

        convertToGray(lecteur.data, in_height,in_width,lecteur.header->canaux ,ecrivain.data);

        lecteur.copieCompteur = lecteur.header->frameWriter;
        // rendu la
        compteur_ecrivain = ecrivain.header->frameReader;
        pthread_mutex_unlock(&ecrivain.header->mutex);
        while(compteur_ecrivain == lecteur.header->frameWriter);
        pthread_mutex_lock(&lecteur.header->mutex);
        while(compteur_lecteur == ecrivain.header->frameReader);
        pthread_mutex_lock(&ecrivain.header->mutex);
    }

    return 0;
}