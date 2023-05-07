#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <malloc.h>
#include <string.h>
#include <arpa/inet.h>
const char* domain = "google.com";
struct addrinfo *res = NULL;

int main()
{
    /*in_addr addr;
    memset(&addr, 0, sizeof(addr));
    inet_aton("111.65.250.2", &addr);
    hostent* phost = gethostbyaddr(&addr, sizeof(addr), AF_INET);
    printf("...");*/
    hostent* phost = gethostbyname("vnexpress.net");
    if (phost != NULL)
    {
        int i = 0;
        while (phost->h_addr_list[i] != NULL)
        {
            printf("%d.%d.%d.%d\n", (unsigned char)phost->h_addr_list[i][0], 
                                    (unsigned char)phost->h_addr_list[i][1],
                                    (unsigned char)phost->h_addr_list[i][2],
                                    (unsigned char)phost->h_addr_list[i][3]);
            i += 1;
        }
    }
    printf("...");
    /*int error = getaddrinfo(domain, NULL, NULL, &res);
    if (error == 0)
    {
        while (res != NULL)
        {
            printf("...");    
            sockaddr_in* sin = (sockaddr_in*)res->ai_addr; 
            if (sin->sin_family == AF_INET)
            {
                char* ipstr = inet_ntoa(sin->sin_addr);
                printf("%s\n", ipstr);
                //unsigned char* ip = (unsigned char*)&(sin->sin_addr.s_addr);
                //printf("%d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
            }else if (sin->sin_family == AF_INET6)                                                                                                                                                                                                                                                                                                                                  ccc           ccxxdxxxd
            {
                printf("IPv6\n");
            }
            res = res->ai_next;    
        }           
    }else
    {
        printf("Error\n");
    }*/
}