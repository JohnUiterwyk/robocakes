#include "tcp_lib.h"

tcp_conn_data_t * tcp_new_conn_data()
{
    tcp_conn_data_t * data;
    data = calloc(1, sizeof(tcp_conn_data_t));
    data->dest_ip_address = calloc(INET6_ADDRSTRLEN, sizeof(char));
    data->port = calloc(12, sizeof(char));
    data->socket_fd = -1;
    data->socket_addr_info = NULL;
    data->server_addr_info = NULL;
    data->socket_type = -1;
    data->send_buffer = thread_new_shared_buffer();
    data->recv_buffer = thread_new_shared_buffer();
    FD_ZERO(data->clients);
    return data;
}

void free_conn_data(tcp_conn_data_t * data)
{
    
}

void tcp_create_server(tcp_conn_data_t * conn_data)
{
    int listener;     // listening socket descriptor
    struct addrinfo hints, *ai, *p;
    int yes=1;        // for setsockopt() SO_REUSEADDR, below
    int return_val;
    
    // get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    return_val = getaddrinfo(NULL, conn_data->port, &hints, &ai);
    if (return_val != 0) {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(return_val));
        exit(1);
    }
    
    for(p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) {
            continue;
        }
        
        // lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        
        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }
        break;
    }
    if (p == NULL) {
        fprintf(stderr, "selectserver: failed to bind\n");
        exit(2);
    }
    tcp_print_addr_info(p);
    freeaddrinfo(ai); // all done with this
    // listen
    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(3);
    }
    conn_data->socket_fd = listener;
    
}

void tcp_start_server(tcp_conn_data_t * conn_data)
{
    
    fd_set master;    // master file descriptor list
    fd_set read_fds;  // temp file descriptor list for select()
    int max_socket;        // maximum file descriptor number
    int client_socket;        // newly accept()ed socket descriptor
    
    char buffer[MAX_BUFFER_LEN];    // buffer for recv
    int num_bytes;
    
    int i;
    
    FD_ZERO(&master);    // clear the master and temp sets
    FD_ZERO(&read_fds);
    
    FD_SET(conn_data->socket_fd, &master);
    max_socket = conn_data->socket_fd; // so far, it's this one
    
    
    
    
    while(1)
    {
        read_fds = master; // copy it
        if (select(max_socket+1, &read_fds, NULL, NULL, NULL) == -1)
        {
            perror("select");
            exit(4);
        }
        for(i=0;i<=max_socket;i++)
        {
            if(FD_ISSET(i, &read_fds))
            {
                //socket active
                if(i == conn_data->socket_fd)
                {
                    //new client trying to connect
                    client_socket = tcp_accept_connection(conn_data->socket_fd);
                    if (client_socket != -1)
                    {
                        FD_SET(client_socket, &master); // add to master set
                        if (client_socket > max_socket)
                        {    // keep track of the max
                            max_socket = client_socket;
                        }
                    }
                    printf("new socket %d !\n", client_socket);
                    
                }else
                {
                    //incoming message from client
                    num_bytes = (int)recv(i, buffer, sizeof(buffer)-1, 0);
                    if (num_bytes <= 0)
                    {
                        // got error or connection closed by client
                        if (num_bytes == 0)
                        {
                            // connection closed
                            printf("socket %d hung up\n", i);
                        } else {
                            perror("recv");
                        }
                        close(i); // bye!
                        FD_CLR(i, &master); // remove from master set
                    }else
                    {
                        buffer[num_bytes] = '\0';
                        
                        printf("got message: %s\n",buffer);
                        thread_copy_to_buffer(conn_data->recv_buffer, buffer);
                    }
                }
            }
        }//end for loop through sockets
        
        if(thread_copy_from_buffer(conn_data->send_buffer, buffer) == 1)
        {
            printf("send message: %s\n",buffer);
            //message waiting, so send it
            for(i = 0; i <= max_socket; i++) {
                // send to everyone!
                if (FD_ISSET(i, &master)) {
                    // except the server socket
                    if (i != conn_data->socket_fd)
                    {
                        //switch to sendall??
                        if (send(i, buffer, strlen(buffer), 0) == -1)
                        {
                            perror("send");
                        }
                    }
                }
            }
        }
        
    }// end while loop
    
}


