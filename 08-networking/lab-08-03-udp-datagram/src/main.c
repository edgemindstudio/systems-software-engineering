#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(void)
{
    int receiver = socket(AF_INET, SOCK_DGRAM, 0);
    int sender = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in recv_addr;
    struct sockaddr_in peer;
    socklen_t recv_len = (socklen_t)sizeof recv_addr;
    socklen_t peer_len = (socklen_t)sizeof peer;
    char buf[64];
    ssize_t n;

    if (receiver < 0 || sender < 0) { perror("socket"); return 1; }

    memset(&recv_addr, 0, sizeof recv_addr);
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    recv_addr.sin_port = htons(0);

    if (bind(receiver, (struct sockaddr *)&recv_addr, sizeof recv_addr) != 0 ||
        getsockname(receiver, (struct sockaddr *)&recv_addr, &recv_len) != 0) {
        perror("bind/getsockname");
        return 1;
    }

    if (sendto(sender, "udp-message", 11U, 0,
               (struct sockaddr *)&recv_addr, sizeof recv_addr) != 11) {
        perror("sendto");
        return 1;
    }

    n = recvfrom(receiver, buf, sizeof buf - 1U, 0,
                 (struct sockaddr *)&peer, &peer_len);
    if (n <= 0) { perror("recvfrom"); return 1; }
    buf[(size_t)n] = '\0';

    printf("receiver_port=%u\n", (unsigned)ntohs(recv_addr.sin_port));
    printf("received=%s\n", buf);
    printf("peer_family_ipv4=%s\n", peer.sin_family == AF_INET ? "yes" : "no");
    printf("udp_exchange_verified=%s\n",
           strcmp(buf, "udp-message") == 0 ? "yes" : "no");

    close(sender);
    close(receiver);
    return 0;
}
