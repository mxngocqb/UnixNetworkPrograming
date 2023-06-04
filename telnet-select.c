#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_CLIENTS 10

int main() {
    int server_socket, client_sockets[MAX_CLIENTS], max_clients = MAX_CLIENTS;
    fd_set readfds;
    int max_fd, activity, i, valread, client_socket, sd;
    struct sockaddr_in server_address, client_address;
    char buffer[1024] = {0};
    char server_response[] = "Chào mừng đến với server telnet!\n";

    // Tạo socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        printf("Không thể tạo socket.\n");
        return 1;
    }

    // Cấu hình địa chỉ server
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8888);

    // Ràng buộc địa chỉ server vào socket
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        printf("Ràng buộc địa chỉ vào socket thất bại.\n");
        return 1;
    }

    // Lắng nghe kết nối đến từ client
    listen(server_socket, MAX_CLIENTS);
    printf("Server đang lắng nghe kết nối...\n");

    // Khởi tạo mảng client_sockets
    for (i = 0; i < max_clients; i++) {
        client_sockets[i] = 0;
    }

    while (1) {
        // Xóa tập hợp readfds và thêm server_socket vào đó
        FD_ZERO(&readfds);
        FD_SET(server_socket, &readfds);
        max_fd = server_socket;

        // Thêm các client_socket đã kết nối vào tập hợp readfds
        for (i = 0; i < max_clients; i++) {
            sd = client_sockets[i];

            if (sd > 0) {
                FD_SET(sd, &readfds);
            }

            if (sd > max_fd) {
                max_fd = sd;
            }
        }

        // Sử dụng select để theo dõi các socket
        activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0) {
            printf("Lỗi trong quá trình sử dụng hàm select.\n");
            return 1;
        }

        // Kiểm tra xem server_socket có kết nối mới không
        if (FD_ISSET(server_socket, &readfds)) {
            if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, (socklen_t *)&client_address)) < 0) {
                printf("Kết nối không hợp lệ.\n");
                return 1;
            }

            printf("Kết nối mới được chấp nhận, socket fd là %d, IP là: %s, cổng là: %d\n", client_socket, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

            // Gửi lời chào đến client mới
            if (send(client_socket, server_response, strlen(server_response), 0) != strlen(server_response)) {
                printf("Không thể gửi lời chào.\n");
            }

            // Thêm client_socket vào mảng client_sockets
            for (i = 0; i < max_clients; i++) {
                if (client_sockets[i] == 0) {
                    client_sockets[i] = client_socket;
                    break;
                }
            }
        }

        // Xử lý dữ liệu từ client đã kết nối
        for (i = 0; i < max_clients; i++) {
            sd = client_sockets[i];

            if (FD_ISSET(sd, &readfds)) {
                // Đọc dữ liệu từ client
                if ((valread = read(sd, buffer, 1024)) == 0) {
                    // Client ngắt kết nối
                    getpeername(sd, (struct sockaddr *)&client_address, (socklen_t *)&client_address);
                    printf("Client đã ngắt kết nối, IP là: %s, cổng là: %d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

                    // Đóng socket và xoá khỏi mảng client_sockets
                    close(sd);
                    client_sockets[i] = 0;
                } else {
                    // Xử lý lệnh từ client
                    buffer[valread] = '\0';
                    printf("Lệnh từ client: %s\n", buffer);

                    // Thực hiện lệnh system và gửi kết quả về client
                    FILE *fp = popen(buffer, "r");
                    if (fp != NULL) {
                        memset(buffer, 0, sizeof(buffer));
                        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
                            send(sd, buffer, strlen(buffer), 0);
                            memset(buffer, 0, sizeof(buffer));
                        }
                        pclose(fp);
                    } else {
                        printf("Lỗi khi thực hiện lệnh system.\n");
                    }
                }
            }
        }
    }

    // Đóng socket server
    close(server_socket);

    return 0;
}
