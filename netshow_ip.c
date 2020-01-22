#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _XOPEN_SOURCE 600
#define ADDR_MAX 32


int main(int argc, char *ifname[]){

        // ************* Nom interface ***********************************************************

        int i;
        char *vide="x";
        char ligne[] = "+----------------------------------------------------------+\n";
        if (argc <= 1) {
                printf("\n*** Veuillez saisir une interface en argument svp ***\n\n");
                ifname[1]="x";
                return 0;
        }
        else{
                printf("\n  Interface: %s\n%s", ifname[1], ligne);
        }

        static char buf[ADDR_MAX];
        static char buf2[ADDR_MAX];
        struct ifaddrs *list;
        struct ifaddrs *it;


        // ************* IPV4 *******************************************************************
        printf("  IPv4: ");
        if (getifaddrs(&list) != 0) {
                perror("getifaddrs");
                return EXIT_FAILURE;
        }
        it = list;
        while (it != NULL) {
                struct sockaddr_in *addr;
                struct sockaddr_in *masq;

                addr = (struct sockaddr_in *)it->ifa_addr;
                masq = (struct sockaddr_in *)it->ifa_netmask;
                if (addr != NULL && it->ifa_addr->sa_family == AF_INET) {
                        if (inet_ntop(AF_INET, &addr->sin_addr, buf, ADDR_MAX) == NULL) {
                                perror("inet_ntop");
                                exit(EXIT_FAILURE);
                        }
                        if (inet_ntop(AF_INET, &masq->sin_addr, buf2, ADDR_MAX) == NULL) {
                                perror("inet_ntop2");
                                exit(EXIT_FAILURE);
                        }
                        if(strcmp(ifname[1],vide)==0){
                                printf("%s   \t%s\n",buf,buf2);
                        }
                        else if (strcmp(ifname[1],it->ifa_name)==0){
                                printf("%s   \t%s\n",buf,buf2);
                        }
                        else{;}   
                }
                it = it->ifa_next;
        }
        freeifaddrs(list);


         // ************* IPV6 *******************************************************************

        printf("  IPv6: ");
        if (getifaddrs(&list) != 0) {
                perror("getifaddrs");
                return EXIT_FAILURE;
        }
        it = list;
        //printf('%s',it);
        while (it != NULL) {
                struct sockaddr_in *addr;
                struct sockaddr_in *masq;

                        addr = (struct sockaddr_in *)it->ifa_addr;     
                        masq = (struct sockaddr_in *)it->ifa_netmask;     
                        if (addr != NULL && it->ifa_addr->sa_family == AF_INET6) {
                                if (inet_ntop(AF_INET6, &addr->sin_addr, buf, ADDR_MAX) == NULL) {
                                        perror("inet_ntop");
                                        exit(EXIT_FAILURE);
                                }
                                if (inet_ntop(AF_INET6, &masq->sin_addr, buf2, ADDR_MAX) == NULL) {
                                        perror("inet_ntop");
                                        exit(EXIT_FAILURE);
                                }
                                if(strcmp(ifname[1],vide)==0){
                                printf("%s   \t%s\n",buf,buf2);

                                }
                                else if (strcmp(ifname[1],it->ifa_name)==0){
                                printf("%s   \t%s\n",buf,buf2);
                                }
                                else{;}                
                                
                        }
                it = it->ifa_next;
        }
        freeifaddrs(list);
        printf("%s",ligne);
        return 0;
}