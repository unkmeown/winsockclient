#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

#define SERVER_IP "127.0.0.1"
#define PORT 888

int loop = 1;

int sendmsg (SOCKET sendSock, char *msg)
{
  int sent = send(sendSock, msg, (int)strlen(msg), 0);
  if (sent == SOCKET_ERROR)
    {
        printf("connection failed: %d\n", WSAGetLastError());
        closesocket(sendSock);
        WSACleanup();
        return EXIT_SUCCESS;
    }
  else
    {
        printf("sent %d bytes !\n", sent);
        return EXIT_SUCCESS;
    }
}

int close (SOCKET closeSock)
{
  closesocket(closeSock);
  WSACleanup();
  return EXIT_SUCCESS;
}

int main ()
{


  printf("[ /x = exit ]\n");
  

  WSADATA wsaData;
  WORD DllVersion = MAKEWORD(2, 1);
  char result;


  result = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (result != NO_ERROR) 
  {
    printf("WSAStartup failed with error: %d\n", result);
    return EXIT_FAILURE;
  }

  SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
  {
    printf("socket creation failed: %d\n", WSAGetLastError());
    WSACleanup();
    return EXIT_FAILURE;
  }


  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(PORT);
  serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);


  result = connect(sock, (SOCKADDR*) &serverAddr, sizeof(serverAddr));
  if (result == SOCKET_ERROR)
    {
      printf("connect failed: %d\n", WSAGetLastError());
      WSACleanup();
      return EXIT_FAILURE;
    }


  printf("[ started! ]\n");


  while (loop == 1)
  {
      char msg[64];
      printf("enter message to send : ");
      gets(msg);

      if (msg[0] == '\\')
      {
        switch (msg[1])
        {
        case 'x':

          close(sock);
          return EXIT_SUCCESS;
        
        default:

          printf("slash command not recognized !");
          break;
        }
      
      }
      else
      {
        sendmsg(sock, msg);
      }
  }
}
