#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/stat.h>
#define MAX_SIZE 2048

int main() {
  int s, r, recb, sntb, x;
  printf("INPUT port number: ");
  scanf("%d", &x);
  struct sockaddr_in server;
  char buff[50];
  s = socket(AF_INET, SOCK_STREAM, 0);
  if(s == -1) {
    printf("\nSocket creation error.");
    exit(0);
  }
  
  printf("\nSocket created.");
  
  server.sin_family = AF_INET;
  server.sin_port = htons(x);
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  
  r = connect(s, (struct sockaddr*)&server, sizeof(server));

  if(r == -1) {
    printf("\nConnection error.");
    exit(0);
  }
  printf("\nSocket connected.\n");

  char *file_name = (char *)malloc(128 * sizeof(char));
  printf("Enter file name: ");
  scanf("%s", file_name);

  sntb = send(s, file_name, sizeof(file_name), 0);
  if(sntb == -1) {
    printf("Couldn't send file name\n");
    free(file_name);
    return -1;
  }
  
  int file_found;
  recv(s, &file_found, sizeof(file_found), 0);
  if(file_found == 0) {
    printf("File not present.\n");
    close(s);
    free(file_name);
    return -1;
  }

  int opt;
  
  while(1) {
    printf("Choose option:\n1.Search\n2.Replace\n3.Reorder\n4.Exit\n");
    scanf("%d", &opt);
    send(s, &opt, sizeof(opt), 0);

    if(opt == 1) {
      int count;
      char *search_string = (char *)malloc(128 * sizeof(char));
      printf("Enter search string: ");
      scanf("%s", search_string);
      send(s, search_string, sizeof(search_string), 0);
      recv(s, &count, sizeof(count), 0);
      if(count == 0)
        printf("String not found\n");
      else printf("No of times string found: %d\n", count);

      free(search_string);
    }
    else if(opt == 2) {
      char *str1 = (char *)malloc(128 * sizeof(char));
      char *str2 = (char *)malloc(128 * sizeof(char));
      int found;

      printf("Enter search and replace strings: ");
      scanf("%s %s", str1, str2);
      send(s, str1, sizeof(str1), 0);
      send(s, str2, sizeof(str2), 0);
      recv(s, &found, sizeof(found), 0);

      if(found == 0) 
        printf("String not found\n");
      else printf("String replaced\n");

      free(str1);
      free(str2);
    }
    else if(opt == 3) {
      printf("File reordered\n");
    }
    if(opt == 4)
      break;  
  }
  free(file_name);
  close(s);
}