#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
 
int main() {
    int s,r,rec,sen,x,ci;
    char buff[50];

    printf("Input port number: ");
    scanf("%d", &x);
    socklen_t len;
    struct sockaddr_in server,client;
    s = socket(AF_INET,SOCK_DGRAM,0);
    
    server.sin_family = AF_INET;
    server.sin_port = htons(x);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    len = sizeof(client);
    ci = sizeof(client);
    
    r = bind(s,(struct sockaddr*)&server,sizeof(server));
    
    while(1) {
        rec = recvfrom(s,buff,sizeof(buff),0,(struct sockaddr*)&client,&ci);
        if (buff[0] == '*') {
            close(s);
            exit(0);
        }
        char ch = buff[0];
        int ans = 0;
        if(ch>=65 && ch<=90) {
            ans = 1;
        }
        if(ch>=97 && ch<=122) {
            ans = 1;
        }

        ans == 1 ? strcpy(buff, "Alphabet") : strcpy(buff, "Number");
        sen = sendto(s,buff,sizeof(buff),0,(struct sockaddr*)&client,len);
    }
    close(s);
}