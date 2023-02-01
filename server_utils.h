#ifndef SERVER_UTILS
#define SERVER_UTILS
#define REQ_LEN 8190

typedef enum {
    illegal_t,
    html_t,
    jpeg_t
} file_type;

enum http_codes {
    http_200,
    http_404
};
static char *HTTP_CODES[] = {
    "HTTP/1.0 200 OK\r\n",
    "HTTP/1.0 404 Not Found\r\n"
};

file_type find_file_type(char *filename);

int find_file(char *file_name);

enum http_codes gen_response(char *request_str, char *response_str);



#endif