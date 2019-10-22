/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[128];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     char report[] = "From server: ";
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,1);
     clilen = sizeof(cli_addr);
     
     
     bzero(buffer,128);
     
     int ind=0;
     int sum=0;
     while(1)
     {
        newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
        if (newsockfd < 0) 
          error("ERROR on accept");
        //bzero(buffer,128);
        sum=0;
        ind=0;
        n = read(newsockfd,buffer,128);
        //printf("%s\n",buffer);
        if (n < 0) error("ERROR reading from socket");
        //printf("%lu\n",strlen(buffer));
        for(int i = 0; i<(strlen(buffer)-1); i++)
        {
            if(!isdigit(buffer[i]))
            {
                ind = 1;
            }  
       }
       if(ind==1)
       {
        n = write(newsockfd,"From server: Sorry, cannot compute!",50);
        bzero(buffer,128);
        continue;
       }

       if(ind==0)
       {
        char response[128];
        //bzero(response,128);
        
        //printf("%s\n",buf);
        //printf("%d\n",buf[0]);
       for(int i = 0; i<strlen(buffer)-1; i++)
        {
            //printf("%c\n",buf[i]);
            sum+=buffer[i]-'0';
            //printf("%d\n",sum);
        }
        //printf("%d\n",sum);

        //printf("%d\n",sum);
        sprintf(response,"%d",sum);
        int l = strlen(report)+strlen(response)+1;
        char message[l];
        strcpy(message,report);
        strcat(message,response);
       //write(1,"Received a datagram: ",21);
       //write(1,buf,n);
        n = write(newsockfd,message,50);
       }
       while(sum>9)
       {

        int n = sum;
        int h = 0;
        while(n>0)
        {
            h+=n%10;
            n/=10;
        }
        sum = h;
        char response[128];
        sprintf(response,"%d",sum);
        int l = strlen(report)+strlen(response)+1;
        char message[l];
        strcpy(message,report);
        strcat(message,response);
        n = write(newsockfd,message,50);
        if (n < 0) error("ERROR writing to socket");
        printf("%d\n",sum);

       }
       bzero(buffer,128);

       close(newsockfd);
       /*if(sum<9)
       {
        close(newsockfd);
       }*/
     }//end of while loop



     //printf("Here is the message: %s\n",buffer);
     //n = write(newsockfd,"I got your message",18);
     //close(newsockfd);
     close(sockfd);
     return 0; 
}