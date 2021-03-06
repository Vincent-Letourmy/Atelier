#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADDR_MAX 64


int netshow(void){

        static char buf[ADDR_MAX]; // 32 défini plus haut
        static char buf2[ADDR_MAX]; // 32 défini plus haut
        struct ifaddrs *list;
        struct ifaddrs *it;

        
        // ************* Toutes les interfaces ***************************************************
     
        char ligne[] = "  +-----------+-------------------------------------------------------------+";
        printf("%s\n",ligne);
        printf("    Interface   Address\n");
        printf("%s",ligne);
        if (getifaddrs(&list) != 0) { // doit être égal à zero si pas d'erreur d'execution ; -1 sinon
                perror("getifaddrs");
                return EXIT_FAILURE;
        }
        it = list;
        while (it != NULL) {
                struct sockaddr_in *addr;
                struct sockaddr_in *masq;
                struct sockaddr_in6 *addr6, *masq6;
                

                addr = (struct sockaddr_in *)it->ifa_addr;
                masq = (struct sockaddr_in *)it->ifa_netmask;
                addr6 = (struct sockaddr_in6 *)it->ifa_addr;
                masq6 = (struct sockaddr_in6 *)it->ifa_netmask;
                if (addr != NULL && it->ifa_addr->sa_family == AF_INET) {
                        
                        // *** Méthode inet_ntop(...) ***
                        if (inet_ntop(AF_INET, &addr->sin_addr, buf, ADDR_MAX) == NULL) {
                                perror("inet_ntop");
                                exit(EXIT_FAILURE);
                        }
                        if (inet_ntop(AF_INET, &masq->sin_addr, buf2, ADDR_MAX) == NULL) {
                                perror("inet_ntop");
                                exit(EXIT_FAILURE);
                        }
                        
                        // Affichage = Interface: IP
                        printf("\n    %s     \tIPv4: %s \t\t%s\n", it->ifa_name, buf, buf2);
                        
                }
                else if (addr6 != NULL && it->ifa_addr->sa_family == AF_INET6) {
                        
                        // *** Méthode inet_ntop(...) avec AF_INET6***
                        if (inet_ntop(AF_INET6, &addr6->sin6_addr, buf, ADDR_MAX) == NULL) {
                                perror("inet_ntop");
                                exit(EXIT_FAILURE);
                        }
                        if (inet_ntop(AF_INET6, &masq6->sin6_addr, buf2, ADDR_MAX) == NULL) {
                                perror("inet_ntop");
                                exit(EXIT_FAILURE);
                        }
                
                        // Affichage = Interface: IP
                        printf("\t\tIPv6: %s\t%s\n", buf, buf2);
                                
                        
                }
                it = it->ifa_next;
        }
        freeifaddrs(list);
        printf("%s\n",ligne);

        return 0;
}

int main(void){
        netshow();
        return 0;
}