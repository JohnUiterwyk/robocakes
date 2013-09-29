//
//  UdpSendMessage.c
//  RoboMessage
//
//  Created by John Uiterwyk on 9/15/13.
//  Copyright (c) 2013 John Uiterwyk. All rights reserved.
//

#include "udp_lib.h"

udp_conn_data_t * udp_new_conn_data()
{
    udp_conn_data_t * result;
    result = calloc(1, sizeof(udp_conn_data_t));
    if(result == NULL)
    {
        perror("Error ");
    }
    return result;
}

void udp_create_socket(udp_conn_data_t * conn_data)
{
    struct addrinfo hints, *ai_ptr;
    int get_info_result, bind_result, broadcast_result;
    int broadcast;

    //set to zero
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    //get address info passing in some helpful hints
    //null as the first value with AI_PASSIVE means use my ip
    //stores result in serverInfo
    if(conn_data->socket_type == SOCKET_TYPE_LISTEN)
    {
        hints.ai_flags = AI_PASSIVE; //use my ip
        get_info_result = getaddrinfo(NULL,
                                    conn_data->port,
                                    &hints,
                                    &(conn_data->server_addr_info));
    }else
    {
        get_info_result = getaddrinfo(conn_data->dest_ip_address,
                                    conn_data->port,
                                    &hints,
                                    &(conn_data->server_addr_info));
    }


    if(get_info_result != 0)
    {
        fprintf(stderr,
                "RoboClient Error: getaddrinfo: %s\n",
                gai_strerror(get_info_result));
        exit(EXIT_FAILURE);
    }
    //now we look through results and bind
    for(ai_ptr = conn_data->server_addr_info; ai_ptr != NULL ; ai_ptr = ai_ptr->ai_next)
    {
        udp_print_address_info(ai_ptr);
        bind_result = -1;
        //try to get the socket fd
        conn_data->socket_fd = socket(ai_ptr->ai_family,
                                ai_ptr->ai_socktype,
                                ai_ptr->ai_protocol);
        if(conn_data->socket_fd == -1)
        {
            //error getting socket
            perror("RoboClient: socket error");
            continue; //try next address info
        }

        if(conn_data->socket_type == SOCKET_TYPE_LISTEN)
        {
            //if no error, bind
            bind_result = bind(conn_data->socket_fd,
                              ai_ptr->ai_addr,
                              ai_ptr->ai_addrlen);
            if(bind_result == -1)
            {
                close(conn_data->socket_fd);
                perror("RoboClient: bind error.");
                continue; //try next address info
            }
        }
        //if we reach here, we have valid socket (and bind result if listening)
        //so break the for loop
        break;
    }

    if(ai_ptr == NULL)
    {
        //we didnt manage to find a socker to use to anything
		fprintf(stderr, "RoboClient: failed to bind socket\n");
        exit(EXIT_FAILURE);
    }else
    {
        conn_data->socket_addr_info = ai_ptr;
    }

    if (conn_data->socket_type == SOCKET_TYPE_BROADCAST) {
        broadcast =1;
        broadcast_result = setsockopt(conn_data->socket_fd,
                                     SOL_SOCKET,
                                     SO_BROADCAST,
                                     &broadcast,
                                     sizeof broadcast);
        if (broadcast_result == -1) {
            perror("setsockopt (SO_BROADCAST)");
            exit(1);
        }
    }

}
void udp_send_message(udp_conn_data_t * conn_data, char * message)
{
    long numBytes;
    numBytes = sendto(conn_data->socket_fd,
                      message,
                      strlen(message),
                      0,
                      conn_data->socket_addr_info->ai_addr,
                      conn_data->socket_addr_info->ai_addrlen);
    printf("RoboServer: sent %d bytes to %s\n", (int)numBytes, conn_data->dest_ip_address);
    /* fflush(stdout); */
    
}
void udp_receive_message(udp_conn_data_t * conn_data, char * message)
{

    struct sockaddr_storage sender_addr;
    socklen_t address_length;
    long num_bytes;
    char ip_string[INET6_ADDRSTRLEN];

    address_length = sizeof(sender_addr);
    num_bytes = recvfrom(conn_data->socket_fd,
                        message,
                        MAX_BUFFER_LEN-1,
                        0,
                        (struct sockaddr *)&sender_addr,
                        &address_length);
    if(num_bytes == -1)
    {
        perror("RoboClient: recvfrom problem");
        exit(EXIT_FAILURE);
    }

    //put the null terminator in the right place
    message[num_bytes] = '\0';

    //get the senders ip as a string
    inet_ntop(sender_addr.ss_family,
              udp_get_ip_as_string((struct sockaddr *)&sender_addr),
              ip_string,
              sizeof(ip_string));

    // print the message!
    printf("Got %d bytes from %s: %s\n",(int)num_bytes,ip_string,message);

}

void * udp_get_ip_as_string(struct sockaddr *socket_address)
{
    struct sockaddr_in * socket_addr_ipv4;
    struct sockaddr_in6 * socket_addr_ipv6;
    void * result = NULL;
    //this gets the ip address cast to the IPv4 or IPv6 structure
    if(socket_address->sa_family == AF_INET)
    {
        //its IPv4
        socket_addr_ipv4 = (struct sockaddr_in*)socket_address;
        result = &(socket_addr_ipv4->sin_addr);

    }else
    {
        socket_addr_ipv6 = (struct sockaddr_in6*)socket_address;
        result = &(socket_addr_ipv6->sin6_addr);
    }
    return result;
}

void udp_print_address_info(struct addrinfo * address_info)
{
    char ip_string[INET6_ADDRSTRLEN];
    void *address;
    char *ipversion;

    // get the pointer to the address itself,
    // different fields in IPv4 and IPv6:
    if (address_info->ai_family == AF_INET) { // IPv4
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)address_info->ai_addr;
        address = &(ipv4->sin_addr);
        ipversion = "IPv4";
    } else { // IPv6
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)address_info->ai_addr;
        address = &(ipv6->sin6_addr);
        ipversion = "IPv6";
    }

    // convert the IP to a string and print it:
    inet_ntop(address_info->ai_family, address, ip_string, sizeof ip_string);
    printf("  %s: %s\n", ipversion, ip_string);
}
