#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "server_utils.h"

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
    char *dir = "./static";
    strcat(dir, file_name);
    FILE * f = fopen(dir, "r");
    if (f == NULL)
    {
        fclose(f);
        return 0;
    } else 
    {
        fclose(f);
        return 1;
    }
    
}

void gen_response(char *request_str, char *buf)
{
    //Kolla så metoden stämmer 
    int req_len = strlen(request_str);
    char *method = "\0";
    char *file_n = "\0";

    char tmp_char[50];
    int i;
    int j=0;
    for (i = 0; i < req_len; i++)
    {
        if (request_str[i]==' ')
            break;
        else
            strncat(method, request_str[i], 1);
    }
    i++;    // To move on from the space
    for (i; i < req_len; i++)
    {
        if (request_str[i]==' ')
            break;
        else
            strncat(file_n, request_str[i], 1);
    }
    if (strcmp(method, "GET") == 0)
    {
        if (find_file(file_n))
        {
            int type = find_file_type(file_n);
            if (type == illegal_t)
            {
                strcat(buf, HTTP_CODES[http_404]);
            } else 
            {
                strcat(buf, HTTP_CODES[http_200]);
                strcat(buf, "Server: Demo Web Server\r\n");
                int length;

                if (type == html_t)
                    strcat(buf, "Content-Type: text/html\r\n");
                else
                    strcat(buf, "Content-Type: image/jpeg\r\n");

                char *dir = "./static";
                strcat(dir, file_n);
                FILE * f = fopen(dir);

                char * content = fread

                strcat(buf, "Content-Length: <length>\r\n");
            }           
        } else 
        {
            strcat(buf, HTTP_CODES[http_404]);
            return;
        }
    } else 
    {
        strcat(buf, HTTP_CODES[http_404]);
        return;
    }
}