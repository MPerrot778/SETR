/* TP2 2020 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Pour recuperer les descriptions d'erreur
#include <errno.h>

// Multiprocessing
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>

// Sockets UNIX
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>

// Signaux UNIX
#include <signal.h>


// Structures et fonctions de communication
#include "communications.h"
// Fonction de téléchargement utilisant cURL
#include "telechargeur.h"
// Structures et fonctions stockant et traitant les requêtes en cours
#include "requete.h"
// Fonctions de la boucle principale
#include "actions.h"

// Chaînes de caractères représentant chaque statut (utile pour l'affichage)
const char* statusDesc[] = {"Inoccupe", "Connexion client etablie", "En cours de telechargement", "Pret a envoyer"};

// Nombre maximal de connexions simultanés
#define MAX_CONNEXIONS 10
// Contient les requetes en cours de traitement
struct requete reqList[MAX_CONNEXIONS];

static void gererSignal(int signo) {
    // Fonction affichant des statistiques sur les tâches en cours
    // lorsque SIGUSR1 (et _seulement_ SIGUSR1) est reçu
    // TODO

}

int main(int argc, char* argv[]){    
    // Chemin du socket UNIX
    // Linux ne supporte pas un chemin de plus de 108 octets (voir man 7 unix)
    char path[108] = "/tmp/unixsocket";
    struct sockaddr_un addr;                    // struct de type sockaddr_un

    if(argc > 1)                                // On peut également le passer en paramètre
        strncpy(path, argv[1], sizeof(path));
    unlink(path);                               // Au cas ou le fichier liant le socket UNIX existerait deja

    // On initialise la liste des requêtes
    memset(&reqList, 0, sizeof(reqList));

    // TODO
    // Implémentez ici le code permettant d'attacher la fonction "gereSignal" au signal SIGUSR1
    struct sigaction sig;
    memset(&sig, '\0', sizeof(sig));
    sig.sa_handler = &gererSignal;
    if (sigaction(SIGUSR1, &sig, NULL) == -1) {
        strerror(errno);
        exit(EXIT_FAILURE);
    }
    
    memset(&addr, 0, sizeof(addr));                             // initialise addr a 0
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);
    
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);  // creation du socket
    if (sock == -1) {                           
        strerror(errno);                         // affiche l'erreur
        exit(EXIT_FAILURE);                      // quitter le processus
    }

    // fcntl() pour mettre le socket en mode non-bloquant
    // Vérifiez si l'opération a été effectuée avec succès, sinon quittez le processus en affichant l'erreur
    int flags = fcntl(sock, F_GETFL, 0);                   
    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1) {
        strerror(errno);
        exit(EXIT_FAILURE);
    }
    
    // bind sur le socket
    // Vérifiez si l'opération a été effectuée avec succès, sinon quittez le processus en affichant l'erreur
    if (bind(sock, (struct sockaddr_on*) &addr, sizeof(addr)) == -1) {
        strerror(errno);
        exit(EXIT_FAILURE);
    }

    // TODO
    // 5) Mettez le socket en mode écoute (listen), en acceptant un maximum de MAX_CONNEXIONS en attente
    //      Vérifiez si l'opération a été effectuée avec succès, sinon quittez le processus en affichant l'erreur
    //      Voyez man listen pour plus de détails sur cette opération
    if (listen(sock, MAX_CONNEXIONS) == -1) {
        strerror(errno);
        exit(EXIT_FAILURE);
    }

    // Initialisation du socket UNIX terminée!

    // Boucle principale du programme
    int tacheRealisee;
    while(1){
        // On vérifie si de nouveaux clients attendent pour se connecter
        tacheRealisee = verifierNouvelleConnexion(reqList, MAX_CONNEXIONS, sock);

        // On teste si un client vient de nous envoyer une requête
        // Si oui, on la traite
        tacheRealisee += traiterConnexions(reqList, MAX_CONNEXIONS);

        // On teste si un de nos processus enfants a terminé son téléchargement
        // Dans ce cas, on traite le résultat
        tacheRealisee += traiterTelechargements(reqList, MAX_CONNEXIONS);

        // Si on a des données à envoyer au client, on le fait
        tacheRealisee += envoyerReponses(reqList, MAX_CONNEXIONS);

        // Si on n'a pas attendu dans un select ou effectué une tâche, on ajoute
        // un petit delai ici pour éviter d'utiliser 100% du CPU inutilement
        if(tacheRealisee == 0)
            usleep(SLEEP_TIME);
    }

    return 0;
}
