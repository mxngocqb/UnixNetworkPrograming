#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

typedef struct sockaddr SOCKADDR;
typedef struct sockaddr_in SOCKADDR_IN;
int main()
{
    SOCKADDR_IN saddr, caddr;    
    int clen = sizeof(caddr);
    // tao socket
    int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(8888);
    saddr.sin_addr.s_addr = 0;
    // B2
    bind(s, (SOCKADDR*)&saddr, sizeof(SOCKADDR));
    // lắng nghe yêu cầu kết nôi
    listen(s, 10);
    // chấp nhận kết nối - nào xong thì nó sẽ trả ra socket c để có thể dụng
    int c = accept(s, (SOCKADDR*)&caddr, (socklen_t*)&clen);
    const char* welcome = "Hello TCP Server\n";
    // gui dư liêuuj
    int sent = send(c, welcome, strlen(welcome), 0);
    printf("Sent: %d bytes\n", sent);
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    // nhận dữ liệu
    int received = recv(c, buffer, sizeof(buffer) - 1, 0);
    printf("Received: %d bytes\n %s", received, buffer);
    close(c);
    close(s);
}