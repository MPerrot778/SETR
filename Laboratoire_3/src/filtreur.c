// Gestion des ressources et permissions
#include <sys/resource.h>

// Nécessaire pour pouvoir utiliser sched_setattr et le mode DEADLINE
#include <sched.h>
#include "schedsupp.h"

#include "allocateurMemoire.h"
#include "commMemoirePartagee.h"
#include "utils.h"


int main(int argc, char* argv[]){
    
    // Écrivez le code permettant de filtrer une image (en utilisant les fonctions précodées
    // dans utils.c). Votre code doit lire une image depuis une zone mémoire partagée et
    // envoyer le résultat sur une autre zone mémoire partagée.
    // N'oubliez pas de respecter la syntaxe de la ligne de commande présentée dans l'énoncé.
    void (*filter_function) (const unsigned int height, const unsigned int width, const unsigned char *input, unsigned char *output,
                   const unsigned int kernel_size, float sigma, const unsigned int n_channels);

    int c;
    char *svalue;
    char *dvalue;
    int deadline_flag = 0;
	int nbrActifs = 0;
	size_t frame = 0;
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
                attr.sched_runtime = atoi(dvalue);
                dvalue = strtok(NULL,",");

                printf("sched_deadline: %s ns\n",dvalue);
                attr.sched_deadline = atoi(dvalue);
                dvalue = strtok(NULL,",");

                printf("sched_period: %s ns\n",dvalue);
                attr.sched_period = atoi(dvalue);
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
            case 't':
                if (strtol(optarg,NULL,10) == 0) {
                    filter_function = lowpassFilter;
                } else {
                    filter_function = highpassFilter;
                }
                break;
            default:
                fprintf(stderr,"%s [-s SCHED_TYPE] [-d SCHED_ATTR] input_flux1 [input_flux2] [input_flux3] [input_flux4]\n",argv[0]);
                abort ();
        }
    }

    if(deadline_flag){
       if(sched_setattr(getpid(),&attr,0)){
           perror("sched_setattr()");
           exit(EXIT_FAILURE);
       } 
    }

	if((argc - optind) < 1 || (argc - optind) > 4){
		fprintf(stderr,"%s [-s SCHED_TYPE] [-d SCHED_ATTR] input_flux1 [input_flux2] [input_flux3] [input_flux4]\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	nbrActifs = argc - optind;	

    if(debug_flag){
        // ADD routine for debug
        printf("Debug mode activated\n");
    }  

    struct memPartage lecteur;
    struct memPartage ecrivain;

    int compteur_lecteur = 0;
    int compteur_ecrivain = 0;

    if (initMemoirePartageeLecteur(argv[optind], &lecteur) != 0)
        ErrorExit("initMemoireLectuer - filtreur");

    pthread_mutex_lock(&lecteur.header->mutex);
    
    uint32_t largeur = lecteur.header->largeur;
    uint32_t hauteur = lecteur.header->hauteur;
    uint32_t canaux = lecteur.header->canaux;
    size_t tailleDonnes = largeur*hauteur*canaux;

    prepareMemoire(lecteur.tailleDonnees, 2*tailleDonnes);

    if (initMemoirePartageeEcrivain(argv[optind+1], &ecrivain, tailleDonnes,lecteur.header) != 0)
        ErrorExit("initMemoireEcrivain - filtreur");

    pthread_mutex_lock(&ecrivain.header->mutex);

    while(1) {

        lecteur.header->frameReader += 1;
        compteur_ecrivain= lecteur.header->frameWriter;
        pthread_mutex_unlock(&lecteur.header->mutex);

        filter_function(hauteur, largeur, lecteur.data, ecrivain.data, 5, 10, canaux);

        ecrivain.header->frameWriter += 1;
        ecrivain.copieCompteur = ecrivain.header->frameReader;

        compteur_lecteur = ecrivain.header->frameReader;
        pthread_mutex_unlock(&ecrivain.header->mutex);
        while(compteur_ecrivain == lecteur.header->frameWriter);
        pthread_mutex_lock(&lecteur.header->mutex);
        while(compteur_lecteur == ecrivain.header->frameReader);
        pthread_mutex_lock(&ecrivain.header->mutex);
    }

    return 0;
}