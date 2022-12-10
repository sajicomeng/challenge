#include <stdio.h>
#include "Winsock2.h"
#include <ws2tcpip.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "database.h"
#include "print_thread.h"
#include <time.h>

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)

#define MAX 80
#define SA struct sockaddr
#define MAX_LINE_LENGTH 1000

#define PORT_TXT "port ="
#define P1_TXT "p1 ="
#define P2_TXT "p2 ="

static int PORT_NUM = 65432;
static char P1_HEADER[MAX];
static char P2_HEADER[MAX];

extern int openDB();
void removeCharcter(char* s, char remove) {
    char* d = s;
    do {
        while (*d == remove) {
            ++d;
        }
    } while (*s++ = *d++);
}

char* getCurrentTime(){
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    char * res = asctime(timeinfo);
    removeCharcter(res, '\n');
    return res;
}

bool getStringFromMessage(const char* message,
                          const char* startsWith,
                          char* retString,
                          int maxLen,
                          bool mustRemoveSpace){
    bool res = false;
    if (strncmp(message, startsWith, strlen(startsWith)) == 0){
        int size = strlen(message) - strlen(startsWith);
        if (size < maxLen){
            res = true;
            memcpy(retString, message + strlen(startsWith), size);
            retString[size] = '\0';
            if (mustRemoveSpace){
                removeCharcter(retString, ' ');
            }
            removeCharcter(retString, '\n');
        }
    }
    return res;
}

bool readConf(){
    FILE    *textfile;
    char    line[MAX_LINE_LENGTH];
    textfile = fopen("config.ini", "r");
    if(textfile == NULL)
        return false;

    while(fgets(line, MAX_LINE_LENGTH, textfile)){
        char value[MAX];
        bzero(value, MAX);
        if (getStringFromMessage(line, PORT_TXT, value, MAX, false)){
            int port = atoi(value);
            if  (port){
                PORT_NUM = port;
            }
        }
        if (getStringFromMessage(line, P1_TXT, value, MAX, true)){
            memcpy(P1_HEADER, value, strlen(value));
        }
        if (getStringFromMessage(line, P2_TXT, value, MAX, true)){
            memcpy(P2_HEADER, value, strlen(value));
        }
    }
    fclose(textfile);
    return true;
}

void checkMessageType(char* message){
    char value[MAX];
    if (getStringFromMessage(message, P1_HEADER, value, MAX, false)){
        int num = atoi(value);
        if (num){
            if (insertInteger(num, getCurrentTime())){
                printf("int message successfully add to DB --> %d\n", num);
            }
            else {
                printf("cannot add int message to DB --> %d\n", num);
            }
        }
        else{
            printf("the message is not a valid number --> %s\n", value);
        }
    }
    bzero(value, MAX);
    if (getStringFromMessage(message, P2_HEADER, value, MAX, false)){
        if (insertString(value, getCurrentTime())){
            printf("string message successfully add to DB --> %s\n", value);
        }
        else {
            printf("cannot add string message to DB --> %s\n", value);
        }
    }
}

void receiveFromSocket(int sockID)
{
    char buff[MAX];
    int n;
    // infinite loop for chat
    for (;;) {
        bzero(buff, MAX);
        int size = recv(sockID, buff, sizeof(buff), 0);
        if (size > 0)
            checkMessageType(buff);

    }
}

int prepareSock(){
    int sockfd, connfd;
    struct sockaddr cli;
    struct sockaddr_in servaddr;

    WSADATA wsaData;

   if(WSAStartup(0x202, &wsaData) == 0)
   {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            printf("socket creation failed...\n");
            exit(0);
        }
        else
            printf("Socket successfully created..\n");
        bzero(&servaddr, sizeof(servaddr));

        // assign IP, PORT
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(PORT_NUM);

        // Binding newly created socket to given IP and verification
        if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
            printf("socket bind failed...\n");
            exit(0);
        }
        else
            printf("Socket successfully binded..\n");

        // Now server is ready to listen and verification
        if ((listen(sockfd, 5)) != 0) {
            printf("Listen failed...\n");
            exit(0);
        }
        else
            printf("Server listening..\n");
        socklen_t len = sizeof(cli);

        // Accept the data packet from client and verification
        connfd = accept(sockfd, &cli, &len);
        if (connfd < 0) {
            printf("server accept failed...\n");
            exit(0);
        }
        else
            printf("server accept the client...\n");
        return connfd;
   }
   return -1;
}

int main(int argc, char *argv[])
{
    if (!openDB()){
        printf("cannot open database file!");
        return 0;
    }
    if (!createTable()){
        printf("cannot create database table!");
        return 0;
    }
    if (!readConf()){
        printf("cannot find config file!\n");
    };
    if (!startThread()){
        printf("cannot start print thread!");
    }
    int sockId = prepareSock();
    receiveFromSocket(sockId);
    stopThread();
    close(sockId);
}
