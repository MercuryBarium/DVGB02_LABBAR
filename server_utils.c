#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
    char dir[100] = "./static";
    strcat(dir, file_name);
    if (access(dir, F_OK) == 0)
        return 1;
    else
        return 0;
}

enum http_codes gen_response(char *request_str, char *response_str)
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
    method[i] = '\0';    // Ends the string instead of space
    i++;    // To move on from the space
    for (; i < req_len; i++)
    {
        if (request_str[i]==' ')
            break;
        else
            strncat(file_n, &request_str[i], 1);
    }
    
    if (strcmp(method, "GET") == 0)
    {
        if (find_file(file_n))
        {   
            int type = find_file_type(file_n);
            if (type == illegal_t)
            {
                strcat(response_str, HTTP_CODES[http_404]);
                return http_404;
            } else 
            {
                strcat(response_str, HTTP_CODES[http_200]);
                strcat(response_str, "Server: Demo Web Server\r\n");

                if (type == html_t)
                    strcat(response_str, "Content-Type: text/html\r\n");
                else
                    strcat(response_str, "Content-Type: image/jpeg\r\n");

                char dir[50] = "./static";
                strcat(dir, file_n);
                FILE * fp = fopen(dir, "r");
                fseek(fp, 0L, SEEK_END);
                unsigned long int size = ftell(fp);
                
                char s[30] = "\0";
                sprintf(s, "Content-Size: %lu\r\n\r\n", size);
                strcat(response_str, s);
                
                // Must realloc for big files. if size > REQ_LEN etc...
                fseek(fp, 0, SEEK_SET);                                 // Resetting file pointer to beginning
                char c;
                while ((c=getc(fp))!=EOF)
                {
                    strncat(response_str, &c, 1);
                }
                
                return http_200;
            }           
        } else 
        {
            strcat(response_str, HTTP_CODES[http_404]);
            return http_404;
        }
    } else 
    {
        strcat(response_str, HTTP_CODES[http_404]);
        return http_404;
    }
    return http_404;
}