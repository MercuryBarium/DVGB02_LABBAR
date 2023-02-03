#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#include <signal.h> // For cleaning up after keyboard interrupt
#include "server_utils.h"
#define MAX 80
#define PORT 8080


int my_socket;
int connection;

void sigint_handler(int sig)
{
    signal(SIGINT, sigint_handler);
    printf("Stopping sockets.");
    close(connection);
    close(my_socket);
    fflush(stdout);
    exit(0);
}

int main(int argc, char const *argv[])
{
    struct sockaddr_in this_socket_addr, client_addr;
    signal(SIGINT, sigint_handler);
    memset(&this_socket_addr, 0, sizeof(struct sockaddr_in));
    this_socket_addr.sin_family=AF_INET;
    this_socket_addr.sin_port=htons(PORT);          // htons = host to network short unsigned 16 bit
    this_socket_addr.sin_addr.s_addr= INADDR_ANY;
    
    my_socket = socket(AF_INET, SOCK_STREAM, 0);
    const int enable = 1;
    if (setsockopt(my_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        printf("setsockopt(SO_REUSEADDR) failed");
    if (my_socket == -1)
    {
        printf("Socket Creation failed");
        exit(-1);
    }
    
    if (bind(my_socket, (struct sockaddr *)&this_socket_addr, sizeof(this_socket_addr)) != -1)
        printf("Socket successfully bound to address %X:%d\n", this_socket_addr.sin_addr.s_addr, PORT);
    else
    {
        printf("Socket failed to bind!\n");
        exit(-1);
    }

    listen(my_socket, SOMAXCONN);
    int client_addr_len = sizeof(struct sockaddr_in);
    char *request_str = (char*)malloc(sizeof(char)*REQ_LEN);    
    while (1)
    {
        connection = accept(my_socket, (struct sockaddr_in *)&client_addr, (socklen_t*)&client_addr_len);
        if (connection != -1)
        {   
            request_str[0]='\0';
            read(connection, request_str, REQ_LEN);
            send_response(request_str, connection);
            
        }
        close(connection);
    }
    free(request_str);
    close(my_socket);
    return 0;
}

