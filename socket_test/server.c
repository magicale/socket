#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>

#define SOCK_SERVER_PATH "socket_server"
#define SOCK_CLIENT_PATH "socket_client"
#define LISTEN_BACKLOG 10
#define MAXLINE 4096

void main(void)
{
    int server_fd, client_fd;
    struct sockaddr_un server_addr;
    struct sockaddr_un client_addr;
    socklen_t client_addr_len;
    char buf_recv[MAXLINE];
    int num;

    //unlink no use socket
    if(access(SOCK_SERVER_PATH, R_OK) == 0) {
        printf("socket file already exist,unlink it!\n");
        unlink(SOCK_SERVER_PATH);
    }

    //create socket
    if((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket fail!");
        exit(1);
    }
    printf("socket ok!\n");

    //bind socket
    memset(&server_addr, 0x00, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCK_SERVER_PATH, (sizeof(server_addr.sun_path) - 1));

    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un)) == -1) {
        close(server_fd);
        perror("bind fail!");
        exit(1);
    }
    printf("bind ok!\n");

    //listen socket
    if(listen(server_fd, LISTEN_BACKLOG) == -1) {
        close(server_fd);
        perror("listen fail!");
        exit(1);
    }
    printf("listen ok!\n");

    //accept socket
    memset(&client_addr, 0x00, sizeof(struct sockaddr_un));
    client_addr.sun_family = AF_UNIX;
    strncpy(client_addr.sun_path, SOCK_CLIENT_PATH, (sizeof(client_addr.sun_path) - 1));
    client_addr_len = sizeof(struct sockaddr_un);
    if((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {
        close(client_fd);
        close(server_fd);
        perror("accept fail!");
        exit(1);
    }
    printf("accept ok!\n");

    do {
        //recv buf
        memset(buf_recv, 0x00, sizeof(buf_recv));
        num = recv(client_fd, buf_recv, MAXLINE, 0);
        printf("Output:%s\n", buf_recv);
    } while((strcmp(buf_recv, "quit") != 0) && num > 0);

    close(client_fd);
    close(server_fd);
    printf("close ok!\n");
}
