/*
	C ECHO client example using sockets
*/
#define _XOPEN_SOURCE 600
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <stdio.h>  //printf
#include <string.h> //strlen
#include <ifaddrs.h>
#include <stdlib.h>

#define ADDR_MAX 32

int main(int argc, char *argv[])
{
	if (argc <= 2) {
		printf("\n*** Veuillez saisir une adresse IP puis un nom de machine en argument svp ***\n\n");
		return 0;
    }
	int sock;
	struct sockaddr_in server;
	char message[1000], server_reply[2000], name[100];

	static char buf[ADDR_MAX];
	struct ifaddrs *list;
	struct ifaddrs *it;
	it = list;


	//Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket créée"); 

	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);

	//Connect to remote server
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}

	puts("Connecté");
	printf("\n---------- IPv4 ----------  \n\n");
	if (getifaddrs(&list) != 0)
	{
		perror("getifaddrs");
		return EXIT_FAILURE;
	}	

	// Envoi du nom de la machine

	strcat(message,"\t -> ");
	strcat(message, argv[2]);
	strcat(message, "                                ");
	send(sock, message, strlen(message), 0);
	
	printf("   Envoi nom :%s\n",message);
	message[0] = '\0';
	if (recv(sock, server_reply, 2000, 0) < 0)
	{
		puts("recv failed");
		return 10;
	}
	printf("Réponse nom : %s \n\n", server_reply);

	it = list;
	//keep communicating with server

		while (it != NULL)
		{
			struct sockaddr_in *addr;

			addr = (struct sockaddr_in *)it->ifa_addr;
			if (addr != NULL && it->ifa_addr->sa_family == AF_INET)
			{
				if (inet_ntop(AF_INET, &addr->sin_addr, buf, ADDR_MAX) == NULL)
				{
					perror("inet_ntop");
					exit(EXIT_FAILURE);
				}
				//puts("test2");
				strcat(message, "\t");
				strcat(message, it->ifa_name);
				strcat(message, " :\t");
				strcat(message, buf);
				strcat(message, "                                       ");

				printf("   Envoi : %s \n",message);

				//Send some data
				if (send(sock, message, strlen(message), 0) < 0)
				{
					puts("Send failed");
					return 10;
				}
				message[0] = '\0';

				//Receive a reply from the server
				if (recv(sock, server_reply, 2000, 0) < 0)
				{
					puts("recv failed");
					break;
				}

				printf("Réponse : %s\n\n", server_reply);
			}
			it = it->ifa_next;
		}

	close(sock);
	return 0;
}	