#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<stdio.h>
 
int main() {
    int s,r,recb,sntb,x;
    int ca;
    printf("INPUT port number: ");
    scanf("%d", &x);
    socklen_t len;
    struct sockaddr_in server,client;
    char buff[50];
    s = socket(AF_INET,SOCK_DGRAM,0);
    if(s == -1) {
        printf("\nSocket creation error.");
        exit(0);
    }
    printf("\nSocket created.");
    server.sin_family=AF_INET;
    server.sin_port=htons(x);
    server.sin_addr.s_addr=htonl(INADDR_ANY);
    len = sizeof(client);
    ca = sizeof(client);
    r = bind(s,(struct sockaddr*)&server,sizeof(server));
    if(r == -1) {
        printf("\nBinding error.");
        exit(0);
    }
    printf("\nSocket binded.");
 
    while(1) {

        recb = recvfrom(s,buff,sizeof(buff),0,(struct sockaddr*)&client,&ca);
        if(recb == -1) {
            printf("\nMessage Recieving Failed");        
            close(s);
            exit(0);
        }    
        
        if(!strcmp(buff,"halt")) {
            break;
        }

        int strl = strlen(buff);
        int palin = 1; 
        for(int i=0; i<strlen(buff)/2; i++) {
            if (buff[i] != buff[strl-i-1]) {
                palin = 0;
            }
        }
        int count[] = {0, 0, 0, 0, 0};
        for(int i=0; i<strlen(buff); i++) {
            if (buff[i] == 'a') {
                count[0]++;
            }
            if (buff[i] == 'e') {
                count[1]++;
            }
            if (buff[i] == 'i') {
                count[2]++;
            }
            if (buff[i] == 'o') {
                count[3]++;
            }
            if (buff[i] == 'u') {
                count[4]++;
            }
        }

        char re[5];
        for (int i=0; i<5; i++) {
            re[i] = count[i]+'0';
        }

        char resp[30];
        palin ? strcpy(resp, "Input is palindrome.") : strcpy(resp, "Input is not palindrome.");
        printf("\n\n");
        
        sntb = sendto(s,resp,sizeof(resp),0,(struct sockaddr*)&client,len);
        sntb = sendto(s,re,sizeof(re),0,(struct sockaddr*)&client,len);
        if(sntb == -1) {
            printf("\nMessage Sending Failed");
            close(s);
            exit(0);
        }
    }
    close(s);
}