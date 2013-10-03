#include "tcp_lib.h"



int tcp_send_full_message(int socketfd, char *buffer, int *length)
{
    int total = 0;        // how many bytes we've sent
    int bytesleft = *length; // how many we have left to send
    int sent;
    
    while(total < *length) {
        sent = (int)send(socketfd, buffer+total, bytesleft, 0);
        if (sent == -1) { break; }
        total += sent;
        bytesleft -= sent;
    }
    
    *length = total; // return number actually sent here
    if (sent == -1)
    {
        return -1;
    }
    return 0; // return -1 on failure, 0 on success
}
void * tcp_get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


void tcp_print_addr_info(struct addrinfo * addrInfo)
{
    char ipstr[INET6_ADDRSTRLEN];
    void *addr;
    char *ipver;
    
    // get the pointer to the address itself,
    // different fields in IPv4 and IPv6:
    if (addrInfo->ai_family == AF_INET) { // IPv4
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)addrInfo->ai_addr;
        addr = &(ipv4->sin_addr);
        ipver = "IPv4";
    } else { // IPv6
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)addrInfo->ai_addr;
        addr = &(ipv6->sin6_addr);
        ipver = "IPv6";
    }
    
    // convert the IP to a string and print it:
    inet_ntop(addrInfo->ai_family, addr, ipstr, sizeof ipstr);
    printf("  %s: %s\n", ipver, ipstr);
}

















