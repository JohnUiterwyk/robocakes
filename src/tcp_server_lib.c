/**
 * \file tcp_server_lib.c
 * Server communication when network is using TCP
 */
#include "tcp_server_lib.h"

tcp_server_data_t * tcp_server_data_new()
{
        tcp_server_data_t * data;
        data = calloc(1, sizeof(tcp_server_data_t));
        data->port = calloc(12, sizeof(char));
        data->socket_fd = -1;
        data->send_buffer = thread_new_shared_buffer();
        data->recv_buffer = thread_new_shared_buffer();
        data->max_client_fd = -1;
        data->alive = 1;
        FD_ZERO(&data->clients);
        return data;
}

void tcp_server_data_free(tcp_server_data_t * server)
{
        thread_free_shared_buffer(server->send_buffer);
        thread_free_shared_buffer(server->recv_buffer);
        free(server->port);
        free(server);
}


void tcp_server_start(tcp_server_data_t * server)
{
        tcp_server_create_socket(server);
        pthread_create(&server->accept_thread, NULL, &tcp_server_accept_loop,
                        server);
        pthread_create(&server->recv_thread, NULL, &tcp_server_recv_loop,
                        server);
        pthread_create(&server->send_thread, NULL, &tcp_server_send_loop,
                        server);

}
void tcp_server_stop(tcp_server_data_t * server)
{
        server->alive = 0;
        pthread_join(server->send_thread,NULL);
        pthread_join(server->recv_thread,NULL);
        close(server->socket_fd);
        pthread_join(server->accept_thread,NULL);
}

void tcp_server_create_socket(tcp_server_data_t * server)
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

        return_val = getaddrinfo(NULL, server->port, &hints, &ai);
        if (return_val != 0) {
                fprintf(stderr, "selectserver: %s\n",
                                gai_strerror(return_val));
                exit(1);
        }

        for(p = ai; p != NULL; p = p->ai_next) {
                listener = socket(p->ai_family, p->ai_socktype,
                                p->ai_protocol);
                if (listener < 0) {
                        continue;
                }

                // lose the pesky "address already in use" error message
                setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes,
                                sizeof(int));

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
        server->socket_fd = listener;

}

void * tcp_server_accept_loop(void * data)
{
        tcp_server_data_t * server;
        int client_socket;
        server = (tcp_server_data_t *) data;
        while(server->alive)
        {
                //new client trying to connect
                client_socket = tcp_server_accept_connection(
                                server->socket_fd);
                if (client_socket != -1)
                {
                        /* add to master set */
                        FD_SET(client_socket, &(server->clients));
                        if (client_socket > server->max_client_fd)
                        {    // keep track of the max
                                server->max_client_fd = client_socket;
                        }
                }
        }
        return NULL;
}
int tcp_server_accept_connection(int listener)
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
                                tcp_get_in_addr((struct sockaddr*)
                                        &remote_address),
                                remote_ip, INET6_ADDRSTRLEN);
                printf("new connection from %s on socket %d\n", remote_ip,
                                client_socket);
        }else
        {
                perror("error tring to accept connection");
        }
        return client_socket;
}

void * tcp_server_recv_loop(void * data)
{
        tcp_server_data_t * server;
        fd_set fd_list;  // temp file descriptor list for select()

        char buffer[MAX_BUFFER_LEN];
        int num_bytes;

        int i;

        struct timeval timeout;
        server = (tcp_server_data_t *) data;

        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        FD_ZERO(&fd_list);


        while(server->alive)
        {
                //duplicate the client fd set
                fd_list = server->clients;
                if (select(server->max_client_fd+1, &fd_list, NULL, NULL,
                                        &timeout) == -1)
                {
                        perror("select");
                        exit(4);
                }
                for(i = 0;i <= server->max_client_fd;i++)
                {
                        if(FD_ISSET(i, &fd_list))
                        {
                                //incoming message from client
                                num_bytes = (int)recv(i, buffer,
                                                sizeof(buffer)-1, 0);
                                if (num_bytes <= 0)
                                {
                                        /* got error or connection
                                         * closed by client
                                         */
                                        if (num_bytes == 0)
                                        {
                                                // connection closed
                                                printf("socket %d hung up\n",
                                                                i);
                                        } else {
                                                perror("recv");
                                        }
                                        close(i); // bye!
                                        /* remove from master set */
                                        FD_CLR(i, &server->clients); 
                                }else
                                {
                                        buffer[num_bytes] = '\0';

                                        printf("server recieved %d bytes.\n",
                                                        num_bytes);
                                        thread_copy_to_buffer(
                                                        server->recv_buffer,
                                                        buffer);
                                }
                        }

                }
        }
        return NULL;
}

void * tcp_server_send_loop(void * data)
{
        tcp_server_data_t * server;
        char buffer[MAX_BUFFER_LEN];
        int i,sent;
        struct timespec sleepTime, sleepTimeResult;
        server = (tcp_server_data_t *) data;

        sleepTime.tv_sec = 0;
        sleepTime.tv_nsec = 0.001 * 1000000000;
        while(server->alive)
        {
                if(thread_copy_from_buffer(server->send_buffer, buffer) == 1)
                {
                        //message waiting, so send it
                        for(i = 0; i <= server->max_client_fd; i++) {
                                // send to everyone!
                                if (FD_ISSET(i, &(server->clients)))
                                {
                                        //switch to sendall??
                                        sent = (int)send(i, buffer, strlen(buffer), 0);
                                        if (sent == -1)
                                        {
                                                perror("send");
                                        }else
                                        {
                                                printf("sever sent %d bytes.\n",sent);
                                        }
                                }
                        }
                }
                nanosleep(&sleepTime, &sleepTimeResult);
        }
        return NULL;
}
