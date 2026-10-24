#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

int main(void)
{
    struct addrinfo hints;
    struct addrinfo *result = NULL;
    struct addrinfo *entry;
    int count = 0;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    {
        int rc = getaddrinfo("localhost", "80", &hints, &result);
        if (rc != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
            return 1;
        }
    }

    for (entry = result; entry != NULL; entry = entry->ai_next) {
        char host[128];
        int rc = getnameinfo(entry->ai_addr, entry->ai_addrlen,
                             host, sizeof host,
                             NULL, 0,
                             NI_NUMERICHOST);
        if (rc == 0) {
            printf("family=%s address=%s\n",
                   entry->ai_family == AF_INET ? "IPv4" :
                   entry->ai_family == AF_INET6 ? "IPv6" : "other",
                   host);
            ++count;
        }
    }

    freeaddrinfo(result);
    printf("resolved_entries=%d\n", count);
    printf("resolution_success=%s\n", count > 0 ? "yes" : "no");
    return count > 0 ? 0 : 1;
}
