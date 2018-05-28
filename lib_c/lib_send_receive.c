/*
    this file for send and receive DATA  on protocol  Level 2 
    exaple
 const int size = 100
 char buffer_data[size] ; where size >= (64 - 14) && size <= 1500
 char mac_address[6] = {0xff,0xff,0xff,0xff,0xff,0xff }; broadcast
 first call initialize_socket(); 

    if returned  value 0 call finished fine
    else retun state error and it is same that errno
    or you can call func str_error() and get string value error
  socket_send(buffer_data, sizeof(buffer_data), mac);
    if error return value != 0
  cahr buff_receive[size];
  socket_receive(buff_receive);
    buffer size must be enought to get frame 
    other  unbehavior
  close_socket();
*/







/* memcpy() */
#include <string.h> 

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
//typedef unsigned char u_char;

static int  descriptor_socket;

/* data such as your mac address , index interface */
static struct   sockaddr_ll socket_address;

static char ether_frame_send   [ ETH_FRAME_LEN ], /* 1514 */
            ether_frame_receive [ ETH_FRAME_LEN ]; /* 1514 */

/* list all functions */
int initialize_socket();
int socket_send(char * , size_t , char * );
int socket_receive(char * );
void close_socket();
const char * str_error();

/*
  create description socket
  return errno 
  0 success
*/
int  initialize_socket()
{
    if (( descriptor_socket = socket (AF_PACKET, SOCK_RAW, htons(ETHER_TYPE_PACKET)) ) != -1 )
    {
            struct   ifreq ifreq_temp;
            memset(&ifreq_temp, 0, sizeof( ifreq_temp ));
            memset(&socket_address, 0, sizeof(struct sockaddr_ll));
     /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
      strncpy( ifreq_temp.ifr_name, "wlo1" , 5);
     /* !!!!!!!!!! search way to get default  interface name and set  libpcap or something else   !!!!!!!*/
     /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!    */
         unsigned long request[] = {
                                     SIOCGIFHWADDR,
                                     SIOCGIFINDEX
                                   };

           /*  ioctl  spoil data and that i am  eatch call copy  data into socket_add  */
            for (int i = 0; i < sizeof(request) / sizeof(*request); ++i)
                {
                     if ( ioctl(descriptor_socket, request[i], &ifreq_temp) )     
                        break;

                     switch(i)
                           {
                               case 0 :  memcpy(&socket_address.sll_addr,
                                                &ifreq_temp.ifr_hwaddr.sa_data,
                                                sizeof( ifreq_temp.ifr_hwaddr.sa_data)); 
                                   break; 
                               case 1 :  memcpy(&socket_address.sll_ifindex,
                                                &ifreq_temp.ifr_ifindex,
                                                sizeof( socket_address.sll_ifindex )); 
                                   break;
                           }
                }
	    socket_address.sll_halen = ETH_ALEN;
   
      }  
    return errno; 
}


/*
   data for send must be more ((60 min ether_frame) - (14 head ether_frame)) => 46 byte
   copy data from data into ether_frame_send
   set mac address destination
   set mac address source
   set EtherType
   sendto()                                
*/
int socket_send(char * data_for_send, size_t  size_data_send, char * mac_destination)
{
    if ( data_for_send && mac_destination && size_data_send )
       {
            if ( (size_data_send < (ETH_ZLEN - ETH_HLEN)) || (size_data_send  >  ETH_DATA_LEN) )
       	           errno  = EINVAL;
               else  
       	           {
       	               struct ethhdr * frame  = (struct ethhdr *)(ether_frame_send);
       	               memcpy(frame->h_dest, mac_destination, ETH_ALEN); /* set mac destination */
       	               memcpy(frame->h_source, socket_address.sll_addr, ETH_ALEN); /* set my mac address  */
       	               frame->h_proto =  htons(ETHER_TYPE_PACKET);               /* set type protocol */
                       memcpy(ether_frame_send + sizeof( struct ethhdr), data_for_send, size_data_send); /* copy data into buffer_send */
                       sendto(descriptor_socket,
                              ether_frame_send,  
                              size_data_send + ETH_HLEN,
                              0,
                             (struct sockaddr*)&socket_address, 
                             sizeof(struct sockaddr_ll));
       	           }
       }
    return errno;
}



/* copy receive frame into  buffer  */
int socket_receive(char * buffer_user)
{  
    /* recv sleep untill ether_frame doesnt arrive */
    size_t size_frame_receiv = recv(descriptor_socket, ether_frame_receive, ETH_DATA_LEN, 0); 
    if ( buffer_user && !errno ) /* buffer_user != NULL and errno is 0 */
       memcpy(buffer_user, ether_frame_receive, size_frame_receiv);
 
    return errno;
}


void close_socket()
{
    close(descriptor_socket);
}


/*
  return str error human readable
*/
const char * str_error()
{
    return strerror( errno ); 
}


