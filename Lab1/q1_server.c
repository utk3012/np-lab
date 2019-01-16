#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<stdio.h>


void swap(int *xp, int *yp)  { 
    int temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
}
void bubbleSort(int arr[], int n) {
   int i, j; 
   for (i = 0; i < n-1; i++)       
       for (j = 0; j < n-i-1; j++)  
           if (arr[j] > arr[j+1]) 
              swap(&arr[j], &arr[j+1]); 
}
int linearSearch(int arr[], int n, int x) {
    int i;
    for (i = 0; i < n; i++) 
        if (arr[i] == x) 
            return i; 
    return -1; 
} 

int main() {
    int s,r,recb,sntb,x,ns,a=0;
    printf("INPUT port number: ");
	scanf("%d", &x);
    socklen_t len;
    struct sockaddr_in server,client;
    char buff[50];
    s = socket(AF_INET,SOCK_STREAM,0);

    if(s == -1) {
        printf("\nSocket creation error.");
        exit(0);
    }

    printf("\nSocket created.");
    server.sin_family=AF_INET;
    server.sin_port=htons(x);
    server.sin_addr.s_addr=htonl(INADDR_ANY);
    r = bind(s,(struct sockaddr*)&server,sizeof(server));

    if(r == -1) {
        printf("\nBinding error.");
        exit(0);
    }

    printf("\nSocket binded.");
    r = listen(s,1);
    if(r == -1) {
        close(s);
        exit(0);
    }

    printf("\nSocket listening.");
    len = sizeof(client);
    ns = accept(s,(struct sockaddr*)&client, &len);
    if(ns == -1) {
        close(s);
        exit(0);
    }
    printf("\nSocket accepting.");

    while(1) {
        char arr[50];
        recb = recv(ns,arr,sizeof(arr),0);
        
        if(recb == -1) {
            printf("\nMessage Recieving Failed");        
            close(s);
            close(ns);
            exit(0);
        }
        printf("\n");
        
        if (!strcmp(arr, "exit")) {
            break;
        }
        int num = arr[0] - '0';
        int choice = arr[1] - '0';

        int* numarr = (int*) malloc(num * sizeof(int));
        for (int i=0; i<num; i++) {
            numarr[i] = arr[i+2] - '0';
        }
        int key, searchRes;
        char ress[30];
        char* resarr1 = (char*) malloc((num + 1) * sizeof(char));
        int k = 0;

        switch(choice) {
            case 1:
                key = arr[num+2] - '0';
                searchRes = linearSearch(numarr, num, key);
                if (searchRes == -1)
                    strcpy(ress, "Element not found");
                else
                    strcpy(ress, "Element found");
                sntb = send(ns,ress,sizeof(ress),0);
                if(sntb == -1) {
                    printf("\nMessage Sending Failed");
                    close(s);
                    close(ns);
                    exit(0);
                }
            break;
            case 2:
                bubbleSort(numarr, num);
                char* resarr = (char*) malloc(num * sizeof(char));
                for (int i=0; i<num; i++) {
                    resarr[i] = numarr[i] + '0';
                }
                
                sntb = send(ns,resarr,sizeof(resarr),0);
                if(sntb == -1) {
                    printf("\nMessage Sending Failed");
                    close(s);
                    close(ns);
                    exit(0);
                }
                free(resarr);
            break;
            case 3:
                for (int i=0; i<num; i++) {
                    if (numarr[i]%2 == 1) {
                        resarr1[k++] = numarr[i] + '0';
                    }
                }
                resarr1[k++] = '-';
                for (int i=0; i<num; i++) {
                    if (numarr[i]%2 == 0) {
                        resarr1[k++] = numarr[i] + '0';
                    }
                }

                sntb = send(ns,resarr1,sizeof(resarr),0);
                if(sntb == -1) {
                    printf("\nMessage Sending Failed");
                    close(s);
                    close(ns);
                    exit(0);
                }
            break;
        }
        free(resarr1);
        free(numarr);
    }
    close(ns);
    close(s);
}