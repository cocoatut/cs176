/* Creates a datagram server.  The port 
   number is passed as an argument.  This
   server runs forever */

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <ctype.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
   int sock, length, n;
   socklen_t fromlen;
   struct sockaddr_in server;
   struct sockaddr_in from;
   char buf[128];
   char report[] = "From server: ";

   if (argc < 2) {
      fprintf(stderr, "ERROR, no port provided\n");
      exit(0);
   }
   
   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) 
    error("Opening socket");
   length = sizeof(server);
   bzero(&server,length);
   server.sin_family=AF_INET;
   server.sin_addr.s_addr=INADDR_ANY;
   server.sin_port=htons(atoi(argv[1]));
   if (bind(sock,(struct sockaddr *)&server,length)<0) 
       error("binding");
   fromlen = sizeof(struct sockaddr_in);
   int ind;
   int sum = 0;
   while (1) {
   	   sum=0;
   	   ind = 0;
       n = recvfrom(sock,buf,128,0,(struct sockaddr *)&from,&fromlen);
       if (n < 0) error("recvfrom");
       for(int i = 0; i<(strlen(buf)-1); i++)
       {
        if(!isdigit(buf[i]))
        {
          ind = 1;
        }  
       }
       if(ind==1)
       {
       	n = sendto(sock,"From server: Sorry, cannot compute!",50,0,(struct sockaddr *)&from,fromlen);
       	bzero(buf,128);
       	continue;
       }
       if(ind==0)
       {
       	char response[128];
        //bzero(response,128);
        
        //printf("%s\n",buf);
        //printf("%d\n",buf[0]);
       for(int i = 0; i<strlen(buf)-1; i++)
       	{
       		//printf("%c\n",buf[i]);
        	sum+=buf[i]-'0';
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
        n = sendto(sock,message,strlen(message),0,(struct sockaddr *)&from,fromlen);
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
        n = sendto(sock,message,strlen(message),0,(struct sockaddr *)&from,fromlen);


       }
       bzero(buf,128);

       if (n  < 0) error("sendto");

      
   }
   return 0;
 }
