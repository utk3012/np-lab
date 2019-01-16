#include<string.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<sys/stat.h>
 
int main() {
    int s,r,recb,sntb,x;
    printf("INPUT port number: ");
    scanf("%d", &x);
    struct sockaddr_in server;
    char buff[50];
    s=socket(AF_INET,SOCK_STREAM,0);
    if(s==-1) {
        printf("\nSocket creation error.");
        exit(0);
    }
    printf("\nSocket created.");
    server.sin_family = AF_INET;
    server.sin_port = htons(x);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    r = connect(s,(struct sockaddr*)&server,sizeof(server));
    if(r == -1) {
        printf("\nConnection error.");
        exit(0);
    }
    printf("\nSocket connected.");
    printf("\n\n");

    
    while (1) {
        int num;
        printf("Enter length of array: ");
        scanf("%d", &num);
        char* arr = (char*) malloc((num+3) * sizeof(char));
        int* numarr = (int*) malloc(num * sizeof(int));
        
        printf("Enter the array: \n");
        for (int i = 0; i < num; i++) {
            scanf("%d", &numarr[i]);
        }


        for (int i = 2; i < num+2; i++) {
            arr[i] = numarr[i-2] + '0';
        }

        printf("1. Search for a number\n");
        printf("2. Sort\n");
        printf("3. Split into odd and even\n");
        printf("4. Exit\n\n");
        printf("Enter choice : ");
        int choice;
        scanf("%d", &choice);

        if (choice == 4) {
            char messa[] = "exit";
            sntb = send(s, messa, sizeof(messa), 0);
            break;
        }
        
        arr[0] = num + '0';
        arr[1] = choice + '0';
        if (choice == 1) {
            printf("\nInput number to be searched : ");
            int searchNum;
            scanf("%d", &searchNum);
            arr[num+2] = searchNum + '0';
        }

        sntb = send(s, arr, sizeof(arr), 0);
        if(sntb == -1) {
            close(s);
            printf("\nMessage Sending Failed");
            exit(0);
        }

        strcpy(buff, "");
        recb = recv(s,buff,sizeof(buff),0);
        if(recb == -1) {
            printf("\nMessage Recieving Failed");    
            close(s);
            exit(0);
        }
        printf("\nArray : ");
        printf("%s", buff);
        printf("\n\n");
        free(arr);
        free(numarr);
    }
    close(s);
}
