#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/select.h>
#include <netdb.h>
#include <unistd.h>

typedef enum
{
    PUT_FORWARD,
    WHAT_X,
    PUT_REPLY_X,
    GET_FORWARD,
    GET_REPLY
} message_t;

typedef struct appln_msg_
{
    unsigned int msg_id;
    unsigned int k;
    unsigned int x;
    unsigned int address;
    unsigned int tcp_port;
} appln_msg_t;

void
process_message(int msg_type)
{
    
}


int main(int argc, const char * argv[]) {
   
    if(argc < 4)
    {
        printf("Provide sufficient ports\n");
        return 1;
    }
    
    int tcp_master_sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int udp_sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if(tcp_master_sock_fd == -1)
    {
        perror("TCP Master Socket");
        return 1;
    }
    
    if(udp_sock_fd == -1)
    {
        perror("UDP Socket");
        return 1;
    }
    
    fd_set readfds;
    struct sockaddr_in server_addr_tcp, server_addr_udp, nbr_addr, client_addr;
    const char *ip = argv[1];
    int udp_port = atoi(argv[2]);
    int nbr_udp_port = atoi(argv[3]);
    int tcp_port = atoi(argv[4]);
    int ret = 0;
    int comm_sock_fd = 0;
    unsigned int client_addr_len = sizeof(struct sockaddr_in);
    
    
    
    server_addr_tcp.sin_family = AF_INET;
    server_addr_tcp.sin_port = tcp_port;
    server_addr_tcp.sin_addr.s_addr = INADDR_ANY;
    
    server_addr_udp.sin_family = AF_INET;
    server_addr_udp.sin_port = udp_port;
    server_addr_udp.sin_addr.s_addr = INADDR_ANY;
    
    struct hostent *host = (struct hostent *) gethostbyname("127.0.0.1");
    
    nbr_addr.sin_family = AF_INET;
    nbr_addr.sin_port = nbr_udp_port;
    nbr_addr.sin_addr = *((struct in_addr *)host->h_addr) ;
    
    
    ret = bind(tcp_master_sock_fd,
               (struct sockaddr *)&server_addr_tcp,
               sizeof(struct sockaddr_in));
    
    if(ret == -1)
    {
        perror("TCP bind()");
        exit(EXIT_FAILURE);
    }
    
    ret = bind(udp_sock_fd,
               (struct sockaddr *)&server_addr_udp,
               sizeof(struct sockaddr_in));
    
    if(ret == -1)
    {
        perror("UDP bind()");
        exit(EXIT_FAILURE);
    }
    
    int max_fd = (tcp_master_sock_fd > udp_sock_fd) ?
                 tcp_master_sock_fd : udp_sock_fd;
    
    for(;;)
    {
        FD_SET(tcp_master_sock_fd, &readfds);
        FD_SET(udp_sock_fd, &readfds);
        FD_SET(0, &readfds);
        
        select(max_fd + 1, &readfds, NULL, NULL, NULL);
        
        if(FD_ISSET(tcp_master_sock_fd, &readfds))
        {
            comm_sock_fd = accept(tcp_master_sock_fd,
                                  (struct sockaddr *)&client_addr,
                                  &client_addr_len);
            
            if(comm_sock_fd == -1)
            {
                perror("accept()");
                exit(EXIT_FAILURE);
            }
            
            
        }
        else if(FD_ISSET(udp_sock_fd, &readfds))
        {
            
        }
        else
        {
            
        }
        
    }
    
    
    
    
    
    return 0;
}
