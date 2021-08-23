#include <stdio.h>
#include <stdlib.h> 
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <ctype.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

#define BUFFER_SIZE 4096


int main(int argc, char *argv[]){
    
    
    int Serversocket = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in serverAddr,clientAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9800);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(Serversocket,(struct sockaddr *)&serverAddr,sizeof(serverAddr))<0){
        perror("Binding Failed");
        return -1;
    }

    if(listen(Serversocket,2)<0){
        perror("Listen failed");
        return -1;
    }

    printf("FTP Server Started on port 9800...\n");

    int clientlen = sizeof(clientAddr);
    int clientsocket = accept(Serversocket,(struct sockaddr *)&clientAddr,&clientlen);
    if(clientsocket <0)
        perror("accept failed");
    
    int n = write(clientsocket,"Connected check\n",strlen("Connected check\n"));


    FILE *fp;

    char filename[50];
    
    n = read(clientsocket,filename,50);
    if(n<0){
        perror("read failed");
        exit(1);
    }
    printf("Got Filename %s \n",filename);
    
    //opening file
    fp = fopen(filename,"r");
    if(fp == NULL){
        perror("File Opening failed\n");
        exit(-1);
    }

    printf("File Opened\n");
    struct stat file_stat;
    // Get file stats;
    int fd;
    if(lstat("test.c",&file_stat)<0){
        perror("File Stat failed\n");
        exit(1);
    }

    printf("files size: %ld bytes\n",file_stat.st_size);

    //sending file size
    char file_size[50];
    sprintf(file_size,"%d",(int)file_stat.st_size);
    n = send(clientsocket,file_size,sizeof(file_size),0);
    if(n<0){
        perror("Send failed\n");
        exit(1);
    }
    printf("File Size Sent\n");

    //sending file
    int file_size_rem = file_stat.st_size;
    int offset= 0;
    while(file_size_rem > 0){
        sprintf()
        n = send(clientsocket,fp,&offset,BUFFER_SIZE);
        if(n<0){
            perror("Send failed\n");
            exit(1);
        }
        // printf("[1] %d bytes Sent and %d remaining\n",n,file_size_rem);
        file_size_rem -= n;
        // printf("[2] %d bytes Sent and %d remaining\n",n,file_size_rem);

    }
    close(clientsocket);
    close(Serversocket);

    printf("Server CLosed \n");
    return 0;    
}