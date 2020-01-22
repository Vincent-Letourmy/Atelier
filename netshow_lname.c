#include "ifaddrs.h"
#include <stdio.h>
#include <stdlib.h>

int main(void){

    // Création struct ifaddrs : interfaces de réseaux, ifa = courante
    struct ifaddrs *ifaddr, *ifa;
    int family, s, n;

    // On exécute getifaddrs(&ifaddr) pour récup les adresses des interfaces
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Interfaces réseaux: \n");

    // Pour chaque addresses
    for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
        
        // Si l'adresse n'est pas nulle et qu'elle est IPV4 (pour éviter les doublons)
        if (ifa->ifa_addr != NULL && ifa->ifa_addr->sa_family == AF_INET){  
        
            // Et on affiche le nom de l'adresse courante
            printf("  %-8s \n", ifa->ifa_name);
        }
    }

    // On libère l'espace et on sort
    freeifaddrs(ifaddr);
    exit(EXIT_SUCCESS);
}