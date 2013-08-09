#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define MAXLINE 4096
#define SOCK_SERVER_PATH "/home/cheny/github/Socket/socket_server"
#define SOCK_CLIENT_PATH "/home/cheny/github/Socket/socket_client"

void main(void)
{
    int client_fd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t server_addr_len;
    char buf_send[MAXLINE];
    char buf_recv[MAXLINE];

    //unlink no use socket
    if(access(SOCK_CLIENT_PATH, R_OK) == 0) {
        printf("socket file already exits,unlink it!\n");
        unlink(SOCK_CLIENT_PATH);
    }

    //create socket
    if((client_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket fail");
        exit(1);
    }
    printf("socket ok!\n");

    //bind socket
    memset(&client_addr, 0x00, sizeof(struct sockaddr_un));
    client_addr.sun_family = AF_UNIX;
    strncpy(client_addr.sun_path, SOCK_CLIENT_PATH, (sizeof(client_addr.sun_path) - 1));
    if(bind(client_fd, (struct sockaddr *)&client_addr, sizeof(struct sockaddr_un)) == -1) {
        close(client_fd);
        perror("bind fail!");
        exit(1);
    }
    printf("bind ok!\n");

    //connect socket
    memset(&server_addr, 0x00, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCK_SERVER_PATH, (sizeof(server_addr.sun_path) - 1));
    server_addr_len = sizeof(struct sockaddr_un);
    if(connect(client_fd, (struct sockaddr *)&server_addr, server_addr_len) != 0) {
        close(client_fd);
        perror("connect fail");
        exit(1);
    }
    printf("connect ok!\n");

    do {
        //send buf
        memset(buf_send, 0x00, sizeof(buf_send));
        printf("Input:");
        //scanf("%s", buf);
        //here is a bug, buf may overflow
        if(gets(buf_send) == NULL) {
            perror("gets fail");
        }
        if(send(client_fd, buf_send, strlen(buf_send), 0) != strlen(buf_send)) {
            perror("send fail");
        }
    } while(strcmp(buf_send, "quit") != 0);

    close(client_fd);
    printf("close ok!\n");
}
