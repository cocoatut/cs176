/* UDP client in the internet domain */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void error(const char *);
int main(int argc, char *argv[])
{
   int sock, n;
   unsigned int length;
   struct sockaddr_in server, from;
   struct hostent *hp;
   char buffer[128];
   
   if (argc != 3) { printf("Usage: server port\n");
                    exit(1);
   }
   sock= socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("socket");

   server.sin_family = AF_INET;
   hp = gethostbyname(argv[1]);
   if (hp==0) error("Unknown host");

   bcopy((char *)hp->h_addr, 
        (char *)&server.sin_addr,
         hp->h_length);
   server.sin_port = htons(atoi(argv[2]));
   length=sizeof(struct sockaddr_in);
   printf("Enter string: ");
   bzero(buffer,128);
   fgets(buffer,128,stdin);
   n=sendto(sock,buffer,strlen(buffer),0,(const struct sockaddr *)&server,length);
   bzero(buffer,128);
   if (n < 0) error("Sendto");
   //n = recvfrom(sock,buffer,128,0,(struct sockaddr *)&from, &length);
   //printf("From server: ");
   //printf("%s\n",buffer);
   while(recvfrom(sock,buffer,128,0,(struct sockaddr *)&from, &length)>0)
   {
   	//bzero(buffer,128);
   	//n = recvfrom(sock,buffer,128,0,(struct sockaddr *)&from, &length);
    printf("%s\n",buffer);
    int ret;
    ret = strncmp(buffer,"From server: Sorry, cannot compute!",50);
    if(ret==0)
    {
    	close(sock);
    }
    else if(strlen(buffer)<=14)
    {
    	break;
    }
    bzero(buffer,128);
    //printf("%d\n",n);
    
   }
   
   if (n < 0) error("recvfrom");
   //write(1,"From server: ",13);
   //write(1,buffer,n);
   close(sock);
   return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}