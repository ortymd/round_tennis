#ifndef LIB_SEND_RECEIVE
#define LIB_SEND_RECEIVE

#include <string.h> 
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>  /* for u_char */
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <linux/if.h>  /* it appears that when using net/if.h instead of linux/if.h
                        and compiling with:
                        gcc -c -std=c99 lib_send_receive.c
                        compiler gives error:

                        lib_send_receiv.c: In function ‘initialize_socket’:
                        lib_send_receiv.c:80:28: error: storage size of ‘ifreq_temp’ isn’t known
                                     struct   ifreq ifreq_temp;

                        not sure what might be the reason and unfortunately we do not have time
                        at the moment to check it. This is just a comment for someone experienced
                        and willing to check the issue. */

#include <errno.h>
#include <unistd.h>     /* for sleep() and close() syscalls */

/*
  It is our  EtherType protocol
  and  we will listen and send
  just our protocol
 */
#define ETHER_TYPE_PACKET 0x5555
typedef struct sockaddr SA;       /* this is general purpose sockaddr used in sendto() syscall */
typedef struct sockaddr_ll SA_LL; /* this is special sockaddr for raw sockets */
typedef struct ethhdr EH;

int  socket_fd;
SA_LL sock_addr;
char ether_frame_send [ETH_FRAME_LEN]; 
char ether_frame_receive [ETH_FRAME_LEN];
extern char *iface_name;

int socket_init();
int send_data (const char *data, size_t data_sz, const char *mac_dest);
int receive(char *buf);
void close_socket();
const char *str_error();

#endif
