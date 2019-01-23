#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    int rec, sen,a, b, port, s;
    char buff[50];
    socklen_t len;
    printf("Input port number : ");
    scanf("%d", &port);
    struct sockaddr_in server, client;

    s = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    a = sizeof(server);
    b = sizeof(server);

    while(1) {
        printf("\nInput letter : ");
        scanf("%s", buff);
        sen = sendto(s, buff, sizeof(buff), 0, (struct sockaddr *)&server, b);
        if (buff[0] == '*') {
            close(s);
            exit(0);
        }
        strcpy(buff, "");
        rec = recvfrom(s, buff, sizeof(buff), 0, (struct sockaddr *)&server, &a);
        printf("%s\n", buff);
    }

    close(s);
}
