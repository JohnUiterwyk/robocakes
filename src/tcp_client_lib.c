#include "tcp_client_lib.h"

tcp_client_data_t * tcp_client_data_new()
{
    tcp_client_data_t * data;
    data = calloc(1, sizeof(tcp_client_data_t));
    data->dest_ip_address = calloc(INET6_ADDRSTRLEN, sizeof(char));
    data->port = calloc(12, sizeof(char));
    data->socket_fd = -1;
    data->send_buffer = thread_new_shared_buffer();
    data->recv_buffer = thread_new_shared_buffer();
    data->alive  = 1;
    return data;
}

void tcp_client_data_free(tcp_client_data_t * data)
{
    free(data->dest_ip_address);
    thread_free_shared_buffer(data->send_buffer);
    thread_free_shared_buffer(data->recv_buffer);
    free(data);
}

void tcp_client_start(tcp_client_data_t * client)
{
    tcp_client_connect(client);
    pthread_create(&client->send_thread, NULL, &tcp_client_send_loop, client);
    pthread_create(&client->recv_thread, NULL, &tcp_client_recv_loop, client);
    
    
}

void tcp_client_stop(tcp_client_data_t * client)
{
    client->alive = 0;
    close(client->socket_fd);
    pthread_join(client->recv_thread,NULL);
    pthread_join(client->send_thread,NULL);
}


void tcp_client_connect(tcp_client_data_t * client)
{
    int client_socket;    
    struct addrinfo hints, *addr_info, *p;
    int return_val;
    char remote_ip[INET6_ADDRSTRLEN];
    
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    return_val = getaddrinfo(client->dest_ip_address, client->port, &hints, &addr_info);
    if (return_val != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(return_val));
        exit(1);
    }
    
    //find a socket to bind to
    for(p = addr_info; p != NULL; p = p->ai_next) {
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
    
    freeaddrinfo(addr_info); // all done with this
    client->socket_fd = client_socket;
}


void * tcp_client_recv_loop(void * data)
{
    tcp_client_data_t * client;
    char buffer[MAX_BUFFER_LEN];    
    int num_bytes;
    struct timespec sleepTime, sleepTimeResult; //used so thread doesnt hog cpu
    
    client = (tcp_client_data_t *) data;
    
    sleepTime.tv_sec = 0;
    sleepTime.tv_nsec = 0.001 * 1000000000;
    while(client->alive)
    {
        // add time out
        //http://stackoverflow.com/questions/2876024/linux-is-there-a-read-or-recv-from-socket-with-timeout
        
        num_bytes = (int)recv(client->socket_fd, buffer, MAX_BUFFER_LEN-1, 0);
        if (num_bytes == -1)
        {
            perror("recv");
            return NULL;
        }else if (num_bytes == 0)
        {
            close(client->socket_fd);
        }
        
        buffer[num_bytes] = '\0';
        printf("client recieved %d bytes.\n",num_bytes);
        thread_copy_to_buffer(client->recv_buffer, buffer);
        nanosleep(&sleepTime, &sleepTimeResult);
    }
    return NULL;
}


void * tcp_client_send_loop(void * data)
{
    tcp_client_data_t * client;
    char buffer[MAX_BUFFER_LEN];
    int updated = 0;
    struct timespec sleepTime, sleepTimeResult; //used so thread doesnt hog
    int sent;
    
    client = (tcp_client_data_t *) data;
    
    
    sleepTime.tv_sec = 0;
    sleepTime.tv_nsec = 0.001 * 1000000000;
    while(client->alive)
    {
        updated = thread_copy_from_buffer(client->send_buffer, buffer);
        if(updated)
        {
            //message waiting, so send it
            sent = (int)send(client->socket_fd, buffer, strlen(buffer), 0);
            if (sent == -1)
            {
                perror("send");
            }else
            {
                printf("client sent %d bytes \n",sent);
            }
        }
        nanosleep(&sleepTime, &sleepTimeResult);
    }
    return NULL;
}

