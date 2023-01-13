#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUFSIZE 1024

typedef struct _{
    SOCKET Sock;
}sk;

void DataRecv(sk *Psk);

int main(){
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    //WSAStartup
    if(iResult == INVALID_SOCKET){
        printf("[-] WSAStartup Failed!\n");
        WSACleanup();
        return 0;
    }
    
    SOCKET Client_Sock;
    SOCKADDR_IN Server_Addr;
    HANDLE R_thread;
    char message[BUFSIZE] = {'\0', };
    DWORD dwSize;
    char ServerIP[20];
    char Port[10];
    int i;
    
    //Create Socket
    Client_Sock = socket(AF_INET, SOCK_STREAM, 0);
    if(Client_Sock == INVALID_SOCKET){
        printf("[-] Invalid Socket!\n");
        closesocket(Client_Sock);
        WSACleanup();
        return 0;
    }

    printf("[*] Server IP: ");
    if(fgets(ServerIP, 20, stdin) == NULL){
        closesocket(Client_Sock);
        WSACleanup();
        return 0;
    }
    printf("[*] Port: ");
    if(fgets(Port, 10, stdin) == NULL){
        closesocket(Client_Sock);
        WSACleanup();
        return 0;
    }

    //Server Setting
    memset(&Server_Addr, 0, sizeof(Server_Addr));
    Server_Addr.sin_family = AF_INET;
    Server_Addr.sin_addr.s_addr = inet_addr(ServerIP);
    Server_Addr.sin_port = htons(atoi(Port));

    //Connect
    if(connect(Client_Sock, (const struct sockaddr*)&Server_Addr, sizeof(Server_Addr)) == SOCKET_ERROR){
        printf("[-] Connection Failed!\n");
        closesocket(Client_Sock);
        WSACleanup();
        return 0;
    }
    printf("[*] Connected!\n");
    printf("================\n");

    sk SK;
    SK.Sock = Client_Sock;

    //Create DataRecv Thread
    R_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DataRecv, &SK, 0, NULL);

    //Chat
    while(TRUE){
        printf(">");
        for(i = 0; i < sizeof(message); i++){
            message[i] = '\0';
        }
        fgets(message, 1024, stdin);

        send(Client_Sock, message, 1024, 0);
    }
    WSACleanup();
    return 0;
}

void DataRecv(sk *Psk){
    char message[BUFSIZE] = {'\0', };
    int i;
    int Chk_Recv;

    while(TRUE){
        for(i = 0; i < sizeof(message); i++){
            message[i] = '\0';
        }
        Chk_Recv = recv(Psk->Sock, message, sizeof(message), 0);
        if(Chk_Recv == SOCKET_ERROR){
            closesocket(Psk->Sock);
            exit(0);
        }
        printf("\nSERVER: %s", message);
    }
    return;
}
