#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#define MAX_SIZE 2048

int main() {
  int s, r, recb, sntb, x, ns, a=0;
  printf("INPUT port number: ");
  scanf("%d", &x);
  struct sockaddr_in server,client;
  
  s = socket(AF_INET, SOCK_STREAM, 0);
  if(s == -1) {
    printf("\nSocket creation error.");
    exit(0);
  }

  printf("\nSocket created.");
  
  server.sin_family = AF_INET;
  server.sin_port = htons(x);
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  
  r = bind(s, (struct sockaddr*)&server, sizeof(server));
  
  if (r == -1) {
    printf("\nBinding error.");
    exit(0);
  }
  printf("\nSocket bound.");
  
  r = listen(s, 1);
  if (r == -1) {
    close(s);
    exit(0);
  }
  printf("\nSocket listening.\n");
  
  int len = sizeof(client);
  ns = accept(s,(struct sockaddr*)&client, &len);
  if(ns == -1) {
    close(s);
    exit(0);
  }
  printf("\nSocket accepting.\n");
  
  char *file_name = (char *)malloc(128 * sizeof(char));
  recb = recv(ns, file_name, sizeof(file_name), 0);
  
  if(recb == -1) {
    printf("\nMessage Receiving Failed");
    free(file_name);
    close(s);
    close(ns);
    exit(0);
  }
  
  printf("File name received: %s\n", file_name);
  
  FILE *file_path = fopen(file_name, "r+");
  int file_found = 1;
  if(file_path == NULL) {
    printf("File not found\n");
    file_found = 0;
    send(ns, &file_found, sizeof(file_found), 0);
    close(ns);
    close(s);
    free(file_path);
    return -1;
  }
  send(ns, &file_found, sizeof(file_found), 0);
  int opt;
  while(recv(ns, &opt, sizeof(opt), 0) != -1) {
    if(opt == 1) {
      char *search_string = (char *)malloc(128 * sizeof(char));
      char *command = (char *)malloc(512 * sizeof(char));

      recv(ns, search_string, sizeof(search_string), 0);
      sprintf(command, "grep -o %s %s | wc -l", search_string, file_name);

      FILE *cmd = popen(command, "r");
      int count = fgetc(cmd) - '0';

      send(ns, &count, sizeof(count), 0);

      free(search_string);
      free(command);
      pclose(cmd);
    }
    else if(opt == 2) {
      char *str1 = (char *)malloc(128 * sizeof(char));
      char *str2 = (char *)malloc(128 * sizeof(char));
      char *command = (char *)malloc(512 * sizeof(char));
      int found;

      recv(ns, str1, sizeof(str1), 0);
      recv(ns, str2, sizeof(str2), 0);
      
      sprintf(command, "grep -o %s %s | wc -l", str1, file_name);

      FILE *cmd = popen(command, "r");
      int count = fgetc(cmd) - '0';
      if(count == 0) {
        found = 0;
      }
      else {
        found = 1;
        memset(command, 0, sizeof(command));
        sprintf(command, "sed -i s/%s/%s/g %s", str1, str2, file_name);
        pclose(cmd);
        cmd = popen(command, "r");
      }

      send(ns, &found, sizeof(found), 0);
      free(str1);
      free(str2);
      free(command);
      pclose(cmd);
    }
    else if(opt == 3) {
      unsigned long int occurrences[256];
      memset(&occurrences, 0, sizeof(occurrences));
      int c;
      for(c = fgetc(file_path); c != EOF; c = fgetc(file_path)) 
        occurrences[(unsigned char)c] ++;

      rewind(file_path);

      for(c = 0; c < 256; c++)
        for(; occurrences[c] > 0; occurrences[c]--)
          fputc(c, file_path);

    }
    else break;
  }

  close(ns);
  close(s);
  fclose(file_path);
  free(file_name);
  return 0;
}