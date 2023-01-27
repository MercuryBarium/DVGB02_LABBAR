#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#include <signal.h> // For cleaning up after keyboard interrupt
#define MAX 80
#define PORT 8080
#define REQ_LEN 8190

int running = 1;

void sigint_handler(int sig)
{
    running = 0;
    //signal(SIGINT, sigint_handler);
    printf("Stopping sockets.");
    //fflush(stdout);

}

int main(int argc, char const *argv[])
{
    struct sockaddr_in this_socket_addr, client_addr;
    signal(SIGINT, sigint_handler);
    memset(&this_socket_addr, 0, sizeof(struct sockaddr_in));
    this_socket_addr.sin_family=AF_INET;
    this_socket_addr.sin_port=htons(PORT);          // htons = host to network short unsigned 16 bit
    this_socket_addr.sin_addr.s_addr= INADDR_ANY;
    
    int my_socket = socket(AF_INET, SOCK_STREAM, 0);
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
    
    char buf[REQ_LEN];



    while (running)
    {
        int connection = accept(my_socket, (struct sockaddr_in *)&client_addr, (socklen_t*)&client_addr_len);

        if (connection != -1)
        {
            read(connection, buf, REQ_LEN);
            buf[REQ_LEN-1]='\0';
            char *resp="HTTP/1.1 200 OK\r\nServer: Demo\r\nContent-Length: 1024\r\nContent-Type: text/html\r\n\r\n<h1>Hello World</h1>";
            write(connection, resp, REQ_LEN);
            printf("%s\n", buf);
        }
        close(connection);


    }

    close(my_socket);
    return 0;
}

