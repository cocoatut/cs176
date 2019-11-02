/* UDP client in the internet domain */
//udp frame work reference http://www.linuxhowtos.org/data/6/client_udp.c
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

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

   double rtt[10];
   int pings[10];
   char ping[] = "PING ";
   int seq = 0;
   struct timeval tInterval;
   tInterval.tv_sec = 1;
   tInterval.tv_usec = 0;
   //set timeInterval
   setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&tInterval, sizeof(tInterval));
   while(seq < 10)
   {
    struct timespec tSend, tReceive;
    clock_gettime(CLOCK_MONOTONIC, &tSend);
    double tS = (double)tSend.tv_nsec;
    double sec = (double)tSend.tv_sec;
    sprintf(buffer, "PING %d %f %lf\n", seq, sec, tS);

    sendto(sock,buffer,strlen(buffer),0,(const struct sockaddr *)&server,length);
    bzero(buffer,128);

    n = recvfrom(sock, buffer, 128, 0, (struct sockaddr*)&from, &length);
    //timeout
    clock_gettime(CLOCK_MONOTONIC, &tReceive);
    if(n<0){
        printf("Request timeout for icmp_seq %d\n", seq);
        rtt[seq] = 0.0;
    }
    else{
        double msecChange = ((double)(tReceive.tv_nsec - tSend.tv_nsec))/1000000.0;
        double allChange = (tReceive.tv_sec-tSend.tv_sec)*1000.0 + msecChange;
        rtt[seq] = allChange;
        printf("PING received from %s: seq#=%d time=%lf ms\n",hp->h_name, seq, allChange);
    }
    seq++;
   }
   
   //if (n < 0) error("recvfrom");
   //write(1,"From server: ",13);
   //write(1,buffer,n);
   close(sock);
   printf("%s", "---ping statistics---\n");
   double maxRTT = 0.0;
   double minRTT = 100000.0;
   double total = 0.0;
   int loss = 0;
   for(int i = 0; i < 10; i ++){
       if(rtt[i] == 0){
           loss++;
       }
       else{
           if(maxRTT < rtt[i]){
               maxRTT = rtt[i];
           }
           if(minRTT > rtt[i]){
               minRTT = rtt[i];
           }
       }
       total += rtt[i];
   }
   double mean = total / (10 - loss);
   int percentage = (((double)(10-loss))/10)*100;
   printf("%d packets transmitted, %d received, %d%% packet loss rtt min/avg/max = %lf %lf %lf ms\n", 10, 10-loss, percentage, minRTT, mean, maxRTT);
   return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}