#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define INTERFACE_NAME_LENGTH 20
#define ADDR_MAX 32


void getinfos(struct ifaddrs *current_interface, char * ipaddress_buffer, char * mask_buffer) {
    struct sockaddr_in *ipaddress;
    struct sockaddr_in *mask;

    ipaddress = (struct sockaddr_in *)current_interface->ifa_addr;
    mask = (struct sockaddr_in *)current_interface->ifa_netmask;
    if (ipaddress->sin_family == AF_INET || ipaddress->sin_family == AF_INET6) {
        printf((ipaddress->sin_family == AF_INET) ? "IPv4 : " : "IPv6 : ");
        if (inet_ntop(ipaddress->sin_family, &ipaddress->sin_addr, ipaddress_buffer, ADDR_MAX) == NULL) {
            perror("ERROR GETTING IP ADDRESS");
            exit(EXIT_FAILURE);
        }
        if (inet_ntop(ipaddress->sin_family, &mask->sin_addr, mask_buffer, ADDR_MAX) == NULL) {
            perror("ERROR GETTING IP MASK");
            exit(EXIT_FAILURE);
        }
        printf("%s   \t%s\n",ipaddress_buffer,mask_buffer);
    }
}


int main(int argc, char *argv[]) {

    typedef struct ifaddrs ifaddrs;

    ifaddrs *list_all_interfaces, *cleaner;
    char *array[INTERFACE_NAME_LENGTH];
    char ** interfaces_names = array;
    int interfaces_names_size = 0;

    if(getifaddrs(&list_all_interfaces)==-1){
        perror("Not any network interface were found on the host machine :'(");
        exit(EXIT_FAILURE);
    }

    cleaner = list_all_interfaces;

    printf("Network interfaces :\n");

    while(list_all_interfaces != NULL) {

        bool found = false;
        for (int i = 0; i < interfaces_names_size; ++i) {
            if (strcmp(list_all_interfaces->ifa_name,interfaces_names[i]) == 0) {
                found = true;
            }
        }
        if (!found) {
            interfaces_names[interfaces_names_size] = list_all_interfaces->ifa_name;
            interfaces_names_size++;
            printf("%i : %-8s \n",interfaces_names_size ,list_all_interfaces->ifa_name);
        }
        list_all_interfaces=list_all_interfaces->ifa_next;
    }

    freeifaddrs(cleaner);

    char string[INTERFACE_NAME_LENGTH];
    char *targetinterfacename = string;
    if (argc > 1) {
        targetinterfacename = argv[1];
    } else {
        int args = argc;
        while (args <= 1) {
            printf("Please enter the name of an interface to get some details : ");
            scanf("%s", targetinterfacename);
            if (targetinterfacename[0] != '\0') { args++; }
        }
    }
    printf("Interface: %s\n", targetinterfacename);

    static char ipaddress_buffer[ADDR_MAX];
    static char mask_buffer[ADDR_MAX];
    struct ifaddrs *current_interface;

    if (getifaddrs(&list_all_interfaces) != 0) {
        perror("Not any network interface were found on the host machine :'(");
        return EXIT_FAILURE;
    }

    current_interface = list_all_interfaces;

    while (current_interface != NULL) {
        if(strcmp(current_interface->ifa_name, targetinterfacename) == 0
           && current_interface->ifa_addr != NULL) {
            getinfos(current_interface, ipaddress_buffer, mask_buffer);
        }
        current_interface = current_interface->ifa_next;
    }

    freeifaddrs(list_all_interfaces);

    return 0;
} 