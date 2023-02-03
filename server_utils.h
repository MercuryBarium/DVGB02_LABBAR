#ifndef SERVER_UTILS
#define SERVER_UTILS
#define REQ_LEN 8190
#define BUFFER_SIZE 4096
typedef enum {
    illegal_t,
    html_t,
    jpeg_t
} file_type;

enum http_codes {
    http_200,
    http_404,
    http_301,
    http_400
};

file_type find_file_type(char *filename);

int find_file(char *file_name);

void send_response(char *request_str, int connection);



#endif