int tcp_accept_connection(int listener)
{
    struct sockaddr_storage remote_address; // client address
    socklen_t address_len;
    char remote_ip[INET6_ADDRSTRLEN];
    int client_socket;
    
    address_len = sizeof remote_address;
    client_socket = accept(listener,
                   (struct sockaddr *)&remote_address,
                   &address_len);
    
    if (client_socket != -1)
    {
        inet_ntop(remote_address.ss_family,
                  tcp_get_in_addr((struct sockaddr*)&remote_address),
                  remote_ip, INET6_ADDRSTRLEN);
        printf("new connection from %s on socket %d\n",remote_ip,client_socket);
    }else
    {
        perror("error tring to accept connection");
    }
    return client_socket;
}

void tcp_create_client(tcp_conn_data_t * conn_data)
{
    int client_socket;     // listening socket descriptor
    struct addrinfo hints, *ai, *p;
    int return_val;
    char remote_ip[INET6_ADDRSTRLEN];
    
    int yes=1;        // for setsockopt() SO_REUSEADDR, below
    
    // get us a socket and bind it
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    return_val = getaddrinfo(conn_data->dest_ip_address, conn_data->port, &hints, &ai);
    if (return_val != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(return_val));
        exit(1);
    }
    
    // loop through all the results and connect to the first we can
    for(p = ai; p != NULL; p = p->ai_next) {
        tcp_print_addr_info(p);
        if ((client_socket = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }
        
        if (connect(client_socket, p->ai_addr, p->ai_addrlen) == -1) {
            close(client_socket);
            perror("client: connect");
            continue;
        }
        
        break;
    }
    
    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        exit(2);
    }

    inet_ntop(p->ai_family, tcp_get_in_addr((struct sockaddr *)p->ai_addr), remote_ip, sizeof remote_ip);
    printf("client: connecting to %s\n", remote_ip);
    
    freeaddrinfo(ai); // all done with this
    conn_data->socket_fd = client_socket;
}


void tcp_start_client_listener(tcp_conn_data_t * conn_data)
{
    char buffer[MAX_BUFFER_LEN];    // buffer for recv
    int num_bytes;
    struct timespec sleepTime, sleepTimeResult; //used so thread doesnt hog
    
    sleepTime.tv_sec = 0;
    sleepTime.tv_nsec = 0.001 * 1000000000;
    while(1)
    {
        num_bytes = (int)recv(conn_data->socket_fd, buffer, MAX_BUFFER_LEN-1, 0);
        if (num_bytes == -1)
        {
            perror("recv");
            exit(1);
        }
        
        buffer[num_bytes] = '\0';
        printf("got message: %s\n",buffer);
        thread_copy_to_buffer(conn_data->recv_buffer, buffer);
        nanosleep(&sleepTime, &sleepTimeResult);
    }
}


void tcp_start_client_sender(tcp_conn_data_t * conn_data)
{
    
    char buffer[MAX_BUFFER_LEN]; 
    int updated = 0;
    struct timespec sleepTime, sleepTimeResult; //used so thread doesnt hog
    int sent;
    
    
    sleepTime.tv_sec = 0;
    sleepTime.tv_nsec = 0.001 * 1000000000;
    while(1)
    {
        updated = thread_copy_from_buffer(conn_data->send_buffer, buffer);
        if(updated)
        {
            printf("sending %d bytes \n",(int)strlen(buffer));
            //message waiting, so send it
            sent = (int)send(conn_data->socket_fd, buffer, strlen(buffer), 0);
            if (sent == -1)
            {
                perror("send");
            }else
            {
                printf("sent %d bytes \n",sent);
            }
        }
        nanosleep(&sleepTime, &sleepTimeResult);
    }
}


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

















