#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "server_utils.h"

static char *HTTP_CODES[] = {
    "HTTP/1.0 200 OK\r\n",
    "HTTP/1.0 404 Not Found\r\nContent-Type: text/html\r\n\r\n<h1>404 Not Found</h1>\n",
    "HTTP/1.0 301 Moved Permanently\r\nLocation: /index.html\r\n",
    "HTTP/1.0 400 Bad Request\r\n"
};

static char *CONTENT_TYPE[] = {
    "--Illegal--",
    "Content-Type: text/html\r\n\r\n",
    "Content-Type: \r\n\r\n"
};

file_type find_file_type(char *filename) 
{
    char *dot = strrchr(filename, '.');
    if (dot == NULL)
        return illegal_t;
    else if (strcmp(dot, ".html") == 0)
        return html_t;
    else if (strcmp(dot, ".jpg") == 0)
        return jpeg_t;
    else
        return illegal_t;
}

int find_file(char *file_name)
{
    char dir[100] = "./static";
    strcat(dir, file_name);
    if (access(dir, F_OK) == 0)
        return open(dir, O_RDONLY);
    else
        return -1;
}

void response404(int connection)
{
    write(connection, HTTP_CODES[http_404], strlen(HTTP_CODES[http_404]));
}

void send_response(char *request_str, int connection)
{
    //Kolla så metoden stämmer 
    int req_len = strlen(request_str);
    char method[50] = "\0";
    char file_n[50] = "\0";
    int i;
    for (i = 0; i < req_len; i++)
    {
        if (request_str[i]==' ')
            break;
        else
            strncat(method, &request_str[i], 1);
    }
    i++;
    for (; i < req_len; i++)
    {
        if (request_str[i]==' ')
            break;
        else
            strncat(file_n, &request_str[i], 1);
    }

    if (strcmp(method, "GET") == 0)
    {
        int fd;
        if ((fd = find_file(file_n))!=-1)
        {
            write(connection, HTTP_CODES[http_200], strlen(HTTP_CODES[http_200]));
            file_type ext = find_file_type(file_n);
            if (ext)
            {
                write(connection, CONTENT_TYPE[ext], strlen(CONTENT_TYPE[ext]));
                char buf[BUFFER_SIZE];
                int s;
                while ((s=read(fd, &buf, BUFFER_SIZE)))
                    write(connection, &buf, s);
                return;
            }
        }
        
        write(connection, HTTP_CODES[http_404], strlen(HTTP_CODES[http_404]));
        return;
    } else 
    {
        write(connection, HTTP_CODES[http_400], strlen(HTTP_CODES[http_400]));
        return;
    }

}