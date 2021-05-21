#include<stdio.h>
#include"hack.h"

int main(int argc, char *argv[])
{
    int port = 80;
    char *targetip;
    
    if (argc < 2)
    {
        printf("WebGrab usage:\r\n");
        printf("    %s <Target Ip> [port]\r\n", argv[0]);
        return (0);
    }
    targetip = argv[1];
    char * output;

    if (argc >= 3)
    {
        port = atoi(argv[2]);
    }
    if (argc >= 4)
    {
        output = get_http(targetip, port, argv[3])
    }
    else
    {
        output = get_http(targetip, port, "/");
    }

    if (is_string_in("Error 40", output)            ||
        (is_string_in("302 Object moved", output))  ||
        (is_string_in("404 Not Found", output))     ||
        (is_string_in("404 Object Not Found", output))
    {
        printf("Pages does not exist!");
    }
    else
    {
        printf("%s", output);
    }
    return (0);
}